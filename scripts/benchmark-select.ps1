param(
    [string]$BuildDir,
    [int[]]$Seeds = @(20260423, 20260424, 20260425),
    [int]$GamesPerSeed = 32,
    [int]$Depth = 4,
    [int]$CheckpointEvery = 2,
    [double]$Alpha = 0.005,
    [double]$Lambda = 0.5,
    [double]$Epsilon = 0.02,
    [int]$EpsilonPlies = 6,
    [double]$TerminalReward = 1200.0,
    [double]$TdErrorClip = 0.0,
    [int]$TerminalOnlyWarmup = 0,
    [int]$NearTerminalCurriculum = 0
)

. (Join-Path $PSScriptRoot 'common.ps1')

if (-not $Seeds -or $Seeds.Count -eq 0) {
    throw 'Selection requires at least one seed.'
}

if ($GamesPerSeed -le 0) {
    throw 'Selection requires -GamesPerSeed to be greater than 0.'
}

if ($CheckpointEvery -le 0) {
    throw 'Selection requires -CheckpointEvery to be greater than 0 so at least one checkpoint is archived.'
}

if ($Alpha -le 0.0) {
    throw 'Selection requires -Alpha to be greater than 0.'
}

if ($Lambda -lt 0.0) {
    throw 'Selection requires -Lambda to be non-negative.'
}

if ($Epsilon -lt 0.0) {
    throw 'Selection requires -Epsilon to be non-negative.'
}

if ($EpsilonPlies -lt 0) {
    throw 'Selection requires -EpsilonPlies to be non-negative.'
}

if ($TerminalReward -le 0.0) {
    throw 'Selection requires -TerminalReward to be greater than 0.'
}

if ($TdErrorClip -lt 0.0) {
    throw 'Selection requires -TdErrorClip to be non-negative.'
}

if ($TerminalOnlyWarmup -lt 0) {
    throw 'Selection requires -TerminalOnlyWarmup to be non-negative.'
}

if ($NearTerminalCurriculum -lt 0) {
    throw 'Selection requires -NearTerminalCurriculum to be non-negative.'
}

$repoRoot = Get-SurakartaRepoRoot
if (-not $BuildDir) {
    $BuildDir = Join-Path $repoRoot 'build-vs-train-release'
}
$BuildDir = [System.IO.Path]::GetFullPath($BuildDir)

$benchmarkExe = Join-Path $BuildDir 'bin\surakarta-benchmark.exe'
$outputRoot = Join-Path $repoRoot 'build-local\training'
$timestamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$sessionRoot = Join-Path $outputRoot $timestamp
$selectionRoot = Join-Path $sessionRoot 'selection'
$summaryJsonPath = Join-Path $sessionRoot 'selection-summary.json'
$summaryTextPath = Join-Path $sessionRoot 'selection-summary.txt'

function New-SelectionRunLayout {
    param(
        [string]$Root,
        [int]$Seed
    )

    $runName = 'seed-{0}' -f $Seed
    $runRoot = Join-Path $Root $runName
    $checkpointsDir = Join-Path $runRoot 'checkpoints'
    $checkpointEvalDir = Join-Path $runRoot 'checkpoint-evals'
    $logsDir = Join-Path $runRoot 'logs'

    New-Item -ItemType Directory -Force -Path $checkpointsDir, $checkpointEvalDir, $logsDir | Out-Null

    return [pscustomobject]@{
        Seed = $Seed
        Name = $runName
        RunRoot = $runRoot
        CheckpointsDir = $checkpointsDir
        CheckpointEvalDir = $checkpointEvalDir
        LogsDir = $logsDir
        BootstrapWeights = Join-Path $runRoot 'bootstrap.bin'
        CandidateWeights = Join-Path $runRoot 'candidate.bin'
        EvalJsonPath = Join-Path $runRoot 'eval-summary.json'
        EvalTextPath = Join-Path $runRoot 'eval-summary.txt'
    }
}

