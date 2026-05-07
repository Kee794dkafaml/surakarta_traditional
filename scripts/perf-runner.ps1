param(
    [string]$BuildDir = (Join-Path (Resolve-Path (Join-Path $PSScriptRoot '..')).Path 'build-vs-release'),
    [string]$BaselinePath,
    [int]$TimeoutSeconds = 90,
    [int]$MovegenIterations = 20000
)

. (Join-Path $PSScriptRoot 'common.ps1')

$repoRoot = Get-SurakartaRepoRoot
if (-not $BaselinePath) {
    $BaselinePath = Join-Path $repoRoot 'test\test_data\perf-baseline.json'
}
$BaselinePath = [System.IO.Path]::GetFullPath($BaselinePath)

$benchmarkExe = Join-Path $BuildDir 'bin\surakarta-benchmark.exe'
$outputRoot = Join-Path $repoRoot 'build-local\perf'
$timestamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$runRoot = Join-Path $outputRoot $timestamp
$logsDir = Join-Path $runRoot 'logs'

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

function Get-PerfBaselineRecords {
    param([string]$Path)

    if (-not (Test-Path $Path)) {
        throw "Missing perf baseline: $Path"
    }

    $payload = Get-Content -Path $Path -Raw | ConvertFrom-Json
    if ($payload.PSObject.Properties['results']) {
        return @($payload.results)
    }
    if ($payload -is [array]) {
        return @($payload)
    }

    throw "Perf baseline must be a JSON array or an object with a results array: $Path"
}

function Get-PerfKey {
    param(
        [string]$CaseId,
        [int]$Depth,
        [int]$Threads
    )

    return '{0}|{1}|{2}' -f $CaseId, $Depth, $Threads
}

function New-PerfBaselineMap {
    param([object[]]$Records)

    $map = @{}
    foreach ($record in $Records) {
        $key = Get-PerfKey -CaseId ([string]$record.case_id) -Depth ([int]$record.depth) -Threads ([int]$record.threads)
        if ($map.ContainsKey($key)) {
            throw "Duplicate perf baseline entry: $key"
        }
        $map[$key] = $record
    }
    return $map
}

function Compare-PerfEntry {
    param(
        [pscustomobject]$Current,
        [pscustomobject]$Baseline
    )

    $searchRatio = Get-Ratio -Numerator ([double]$Current.search_seconds) -Denominator ([double]$Baseline.search_seconds)
    $nodeRatio = Get-Ratio -Numerator ([double]$Current.nodes) -Denominator ([double]$Baseline.nodes)
    $tinyPositionNodeFloor = 500
    $isTinyPosition = [Math]::Max([uint64]$Current.nodes, [uint64]$Baseline.nodes) -lt $tinyPositionNodeFloor
    $performanceRatio = if ($isTinyPosition) {
        $nodeRatio
    } else {
        [Math]::Max($searchRatio, $nodeRatio)
    }
    $performanceBasis = if ($isTinyPosition) {
        'node_ratio_tiny_position'
    } else {
        'max_search_or_node_ratio'
    }

    $failures = @()
    if ([int]$Current.threads -eq 1 -and [string]$Current.best_move -ne [string]$Baseline.best_move) {
        $failures += [ordered]@{
            reason = 'best_move_drift'
            case_id = $Current.case_id
            depth = $Current.depth
            threads = $Current.threads
            current = $Current.best_move
            baseline = $Baseline.best_move
        }
    }
    if ([int]$Current.threads -eq 1 -and [int]$Current.score -ne [int]$Baseline.score) {
        $failures += [ordered]@{
            reason = 'score_drift'
            case_id = $Current.case_id
            depth = $Current.depth
            threads = $Current.threads
            current = [int]$Current.score
            baseline = [int]$Baseline.score
        }
    }
    if ($performanceRatio -gt 1.25) {
        $failures += [ordered]@{
            reason = 'performance_regression'
            case_id = $Current.case_id
            depth = $Current.depth
            threads = $Current.threads
            performance_ratio = [Math]::Round($performanceRatio, 6)
            performance_basis = $performanceBasis
        }
    }

    return [pscustomobject]@{
        case_id = $Current.case_id
        depth = [int]$Current.depth
        threads = [int]$Current.threads
        current_search_seconds = [double]$Current.search_seconds
        baseline_search_seconds = [double]$Baseline.search_seconds
        search_ratio = [Math]::Round($searchRatio, 6)
        current_nodes = [uint64]$Current.nodes
        baseline_nodes = [uint64]$Baseline.nodes
        node_ratio = [Math]::Round($nodeRatio, 6)
        performance_ratio = [Math]::Round($performanceRatio, 6)
        performance_basis = $performanceBasis
        current_best_move = [string]$Current.best_move
        baseline_best_move = [string]$Baseline.best_move
        current_score = [int]$Current.score
        baseline_score = [int]$Baseline.score
        failures = $failures
    }
}