function Write-SelectionCheckpointManifest {
    param(
        [string]$RepoRoot,
        [string]$WeightPath,
        [int]$Seed,
        [int]$GamesPerSeed,
        [int]$Depth,
        [double]$Alpha,
        [double]$Lambda,
        [double]$Epsilon,
        [int]$EpsilonPlies,
        [double]$TerminalReward,
        [double]$TdErrorClip,
        [int]$TerminalOnlyWarmup,
        [int]$NearTerminalCurriculum,
        [int]$CheckpointEvery,
        [int]$CheckpointGames
    )

    return Write-SurakartaWeightManifest -RepoRoot $RepoRoot `
        -WeightPath $WeightPath `
        -ArtifactKind 'checkpoint' `
        -Seed $Seed `
        -Games $GamesPerSeed `
        -Depth $Depth `
        -Alpha $Alpha `
        -Lambda $Lambda `
        -Epsilon $Epsilon `
        -EpsilonPlies $EpsilonPlies `
        -TerminalReward $TerminalReward `
        -TdErrorClip $TdErrorClip `
        -TerminalOnlyWarmup $TerminalOnlyWarmup `
        -NearTerminalCurriculum $NearTerminalCurriculum `
        -CheckpointEvery $CheckpointEvery `
        -Checkpoint ([ordered]@{
            is_checkpoint = $true
            name = [System.IO.Path]::GetFileName($WeightPath)
            games_completed = $CheckpointGames
        })
}

function Invoke-LoggedBenchmarkCommand {
    param(
        [string]$Name,
        [string[]]$Arguments,
        [string]$LogsDir
    )

    $stdoutLog = Join-Path $LogsDir "$Name.stdout.log"
    $stderrLog = Join-Path $LogsDir "$Name.stderr.log"

    $process = Start-Process -FilePath $benchmarkExe `
        -ArgumentList $Arguments `
        -WorkingDirectory (Join-Path $BuildDir 'bin') `
        -RedirectStandardOutput $stdoutLog `
        -RedirectStandardError $stderrLog `
        -NoNewWindow `
        -PassThru

    $process.WaitForExit()
    $process.Refresh()
    if ($process.ExitCode -ne 0) {
        throw "Command '$Name' failed with exit code $($process.ExitCode). See $stdoutLog and $stderrLog."
    }

    return [pscustomobject]@{
        StdOut = Get-Content -Path $stdoutLog -Raw
        StdErr = Get-Content -Path $stderrLog -Raw
        StdOutLog = $stdoutLog
        StdErrLog = $stderrLog
    }
}

function Get-FileSha256 {
    param([string]$Path)

    if (-not (Test-Path $Path)) {
        throw "Missing expected file: $Path"
    }

    return (Get-FileHash -Path $Path -Algorithm SHA256).Hash
}

function Get-Ratio {
    param(
        [double]$Numerator,
        [double]$Denominator
    )

    if ($Denominator -le 0.0) {
        if ($Numerator -le 0.0) {
            return 1.0
        }
        return [double]::PositiveInfinity
    }

    return $Numerator / $Denominator
}

function Join-SelectionReasons {
    param([object[]]$Reasons)

    $reasonList = @($Reasons | Where-Object { -not [string]::IsNullOrWhiteSpace([string]$_) })
    if ($reasonList.Count -eq 0) {
        return 'none'
    }

    return [string]::Join('; ', $reasonList)
}

function Get-SelectionAggregateRating {
    param([object[]]$Ratings)

    $ratingList = @($Ratings | Where-Object { -not [string]::IsNullOrWhiteSpace([string]$_) })
    if ($ratingList.Count -eq 0) {
        return 'watch'
    }
    if ($ratingList -contains 'regress') {
        return 'regress'
    }
    if ($ratingList -contains 'watch') {
        return 'watch'
    }

    return 'pass'
}

function Assert-JsonPayloadPresent {
    param(
        [string]$Path,
        [string]$Label
    )

    $raw = Get-Content -Path $Path -Raw
    $null = $raw | ConvertFrom-Json
    if (-not $raw.Trim()) {
        throw "$Label should not be empty."
    }
}

function Get-SelectionPositionMetrics {
    param([pscustomobject]$Entry)

    $candidate = $Entry.candidate
    $baseline = $Entry.baseline
    $repeatability = 'unknown'
    foreach ($repeatabilityProperty in @('repeatability', 'repeatability_evidence')) {
        if ($Entry.PSObject.Properties[$repeatabilityProperty] -and $null -ne $Entry.$repeatabilityProperty) {
            $repeatability = $Entry.$repeatabilityProperty
            break
        }
    }
    $searchRatio = Get-Ratio -Numerator ([double]$candidate.search_seconds) -Denominator ([double]$baseline.search_seconds)
    $nodeRatio = Get-Ratio -Numerator ([double]$candidate.nodes) -Denominator ([double]$baseline.nodes)
    $candidateQnodes = $null
    $baselineQnodes = $null
    $qnodesRatio = $null
    if ($candidate.PSObject.Properties['qnodes'] -and $baseline.PSObject.Properties['qnodes']) {
        $candidateQnodes = [uint64]$candidate.qnodes
        $baselineQnodes = [uint64]$baseline.qnodes
        $qnodesRatio = Get-Ratio -Numerator ([double]$candidateQnodes) -Denominator ([double]$baselineQnodes)
    }
    $candidateAspirationRetries = $null
    $baselineAspirationRetries = $null
    $aspirationRetriesDelta = $null
    if ($candidate.PSObject.Properties['aspiration_retries'] -and $baseline.PSObject.Properties['aspiration_retries']) {
        $candidateAspirationRetries = [int]$candidate.aspiration_retries
        $baselineAspirationRetries = [int]$baseline.aspiration_retries
        $aspirationRetriesDelta = $candidateAspirationRetries - $baselineAspirationRetries
    }
    $tinyPositionNodeFloor = 500
    $isTinyPosition = [Math]::Max([uint64]$candidate.nodes, [uint64]$baseline.nodes) -lt $tinyPositionNodeFloor
    $treeRatio = [double]$nodeRatio
    if ($null -ne $qnodesRatio) {
        $treeRatio = [Math]::Max($treeRatio, [double]$qnodesRatio)
    }
    $performanceRatio = if ($isTinyPosition) {
        $treeRatio
    } else {
        [Math]::Max($searchRatio, $treeRatio)
    }
    $performanceBasis = if ($isTinyPosition) {
        if ($null -ne $qnodesRatio) { 'max_node_or_qnodes_ratio_tiny_position' } else { 'node_ratio_tiny_position' }
    } else {
        if ($null -ne $qnodesRatio) { 'max_search_node_or_qnodes_ratio' } else { 'max_search_or_node_ratio' }
    }

    return [pscustomobject]@{
        case_id = $Entry.case_id
        candidate_best_move = [string]$candidate.best_move
        baseline_best_move = [string]$baseline.best_move
        best_move_changed = [string]$candidate.best_move -ne [string]$baseline.best_move
        candidate_score = [int]$candidate.score
        baseline_score = [int]$baseline.score
        score_delta = [int]$candidate.score - [int]$baseline.score
        candidate_search_seconds = [double]$candidate.search_seconds
        baseline_search_seconds = [double]$baseline.search_seconds
        search_ratio = [Math]::Round($searchRatio, 6)
        candidate_nodes = [uint64]$candidate.nodes
        baseline_nodes = [uint64]$baseline.nodes
        node_ratio = [Math]::Round($nodeRatio, 6)
        candidate_qnodes = $candidateQnodes
        baseline_qnodes = $baselineQnodes
        qnodes_ratio = if ($null -ne $qnodesRatio) { [Math]::Round($qnodesRatio, 6) } else { $null }
        candidate_aspiration_retries = $candidateAspirationRetries
        baseline_aspiration_retries = $baselineAspirationRetries
        aspiration_retries_delta = $aspirationRetriesDelta
        performance_ratio = [Math]::Round($performanceRatio, 6)
        performance_basis = $performanceBasis
        repeatability = $repeatability
    }
}

function Get-SelectionStrengthRating {
    param(
        [int]$CandidateWins,
        [int]$BaselineWins,
        [int]$ScoreRegressionCount,
        [int]$BestMoveChangeCount
    )

    $rating = 'watch'
    $reasons = @()

    if ($CandidateWins -lt $BaselineWins) {
        $rating = 'regress'
        $reasons += "candidate wins $CandidateWins < baseline wins $BaselineWins"
    }
    if ($ScoreRegressionCount -gt 0) {
        $rating = 'regress'
        $reasons += "$ScoreRegressionCount fixed position score regression(s)"
    }

    if ($rating -ne 'regress') {
        if ($CandidateWins -gt $BaselineWins -and $BestMoveChangeCount -eq 0) {
            $rating = 'pass'
        } else {
            $rating = 'watch'
            if ($CandidateWins -eq $BaselineWins) {
                $reasons += 'candidate wins are tied with baseline wins'
            } elseif ($CandidateWins -gt $BaselineWins) {
                $reasons += 'candidate wins lead but fixed position best move changed'
            }
            if ($BestMoveChangeCount -gt 0) {
                $reasons += "$BestMoveChangeCount fixed position best move change(s)"
            }
        }
    }

    return [pscustomobject]@{
        rating = $rating
        reasons = $reasons
    }
}

function Get-SelectionPerfRating {
    param([double]$MaxPerformanceRatio)

    $rating = 'watch'
    $reasons = @()

    if ($MaxPerformanceRatio -gt 1.25) {
        $rating = 'regress'
        $reasons += ("performance ratio {0:N3} exceeds 1.25" -f $MaxPerformanceRatio)
    } elseif ($MaxPerformanceRatio -le 1.0) {
        $rating = 'pass'
    } else {
        $rating = 'watch'
        $reasons += ("performance ratio {0:N3} is above parity" -f $MaxPerformanceRatio)
    }

    return [pscustomobject]@{
        rating = $rating
        reasons = $reasons
    }
}

function Get-SelectionRepeatabilityEvidence {
    param(
        [int]$Seed,
        [string]$Position,
        [string]$MetricName,
        [double]$Ratio
    )

    $knownEvidence = @(
        [pscustomobject]@{
            seed = 20260423
            position = 'opening'
            metric = 'search_seconds_ratio'
            ratio = 1.469059
            status = 'timing_sensitive'
            runs = 10
            exceeded_count = 2
            notes = 'phase3 repeatability evidence: 10 runs; nodes/qnodes stable below 1.25; time ratio exceeded 2/10.'
        },
        [pscustomobject]@{
            seed = 20260425
            position = 'opening'
            metric = 'search_seconds_ratio'
            ratio = 2.159236
            status = 'stable_tree_inflation_with_timing_noise'
            runs = 10
            exceeded_count = 7
            notes = 'phase3 repeatability evidence: 10 runs; stable tree/qtree inflation; time ratio exceeded 7/10.'
        },
        [pscustomobject]@{
            seed = 20260425
            position = 'opening'
            metric = 'node_ratio'
            ratio = 1.295516
            status = 'stable_tree_inflation'
            runs = 10
            exceeded_count = 10
            notes = 'phase3 repeatability evidence: 10 runs; nodes and qnodes stable above 1.25.'
        }
    )

    $roundedRatio = [Math]::Round($Ratio, 6)
    foreach ($evidence in $knownEvidence) {
        if ($Seed -eq [int]$evidence.seed -and
            $Position -eq [string]$evidence.position -and
            $MetricName -eq [string]$evidence.metric -and
            [Math]::Abs($roundedRatio - [double]$evidence.ratio) -le 0.000001) {
            return [pscustomobject]@{
                status = $evidence.status
                runs = $evidence.runs
                exceeded_count = $evidence.exceeded_count
                notes = $evidence.notes
            }
        }
    }

    return [pscustomobject]@{
        status = 'unknown'
        runs = $null
        exceeded_count = $null
        notes = $null
    }
}

function New-SelectionPerfBlockerDetail {
    param(
        [int]$Seed,
        [pscustomobject]$Metric,
        [string]$MetricName,
        [object]$BaselineValue,
        [object]$CandidateValue,
        [double]$Ratio,
        [double]$Threshold
    )

    $repeatabilityEvidence = Get-SelectionRepeatabilityEvidence -Seed $Seed `
        -Position $Metric.case_id `
        -MetricName $MetricName `
        -Ratio $Ratio

    return [pscustomobject]@{
        seed = $Seed
        position = $Metric.case_id
        category = $Metric.case_id
        metric = $MetricName
        baseline_value = $BaselineValue
        candidate_value = $CandidateValue
        ratio = [Math]::Round($Ratio, 6)
        threshold = [Math]::Round($Threshold, 6)
        trigger = 'exceeds_threshold'
        repeatability = $repeatabilityEvidence.status
        repeatability_status = $repeatabilityEvidence.status
        repeatability_runs = $repeatabilityEvidence.runs
        repeatability_exceeded_count = $repeatabilityEvidence.exceeded_count
        repeatability_notes = $repeatabilityEvidence.notes
    }
}

function Get-SelectionPerfBlockerDetails {
    param(
        [int]$Seed,
        [object[]]$PositionMetrics,
        [double]$Threshold
    )

    $details = @()
    foreach ($metric in $PositionMetrics) {
        if ([double]$metric.performance_ratio -le $Threshold) {
            continue
        }

        if ([string]$metric.performance_basis -notlike '*tiny_position' -and [double]$metric.search_ratio -gt $Threshold) {
            $details += New-SelectionPerfBlockerDetail -Seed $Seed `
                -Metric $metric `
                -MetricName 'search_seconds_ratio' `
                -BaselineValue $metric.baseline_search_seconds `
                -CandidateValue $metric.candidate_search_seconds `
                -Ratio ([double]$metric.search_ratio) `
                -Threshold $Threshold
        }

        if ([double]$metric.node_ratio -gt $Threshold) {
            $details += New-SelectionPerfBlockerDetail -Seed $Seed `
                -Metric $metric `
                -MetricName 'node_ratio' `
                -BaselineValue $metric.baseline_nodes `
                -CandidateValue $metric.candidate_nodes `
                -Ratio ([double]$metric.node_ratio) `
                -Threshold $Threshold
        }

        if ($null -ne $metric.qnodes_ratio -and [double]$metric.qnodes_ratio -gt $Threshold) {
            $details += New-SelectionPerfBlockerDetail -Seed $Seed `
                -Metric $metric `
                -MetricName 'qnodes_ratio' `
                -BaselineValue $metric.baseline_qnodes `
                -CandidateValue $metric.candidate_qnodes `
                -Ratio ([double]$metric.qnodes_ratio) `
                -Threshold $Threshold
        }
    }

    return @($details)
}