$baselineRecords = Get-PerfBaselineRecords -Path $BaselinePath
$baselineMap = New-PerfBaselineMap -Records $baselineRecords

Invoke-SurakartaConfigure -Configuration 'Release' -BuildDir $BuildDir
Invoke-SurakartaBuild -BuildDir $BuildDir -Targets @('surakarta-benchmark')

New-Item -ItemType Directory -Force -Path $logsDir | Out-Null
Stop-SurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot | Out-Null

$cases = @(
    [pscustomobject]@{ CaseId = 'opening'; File = $null },
    [pscustomobject]@{ CaseId = 'middlegame'; File = (Join-Path $repoRoot 'test\test_data\game1.txt') },
    [pscustomobject]@{ CaseId = 'endgame'; File = (Join-Path $repoRoot 'test\test_data\game6.txt') }
)
$depths = @(6, 7, 8)
$threads = @(1, 2, 4)
$results = New-Object System.Collections.Generic.List[object]
$comparisons = New-Object System.Collections.Generic.List[object]
$failures = New-Object System.Collections.Generic.List[object]
$baselineSeconds = @{}

foreach ($case in $cases) {
    foreach ($depth in $depths) {
        foreach ($threadCount in $threads) {
            $baselineKey = Get-PerfKey -CaseId $case.CaseId -Depth $depth -Threads $threadCount
            if (-not $baselineMap.ContainsKey($baselineKey)) {
                throw "Missing perf baseline entry for $baselineKey in $BaselinePath."
            }

            $tag = '{0}-d{1}-t{2}' -f $case.CaseId, $depth, $threadCount
            $stdoutLog = Join-Path $logsDir "$tag.stdout.log"
            $stderrLog = Join-Path $logsDir "$tag.stderr.log"
            $args = @(
                'bitboard-benchmark',
                '--format', 'json',
                '--case-id', $case.CaseId,
                '--depth', "$depth",
                '--threads', "$threadCount",
                '--movegen-iters', "$MovegenIterations"
            )
            if ($case.File) {
                $args += @('--file', $case.File)
            }

            $startTime = Get-Date
            $process = Start-Process -FilePath $benchmarkExe `
                -ArgumentList $args `
                -WorkingDirectory (Join-Path $BuildDir 'bin') `
                -RedirectStandardOutput $stdoutLog `
                -RedirectStandardError $stderrLog `
                -NoNewWindow `
                -PassThru

            $timedOut = $false
            try {
                Wait-Process -Id $process.Id -Timeout $TimeoutSeconds -ErrorAction Stop
            } catch {
                $timedOut = $true
                Stop-Process -Id $process.Id -Force -ErrorAction SilentlyContinue
                Wait-Process -Id $process.Id -ErrorAction SilentlyContinue
            }

            $process.Refresh()
            $wallSeconds = [Math]::Round(((Get-Date) - $startTime).TotalSeconds, 6)
            $cleaned = @(Stop-SurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot -ExcludeProcessId @($PID))
            if ($timedOut) {
                throw "Perf case $tag timed out after $TimeoutSeconds seconds. See $stdoutLog and $stderrLog."
            }
            if ($process.ExitCode -ne 0) {
                throw "Perf case $tag failed with exit code $($process.ExitCode). See $stdoutLog and $stderrLog."
            }

            $rawJson = Get-Content -Path $stdoutLog -Raw
            $record = $rawJson | ConvertFrom-Json
            if ($threadCount -eq 1) {
                $baselineSeconds["$($case.CaseId)|$depth"] = [double]$record.search_seconds
                $speedup = 1.0
            } else {
                $speedupBaselineKey = "$($case.CaseId)|$depth"
                if (-not $baselineSeconds.ContainsKey($speedupBaselineKey)) {
                    throw "Missing 1T baseline for $speedupBaselineKey before computing speedup."
                }
                $speedup = $baselineSeconds[$speedupBaselineKey] / [double]$record.search_seconds
            }

            $current = [pscustomobject]@{
                case_id = $record.case_id
                depth = [int]$record.depth
                threads = [int]$record.threads
                search_seconds = [double]$record.search_seconds
                nodes = [uint64]$record.nodes
                nps = [uint64]$record.nps
                tt_hits = [uint64]$record.tt_hits
                null_prunes = [uint64]$record.null_prunes
                lmr_reductions = [uint64]$record.lmr_reductions
                fail_highs = [uint64]$record.fail_highs
                fail_lows = [uint64]$record.fail_lows
                aspiration_retries = [uint64]$record.aspiration_retries
                pv = [string]$record.pv
                best_move = [string]$record.best_move
                score = [int]$record.score
                speedup = [Math]::Round($speedup, 6)
                wall_seconds = $wallSeconds
                stdout_log = $stdoutLog
                stderr_log = $stderrLog
                cleaned_processes = $cleaned.Count
            }
            $results.Add($current)

            $comparison = Compare-PerfEntry -Current $current -Baseline $baselineMap[$baselineKey]
            $comparisons.Add($comparison)
            foreach ($failure in $comparison.failures) {
                $failures.Add($failure)
            }
        }
    }
}