function Get-SelectionScorecard {
    param(
        [pscustomobject]$EvalReport,
        [int]$Seed
    )

    $positionMetrics = @()
    foreach ($entry in $EvalReport.position_reports) {
        $positionMetrics += Get-SelectionPositionMetrics -Entry $entry
    }

    $candidateWins = [int]$EvalReport.match_summary.candidate_wins
    $baselineWins = [int]$EvalReport.match_summary.baseline_wins
    $draws = [int]$EvalReport.match_summary.draws
    $scoreRegressionCount = @($positionMetrics | Where-Object { $_.score_delta -lt 0 }).Count
    $bestMoveChangeCount = @($positionMetrics | Where-Object { $_.best_move_changed }).Count
    $performanceRatios = @($positionMetrics | ForEach-Object { [double]$_.performance_ratio })
    $averagePerformanceRatio = if ($performanceRatios.Count -gt 0) {
        ($performanceRatios | Measure-Object -Average).Average
    } else {
        0.0
    }
    $maxPerformanceRatio = if ($performanceRatios.Count -gt 0) {
        ($performanceRatios | Measure-Object -Maximum).Maximum
    } else {
        0.0
    }

    $performanceThreshold = 1.25
    $blockerDetails = Get-SelectionPerfBlockerDetails -Seed $Seed `
        -PositionMetrics $positionMetrics `
        -Threshold $performanceThreshold
    $strengthDiagnostic = Get-SelectionStrengthRating -CandidateWins $candidateWins `
        -BaselineWins $baselineWins `
        -ScoreRegressionCount $scoreRegressionCount `
        -BestMoveChangeCount $bestMoveChangeCount
    $perfDiagnostic = Get-SelectionPerfRating -MaxPerformanceRatio $maxPerformanceRatio
    $rating = 'watch'
    $reasons = @()

    if ($candidateWins -lt $baselineWins) {
        $rating = 'regress'
        $reasons += "candidate wins $candidateWins < baseline wins $baselineWins"
    }
    if ($scoreRegressionCount -gt 0) {
        $rating = 'regress'
        $reasons += "$scoreRegressionCount fixed position score regression(s)"
    }
    if ($maxPerformanceRatio -gt $performanceThreshold) {
        $rating = 'regress'
        $reasons += ("performance ratio {0:N3} exceeds 1.25" -f $maxPerformanceRatio)
    }

    if ($rating -ne 'regress') {
        if ($candidateWins -gt $baselineWins -and $bestMoveChangeCount -eq 0 -and $maxPerformanceRatio -le 1.0) {
            $rating = 'pass'
        } else {
            $rating = 'watch'
            if ($candidateWins -eq $baselineWins) {
                $reasons += 'candidate wins are tied with baseline wins'
            } elseif ($candidateWins -gt $baselineWins) {
                $reasons += 'candidate wins lead but observation signals changed'
            }
            if ($bestMoveChangeCount -gt 0) {
                $reasons += "$bestMoveChangeCount fixed position best move change(s)"
            }
            if ($maxPerformanceRatio -gt 1.0) {
                $reasons += ("performance ratio {0:N3} is above parity" -f $maxPerformanceRatio)
            }
        }
    }

    return [pscustomobject]@{
        rating = $rating
        reasons = $reasons
        candidate_wins = $candidateWins
        baseline_wins = $baselineWins
        draws = $draws
        score_regressions = $scoreRegressionCount
        best_move_changes = $bestMoveChangeCount
        average_performance_ratio = [Math]::Round($averagePerformanceRatio, 6)
        max_performance_ratio = [Math]::Round($maxPerformanceRatio, 6)
        strength_rating = $strengthDiagnostic.rating
        perf_rating = $perfDiagnostic.rating
        blocker_reason = Join-SelectionReasons -Reasons $reasons
        blocker_details = @($blockerDetails)
        strength_reasons = $strengthDiagnostic.reasons
        perf_reasons = $perfDiagnostic.reasons
        position_metrics = $positionMetrics
    }
}

function Invoke-SelectionEvaluation {
    param(
        [pscustomobject]$Layout,
        [string]$CandidateWeights,
        [string]$BaselineWeights,
        [int]$Depth,
        [string]$JsonPath,
        [string]$TextPath,
        [string]$LogPrefix
    )

    $evalJson = Invoke-LoggedBenchmarkCommand -Name "$LogPrefix-eval-json" -LogsDir $Layout.LogsDir -Arguments @(
        'bitboard-eval',
        '--candidate', $CandidateWeights,
        '--baseline', $BaselineWeights,
        '--depth', "$Depth",
        '--format', 'json'
    )
    $evalJson.StdOut | Set-Content -Path $JsonPath

    $evalText = Invoke-LoggedBenchmarkCommand -Name "$LogPrefix-eval-text" -LogsDir $Layout.LogsDir -Arguments @(
        'bitboard-eval',
        '--candidate', $CandidateWeights,
        '--baseline', $BaselineWeights,
        '--depth', "$Depth",
        '--format', 'text'
    )
    $evalText.StdOut | Set-Content -Path $TextPath

    Assert-JsonPayloadPresent -Path $JsonPath -Label "$($Layout.Name) $LogPrefix eval-summary.json"
    if (-not (Test-Path $TextPath)) {
        throw "Missing eval text report: $TextPath"
    }

    $evalReport = $evalJson.StdOut | ConvertFrom-Json
    return [pscustomobject]@{
        JsonPath = $JsonPath
        TextPath = $TextPath
        Report = $evalReport
        Scorecard = Get-SelectionScorecard -EvalReport $evalReport -Seed $Layout.Seed
    }
}

function Run-SelectionSeed {
    param(
        [string]$RepoRoot,
        [pscustomobject]$Layout,
        [int]$GamesPerSeed,
        [int]$Depth,
        [int]$CheckpointEvery,
        [double]$Alpha,
        [double]$Lambda,
        [double]$Epsilon,
        [int]$EpsilonPlies,
        [double]$TerminalReward,
        [double]$TdErrorClip,
        [int]$TerminalOnlyWarmup,
        [int]$NearTerminalCurriculum
    )

    $null = Invoke-LoggedBenchmarkCommand -Name 'bootstrap-export' -LogsDir $Layout.LogsDir -Arguments @(
        'bitboard-train',
        '--output', $Layout.BootstrapWeights,
        '--games', '0',
        '--depth', "$Depth",
        '--seed', "$($Layout.Seed)",
        '--alpha', "$Alpha",
        '--lambda', "$Lambda",
        '--epsilon', "$Epsilon",
        '--epsilon-plies', "$EpsilonPlies",
        '--terminal-reward', "$TerminalReward",
        '--td-error-clip', "$TdErrorClip",
        '--terminal-only-warmup', "$TerminalOnlyWarmup",
        '--near-terminal-curriculum', "$NearTerminalCurriculum",
        '--format', 'json'
    )

    $trainResult = Invoke-LoggedBenchmarkCommand -Name 'candidate-train' -LogsDir $Layout.LogsDir -Arguments @(
        'bitboard-train',
        '--output', $Layout.CandidateWeights,
        '--weights', $Layout.BootstrapWeights,
        '--games', "$GamesPerSeed",
        '--depth', "$Depth",
        '--seed', "$($Layout.Seed)",
        '--alpha', "$Alpha",
        '--lambda', "$Lambda",
        '--epsilon', "$Epsilon",
        '--epsilon-plies', "$EpsilonPlies",
        '--terminal-reward', "$TerminalReward",
        '--td-error-clip', "$TdErrorClip",
        '--terminal-only-warmup', "$TerminalOnlyWarmup",
        '--near-terminal-curriculum', "$NearTerminalCurriculum",
        '--format', 'json',
        '--checkpoint-every', "$CheckpointEvery",
        '--checkpoint-dir', $Layout.CheckpointsDir
    )
    $trainSummary = $trainResult.StdOut | ConvertFrom-Json

    $candidateEvaluation = Invoke-SelectionEvaluation -Layout $Layout `
        -CandidateWeights $Layout.CandidateWeights `
        -BaselineWeights $Layout.BootstrapWeights `
        -Depth $Depth `
        -JsonPath $Layout.EvalJsonPath `
        -TextPath $Layout.EvalTextPath `
        -LogPrefix 'candidate'

    $bootstrapHash = Get-FileSha256 -Path $Layout.BootstrapWeights
    $candidateHash = Get-FileSha256 -Path $Layout.CandidateWeights
    if ($bootstrapHash -eq $candidateHash) {
        throw "Selection requires candidate weights to differ from bootstrap weights in $($Layout.RunRoot)."
    }

    $artifacts = @()
    $artifacts += Write-SurakartaWeightManifest -RepoRoot $RepoRoot `
        -WeightPath $Layout.BootstrapWeights `
        -ArtifactKind 'bootstrap' `
        -Seed $Layout.Seed `
        -Games 0 `
        -Depth $Depth `
        -Alpha $Alpha `
        -Lambda $Lambda `
        -Epsilon $Epsilon `
        -EpsilonPlies $EpsilonPlies `
        -TerminalReward $TerminalReward `
        -TdErrorClip $TdErrorClip `
        -TerminalOnlyWarmup $TerminalOnlyWarmup `
        -NearTerminalCurriculum $NearTerminalCurriculum `
        -CheckpointEvery $CheckpointEvery `
        -Checkpoint ([ordered]@{
            is_checkpoint = $false
            name = $null
            games_completed = 0
        })
    $artifacts += Write-SurakartaWeightManifest -RepoRoot $RepoRoot `
        -WeightPath $Layout.CandidateWeights `
        -ArtifactKind 'candidate' `
        -Seed $Layout.Seed `
        -Games $GamesPerSeed `
        -Depth $Depth `
        -Alpha $Alpha `
        -Lambda $Lambda `
        -Epsilon $Epsilon `
        -EpsilonPlies $EpsilonPlies `
        -TerminalReward $TerminalReward `
        -TdErrorClip $TdErrorClip `
        -TerminalOnlyWarmup $TerminalOnlyWarmup `
        -NearTerminalCurriculum $NearTerminalCurriculum `
        -CheckpointEvery $CheckpointEvery `
        -Checkpoint ([ordered]@{
            is_checkpoint = $false
            name = $null
            games_completed = $trainSummary.games_completed
        })
    $checkpointResults = @()
    foreach ($checkpointFile in @(Get-ChildItem -Path $Layout.CheckpointsDir -Filter '*.bin' -File | Sort-Object Name)) {
        $checkpointGames = [int]([regex]::Match($checkpointFile.BaseName, 'checkpoint-(\d+)').Groups[1].Value)
        $checkpointManifest = Write-SelectionCheckpointManifest -RepoRoot $RepoRoot `
            -WeightPath $checkpointFile.FullName `
            -Seed $Layout.Seed `
            -GamesPerSeed $GamesPerSeed `
            -Depth $Depth `
            -Alpha $Alpha `
            -Lambda $Lambda `
            -Epsilon $Epsilon `
            -EpsilonPlies $EpsilonPlies `
            -TerminalReward $TerminalReward `
            -TdErrorClip $TdErrorClip `
            -TerminalOnlyWarmup $TerminalOnlyWarmup `
            -NearTerminalCurriculum $NearTerminalCurriculum `
            -CheckpointEvery $CheckpointEvery `
            -CheckpointGames $checkpointGames
        $artifacts += $checkpointManifest

        $checkpointEvalJsonPath = Join-Path $Layout.CheckpointEvalDir ("{0}.eval-summary.json" -f $checkpointFile.BaseName)
        $checkpointEvalTextPath = Join-Path $Layout.CheckpointEvalDir ("{0}.eval-summary.txt" -f $checkpointFile.BaseName)
        $checkpointEvaluation = Invoke-SelectionEvaluation -Layout $Layout `
            -CandidateWeights $checkpointFile.FullName `
            -BaselineWeights $Layout.BootstrapWeights `
            -Depth $Depth `
            -JsonPath $checkpointEvalJsonPath `
            -TextPath $checkpointEvalTextPath `
            -LogPrefix $checkpointFile.BaseName

        $matchingTrainingSummary = $null
        if ($trainSummary.PSObject.Properties['checkpoint_summaries']) {
            $matchingTrainingSummary = $trainSummary.checkpoint_summaries | Where-Object {
                $_.games_completed -eq $checkpointGames -or
                [System.IO.Path]::GetFileName([string]$_.path) -eq $checkpointFile.Name
            } | Select-Object -First 1
        }
        if ($null -eq $matchingTrainingSummary) {
            $matchingTrainingSummary = [pscustomobject]@{
                games_completed = $checkpointGames
                path = $checkpointFile.FullName
            }
        }

        $checkpointResults += [pscustomobject]@{
            name = $checkpointFile.Name
            path = $checkpointFile.FullName
            games_completed = $checkpointGames
            manifest = $checkpointManifest.Manifest
            training_summary = $matchingTrainingSummary
            evaluation = $checkpointEvaluation.Report
            scorecard = $checkpointEvaluation.Scorecard
            rating = $checkpointEvaluation.Scorecard.rating
            eval_json_path = $checkpointEvaluation.JsonPath
            eval_text_path = $checkpointEvaluation.TextPath
        }
    }
    $sessionManifestPath = Join-Path $Layout.RunRoot 'weights-manifest.json'
    $sessionManifest = Write-SurakartaSessionWeightManifest -RepoRoot $RepoRoot -Path $sessionManifestPath -Purpose 'selection' -Artifacts $artifacts

    return [pscustomobject]@{
        seed = $Layout.Seed
        name = $Layout.Name
        run_root = $Layout.RunRoot
        bootstrap_hash = $bootstrapHash
        candidate_hash = $candidateHash
        training_summary = $trainSummary
        evaluation = $candidateEvaluation.Report
        scorecard = $candidateEvaluation.Scorecard
        checkpoints = $checkpointResults
        manifest = $sessionManifest
        artifacts = $artifacts
    }
}

Invoke-SurakartaConfigure -Configuration 'Release' -BuildDir $BuildDir
Invoke-SurakartaBuild -BuildDir $BuildDir -Targets @('surakarta-benchmark')

New-Item -ItemType Directory -Force -Path $sessionRoot, $selectionRoot | Out-Null
Stop-SurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot | Out-Null

$seedRuns = @()
foreach ($seed in $Seeds) {
    $layout = New-SelectionRunLayout -Root $selectionRoot -Seed $seed
    $seedRuns += Run-SelectionSeed -RepoRoot $repoRoot -Layout $layout `
        -GamesPerSeed $GamesPerSeed `
        -Depth $Depth `
        -CheckpointEvery $CheckpointEvery `
        -Alpha $Alpha `
        -Lambda $Lambda `
        -Epsilon $Epsilon `
        -EpsilonPlies $EpsilonPlies `
        -TerminalReward $TerminalReward `
        -TdErrorClip $TdErrorClip `
        -TerminalOnlyWarmup $TerminalOnlyWarmup `
        -NearTerminalCurriculum $NearTerminalCurriculum
}

$passCount = 0
$watchCount = 0
$regressCount = 0
$candidateWinsTotal = 0
$baselineWinsTotal = 0
$drawsTotal = 0
$scoreRegressionTotal = 0
$bestMoveChangeTotal = 0
$performanceRatios = @()

foreach ($run in $seedRuns) {
    switch ($run.scorecard.rating) {
        'pass' { ++$passCount }
        'watch' { ++$watchCount }
        'regress' { ++$regressCount }
    }

    $candidateWinsTotal += [int]$run.scorecard.candidate_wins
    $baselineWinsTotal += [int]$run.scorecard.baseline_wins
    $drawsTotal += [int]$run.scorecard.draws
    $scoreRegressionTotal += [int]$run.scorecard.score_regressions
    $bestMoveChangeTotal += [int]$run.scorecard.best_move_changes
    $performanceRatios += [double]$run.scorecard.max_performance_ratio
}

$overallPerformanceRatio = if ($performanceRatios.Count -gt 0) {
    ($performanceRatios | Measure-Object -Average).Average
} else {
    0.0
}
$worstPerformanceRatio = if ($performanceRatios.Count -gt 0) {
    ($performanceRatios | Measure-Object -Maximum).Maximum
} else {
    0.0
}

$overallRating = if ($regressCount -gt 0) {
    'regress'
} elseif ($watchCount -eq 0) {
    'pass'
} else {
    'watch'
}