$jsonPath = Join-Path $runRoot 'perf-summary.json'
$textPath = Join-Path $runRoot 'perf-summary.txt'
$gateJsonPath = Join-Path $runRoot 'perf-gate.json'
$gateTextPath = Join-Path $runRoot 'perf-gate.txt'

$results | ConvertTo-Json -Depth 6 | Set-Content -Path $jsonPath

$status = if ($failures.Count -gt 0) { 'fail' } else { 'pass' }
$gate = [pscustomobject]@{
    status = $status
    baseline_path = $BaselinePath
    failure_count = [int]$failures.Count
    failures = @($failures.ToArray())
    results = @($comparisons.ToArray())
}
$gate | ConvertTo-Json -Depth 10 | Set-Content -Path $gateJsonPath

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add("Surakarta perf suite")
$lines.Add("Output: $runRoot")
$lines.Add("Baseline: $BaselinePath")
$lines.Add("status: $status")
$lines.Add('')

foreach ($caseId in @('opening', 'middlegame', 'endgame')) {
    foreach ($depth in $depths) {
        $lines.Add("$caseId depth $depth")
        $subset = $results | Where-Object { $_.case_id -eq $caseId -and $_.depth -eq $depth } | Sort-Object threads
        foreach ($entry in $subset) {
            $comparison = $comparisons | Where-Object { $_.case_id -eq $entry.case_id -and $_.depth -eq $entry.depth -and $_.threads -eq $entry.threads } | Select-Object -First 1
            $lines.Add(("  {0}T  nodes={1}  nps={2}  tt_hits={3}  speedup={4:N3}  search_seconds={5:N6}  performance_ratio={6:N6}  basis={7}  pv={8}" -f `
                $entry.threads,
                $entry.nodes,
                $entry.nps,
                $entry.tt_hits,
                $entry.speedup,
                $entry.search_seconds,
                $comparison.performance_ratio,
                $comparison.performance_basis,
                $entry.pv))
            foreach ($failure in $comparison.failures) {
                $lines.Add(("    FAIL {0}" -f $failure.reason))
            }
        }
        $lines.Add('')
    }
}

$lines | Set-Content -Path $textPath
$lines | Set-Content -Path $gateTextPath
Assert-NoSurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot

if ($failures.Count -gt 0) {
    throw "Perf baseline gate failed with $($failures.Count) regression(s). See $gateJsonPath."
}

Write-Host "[PASS] perf baseline gate"
Write-Host "JSON: $jsonPath"
Write-Host "Gate JSON: $gateJsonPath"
Write-Host "Text: $textPath"