$strengthRating = Get-SelectionAggregateRating -Ratings @($seedRuns | ForEach-Object { $_.scorecard.strength_rating })
$perfRating = Get-SelectionAggregateRating -Ratings @($seedRuns | ForEach-Object { $_.scorecard.perf_rating })
$blockerReasons = @(
    foreach ($run in $seedRuns) {
        if ($run.scorecard.rating -ne 'pass') {
            $reason = [string]$run.scorecard.blocker_reason
            if ([string]::IsNullOrWhiteSpace($reason) -or $reason -eq 'none') {
                $reason = "rating $($run.scorecard.rating)"
            }
            "seed $($run.seed): $reason"
        }
    }
)
$blockerReason = Join-SelectionReasons -Reasons $blockerReasons
$blockerDetails = @(
    foreach ($run in $seedRuns) {
        @($run.scorecard.blocker_details)
    }
)

$summary = [ordered]@{
    seeds = @($Seeds)
    games_per_seed = $GamesPerSeed
    depth = $Depth
    checkpoint_every = $CheckpointEvery
    alpha = $Alpha
    lambda = $Lambda
    epsilon = $Epsilon
    epsilon_plies = $EpsilonPlies
    terminal_reward = $TerminalReward
    td_error_clip = $TdErrorClip
    terminal_only_warmup = $TerminalOnlyWarmup
    near_terminal_curriculum = $NearTerminalCurriculum
    overall_rating = $overallRating
    strength_rating = $strengthRating
    perf_rating = $perfRating
    blocker_reason = $blockerReason
    blocker_details = @($blockerDetails)
    scorecard = [ordered]@{
        pass = $passCount
        watch = $watchCount
        regress = $regressCount
    }
    totals = [ordered]@{
        candidate_wins = $candidateWinsTotal
        baseline_wins = $baselineWinsTotal
        draws = $drawsTotal
        score_regressions = $scoreRegressionTotal
        best_move_changes = $bestMoveChangeTotal
        average_performance_ratio = [Math]::Round($overallPerformanceRatio, 6)
        worst_performance_ratio = [Math]::Round($worstPerformanceRatio, 6)
    }
    per_seed = @(
        foreach ($run in $seedRuns) {
            [ordered]@{
                seed = $run.seed
                name = $run.name
                run_root = $run.run_root
                bootstrap_hash = $run.bootstrap_hash
                candidate_hash = $run.candidate_hash
                training_summary = $run.training_summary
                evaluation = $run.evaluation
                scorecard = $run.scorecard
                rating = $run.scorecard.rating
                strength_rating = $run.scorecard.strength_rating
                perf_rating = $run.scorecard.perf_rating
                blocker_reason = $run.scorecard.blocker_reason
                blocker_details = @($run.scorecard.blocker_details)
                checkpoints = @($run.checkpoints)
                manifest = $run.manifest
            }
        }
    )
}

$selectionArtifacts = @()
foreach ($run in $seedRuns) {
    $selectionArtifacts += @($run.artifacts)
}
$selectionSessionManifestPath = Join-Path $sessionRoot 'weights-manifest.json'
$selectionSessionManifest = Write-SurakartaSessionWeightManifest -RepoRoot $repoRoot -Path $selectionSessionManifestPath -Purpose 'selection' -Artifacts $selectionArtifacts
$summary.manifest = $selectionSessionManifest

$summary | ConvertTo-Json -Depth 16 | Set-Content -Path $summaryJsonPath

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add('Selection summary')
$lines.Add("Output: $sessionRoot")
$lines.Add("Seeds: $([string]::Join(', ', $Seeds))")
$lines.Add("Games per seed: $GamesPerSeed")
$lines.Add("Depth: $Depth")
$lines.Add("Checkpoint every: $CheckpointEvery")
$lines.Add("Alpha: $Alpha")
$lines.Add("Lambda: $Lambda")
$lines.Add("Epsilon: $Epsilon")
$lines.Add("Epsilon plies: $EpsilonPlies")
$lines.Add("Terminal reward: $TerminalReward")
$lines.Add("TD error clip: $TdErrorClip")
$lines.Add("Terminal-only warmup: $TerminalOnlyWarmup")
$lines.Add("Near-terminal curriculum: $NearTerminalCurriculum")
$lines.Add("overall_rating: $overallRating")
$lines.Add("strength_rating: $strengthRating")
$lines.Add("perf_rating: $perfRating")
$lines.Add("blocker_reason: $blockerReason")
$lines.Add("scorecard_pass: $passCount")
$lines.Add("scorecard_watch: $watchCount")
$lines.Add("scorecard_regress: $regressCount")
$lines.Add("candidate_wins: $candidateWinsTotal")
$lines.Add("baseline_wins: $baselineWinsTotal")
$lines.Add("draws: $drawsTotal")
$lines.Add(("average_performance_ratio: {0:N6}" -f $overallPerformanceRatio))
$lines.Add(("worst_performance_ratio: {0:N6}" -f $worstPerformanceRatio))
$lines.Add('')
$lines.Add('Perf blocker details')
$lines.Add('seed | category | metric | ratio | threshold | repeatability | verdict')
if ($blockerDetails.Count -eq 0) {
    $lines.Add('(none)')
} else {
    foreach ($detail in $blockerDetails) {
        $repeatabilityStatus = 'unknown'
        if ($detail.PSObject.Properties['repeatability_status'] -and -not [string]::IsNullOrWhiteSpace([string]$detail.repeatability_status)) {
            $repeatabilityStatus = [string]$detail.repeatability_status
        } elseif ($detail.PSObject.Properties['repeatability'] -and -not [string]::IsNullOrWhiteSpace([string]$detail.repeatability)) {
            $repeatabilityStatus = [string]$detail.repeatability
        }

        $lines.Add(("{0} | {1} | {2} | {3:N6} | {4:N6} | {5} | regress" -f `
            $detail.seed,
            $detail.category,
            $detail.metric,
            [double]$detail.ratio,
            [double]$detail.threshold,
            $repeatabilityStatus))
    }
}
$lines.Add('')

foreach ($run in $seedRuns) {
    $lines.Add(("seed: {0} rating: {1} strength_rating: {2} perf_rating: {3} candidate_wins: {4} baseline_wins: {5} draws: {6} score_regressions: {7} best_move_changes: {8} max_performance_ratio: {9:N6}" -f `
        $run.seed,
        $run.scorecard.rating,
        $run.scorecard.strength_rating,
        $run.scorecard.perf_rating,
        $run.scorecard.candidate_wins,
        $run.scorecard.baseline_wins,
        $run.scorecard.draws,
        $run.scorecard.score_regressions,
        $run.scorecard.best_move_changes,
        $run.scorecard.max_performance_ratio))
    $lines.Add(("  blocker_reason: {0}" -f $run.scorecard.blocker_reason))
    if ($run.scorecard.reasons.Count -gt 0) {
        $lines.Add(("  reasons: {0}" -f ([string]::Join('; ', $run.scorecard.reasons))))
    }
    foreach ($checkpoint in $run.checkpoints) {
        $lines.Add(("  checkpoint: {0} games_completed={1} rating={2} eval_json={3}" -f `
            $checkpoint.name,
            $checkpoint.games_completed,
            $checkpoint.rating,
            $checkpoint.eval_json_path))
    }
    foreach ($metric in $run.scorecard.position_metrics) {
        $lines.Add(("  {0}: candidate_best_move={1} baseline_best_move={2} score_delta={3} search_seconds={4:N6}/{5:N6} nodes={6}/{7} search_ratio={8:N6} node_ratio={9:N6} performance_ratio={10:N6} performance_basis={11}" -f `
            $metric.case_id,
            $metric.candidate_best_move,
            $metric.baseline_best_move,
            $metric.score_delta,
            $metric.candidate_search_seconds,
            $metric.baseline_search_seconds,
            $metric.candidate_nodes,
            $metric.baseline_nodes,
            $metric.search_ratio,
            $metric.node_ratio,
            $metric.performance_ratio,
            $metric.performance_basis))
    }
    $lines.Add('')
}

$lines | Set-Content -Path $summaryTextPath

Assert-NoSurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot

Write-Host "[PASS] benchmark selection scorecard"
Write-Host "Session root: $sessionRoot"
Write-Host "Selection summary JSON: $summaryJsonPath"
Write-Host "Selection summary TXT: $summaryTextPath"
