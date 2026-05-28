[CmdletBinding()]
param(
    [string]$RunRoot = ".\build-local\training\20260522-172134",

    [string]$OutputDir = ".\build-local\training\20260522-172134\opening-diagnostics\phase3_9_offline_neutralization",

    [string]$BenchmarkExe = ".\build-vs-train-release\bin\surakarta-benchmark.exe",

    [string[]]$Seeds = @("20260423", "20260424", "20260425"),

    [string[]]$Stages = @("cp2", "final")
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$ExpectedMagic = [uint32]0x5455504C
$ExpectedVersion = [uint16]1
$ExpectedTupleSetHash = [uint64]0x1D4A6EBF2BAF6484
$ExpectedTupleSetHashText = "0x1D4A6EBF2BAF6484"
$ExpectedWeightCount = [int]16605
$ExpectedHeaderBytes = 24
$ExpectedFileLength = $ExpectedHeaderBytes + ($ExpectedWeightCount * 4)
$ExpectedDepth = 4
$ExpectedThreads = 1
$ExpectedCaseId = "opening"
$BaselineMove = "(0, 1) -> (1, 2) (B)"
$Cp2DriftMove = "(5, 1) -> (4, 2) (B)"
$FinalDriftMove = "(3, 1) -> (2, 2) (B)"

function Fail {
    param(
        [string]$Point,
        [string]$File,
        [object]$Actual,
        [object]$Expected
    )
    throw "[offline-neutralization-diagnostics] first_failure='$Point' file='$File' actual='$Actual' expected='$Expected'"
}

function Assert-Equal {
    param(
        [object]$Actual,
        [object]$Expected,
        [string]$Point,
        [string]$File
    )
    if ($Actual -ne $Expected) {
        Fail $Point $File $Actual $Expected
    }
}

function Assert-DoubleEqual {
    param(
        [double]$Actual,
        [double]$Expected,
        [string]$Point,
        [string]$File
    )
    if ([math]::Abs($Actual - $Expected) -gt 0.000000001) {
        Fail $Point $File $Actual $Expected
    }
}

function Get-FullPath {
    param([string]$Path)
    return [System.IO.Path]::GetFullPath($Path)
}

function Get-ExistingFullPath {
    param([string]$Path)
    return (Resolve-Path -LiteralPath $Path).ProviderPath
}

function Get-FileSha256 {
    param([string]$Path)
    return (Get-FileHash -LiteralPath $Path -Algorithm SHA256).Hash.ToUpperInvariant()
}

function Read-RequiredTextFile {
    param(
        [string]$Path,
        [string]$Label
    )
    $fullPath = Get-ExistingFullPath $Path
    $item = Get-Item -LiteralPath $fullPath
    return [pscustomobject]@{
        label = $Label
        path = $fullPath
        sha256 = Get-FileSha256 $fullPath
        length = $item.Length
        content = Get-Content -LiteralPath $fullPath -Raw
    }
}

function Convert-ListArgs {
    param([string[]]$Values)
    $items = New-Object System.Collections.Generic.List[string]
    foreach ($value in $Values) {
        foreach ($part in ($value -split ",")) {
            $trimmed = $part.Trim()
            if ($trimmed.Length -gt 0) {
                $items.Add($trimmed)
            }
        }
    }
    return [string[]]@($items)
}

function Convert-SeedArgs {
    param([string[]]$Values)
    return [int[]]@(Convert-ListArgs $Values | ForEach-Object { [int]$_ })
}

function ConvertTo-BoolText {
    param([bool]$Value)
    if ($Value) { return "true" }
    return "false"
}

function New-VariantDefinitions {
    return @(
        [pscustomobject]@{
            id = "conservative_core"
            label = "conservative core"
            tuple_ids = [int[]]@(5, 10, 32, 41)
            risk = "conservative"
            expected_target = "shared cp2 onset and final gate blocker core"
        },
        [pscustomobject]@{
            id = "c1_only_k10"
            label = "C1-only K10"
            tuple_ids = [int[]]@(4, 5, 6, 10, 11, 27, 32, 36, 38, 41)
            risk = "bounded_k10"
            expected_target = "cp2 checkpoint-2 first drift"
        },
        [pscustomobject]@{
            id = "c2_focused_stable_k10"
            label = "C2-focused stable K10"
            tuple_ids = [int[]]@(5, 10, 32, 41, 43)
            risk = "focused_stable"
            expected_target = "final gate blocker"
        },
        [pscustomobject]@{
            id = "union_k10_riskier"
            label = "union K10 riskier"
            tuple_ids = [int[]]@(4, 5, 6, 10, 11, 27, 32, 36, 38, 41, 43)
            risk = "riskier_pressure_control"
            expected_target = "shared stress check only"
        }
    )
}

function Get-StageComparisonId {
    param([string]$Stage)
    if ($Stage -eq "cp2") {
        return "C1_cp2_onset"
    }
    if ($Stage -eq "final") {
        return "C2_final_gate_blocker"
    }
    Fail "unsupported stage" $Stage $Stage "cp2|final"
}

function Get-StageArtifact {
    param(
        [object]$Manifest,
        [object]$SelectionSummary,
        [int]$Seed,
        [string]$Stage
    )
    $matches = @(
        $Manifest.artifacts | Where-Object {
            $_.seed -eq $Seed -and (
                ($Stage -eq "bootstrap" -and $_.artifact_kind -eq "bootstrap") -or
                ($Stage -eq "cp2" -and $_.artifact_kind -eq "checkpoint" -and $_.checkpoint.games_completed -eq 2) -or
                ($Stage -eq "final" -and $_.artifact_kind -eq "candidate")
            )
        }
    )
    if ($matches.Count -ne 1) {
        Fail "manifest artifact lookup seed=${Seed} stage=${Stage}" "weights-manifest.json" $matches.Count 1
    }
    $artifact = $matches[0]
    Assert-Equal $artifact.seed $Seed "artifact seed seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-Equal $artifact.depth $SelectionSummary.depth "artifact depth seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-DoubleEqual ([double]$artifact.alpha) ([double]$SelectionSummary.alpha) "artifact alpha seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-DoubleEqual ([double]$artifact.lambda) ([double]$SelectionSummary.lambda) "artifact lambda seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-DoubleEqual ([double]$artifact.epsilon) ([double]$SelectionSummary.epsilon) "artifact epsilon seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-Equal $artifact.epsilon_plies $SelectionSummary.epsilon_plies "artifact epsilon_plies seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-DoubleEqual ([double]$artifact.terminal_reward) ([double]$SelectionSummary.terminal_reward) "artifact terminal_reward seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-DoubleEqual ([double]$artifact.td_error_clip) ([double]$SelectionSummary.td_error_clip) "artifact td_error_clip seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-Equal $artifact.terminal_only_warmup $SelectionSummary.terminal_only_warmup "artifact terminal_only_warmup seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-Equal $artifact.near_terminal_curriculum $SelectionSummary.near_terminal_curriculum "artifact near_terminal_curriculum seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-Equal $artifact.checkpoint_every $SelectionSummary.checkpoint_every "artifact checkpoint_every seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-Equal $artifact.tuple_set_hash $ExpectedTupleSetHashText "artifact tuple_set_hash seed=${Seed} stage=${Stage}" $artifact.weights_path
    Assert-Equal $artifact.weight_count $ExpectedWeightCount "artifact weight_count seed=${Seed} stage=${Stage}" $artifact.weights_path

    if ($Stage -eq "bootstrap") {
        Assert-Equal $artifact.artifact_kind "bootstrap" "artifact kind seed=${Seed} stage=${Stage}" $artifact.weights_path
        Assert-Equal $artifact.games 0 "artifact games seed=${Seed} stage=${Stage}" $artifact.weights_path
        Assert-Equal $artifact.checkpoint.games_completed 0 "checkpoint games seed=${Seed} stage=${Stage}" $artifact.weights_path
    } elseif ($Stage -eq "cp2") {
        Assert-Equal $artifact.artifact_kind "checkpoint" "artifact kind seed=${Seed} stage=${Stage}" $artifact.weights_path
        Assert-Equal $artifact.games $SelectionSummary.games_per_seed "artifact games seed=${Seed} stage=${Stage}" $artifact.weights_path
        Assert-Equal $artifact.checkpoint.games_completed 2 "checkpoint games seed=${Seed} stage=${Stage}" $artifact.weights_path
    } elseif ($Stage -eq "final") {
        Assert-Equal $artifact.artifact_kind "candidate" "artifact kind seed=${Seed} stage=${Stage}" $artifact.weights_path
        Assert-Equal $artifact.games $SelectionSummary.games_per_seed "artifact games seed=${Seed} stage=${Stage}" $artifact.weights_path
        Assert-Equal $artifact.checkpoint.games_completed $SelectionSummary.games_per_seed "checkpoint games seed=${Seed} stage=${Stage}" $artifact.weights_path
    }
    return $artifact
}

function Read-WeightFile {
    param(
        [object]$Artifact,
        [string]$Stage
    )
    $fullPath = Get-ExistingFullPath $Artifact.weights_path
    $item = Get-Item -LiteralPath $fullPath
    Assert-Equal $item.Length $ExpectedFileLength "weight file length stage=${Stage}" $fullPath
    $sha256 = Get-FileSha256 $fullPath
    Assert-Equal $sha256 ([string]$Artifact.binary_sha256).ToUpperInvariant() "weight SHA256 stage=${Stage}" $fullPath

    $stream = [System.IO.File]::Open($fullPath, [System.IO.FileMode]::Open, [System.IO.FileAccess]::Read, [System.IO.FileShare]::Read)
    try {
        $reader = [System.IO.BinaryReader]::new($stream)
        $magic = $reader.ReadUInt32()
        $version = $reader.ReadUInt16()
        $reserved = $reader.ReadUInt16()
        $tupleHash = $reader.ReadUInt64()
        $weightCount = $reader.ReadUInt32()
        $padding = $reader.ReadBytes(4)
        Assert-Equal $magic $ExpectedMagic "weight magic stage=${Stage}" $fullPath
        Assert-Equal $version $ExpectedVersion "weight version stage=${Stage}" $fullPath
        Assert-Equal $tupleHash $ExpectedTupleSetHash "weight tuple_set_hash stage=${Stage}" $fullPath
        Assert-Equal $weightCount $ExpectedWeightCount "weight_count stage=${Stage}" $fullPath
        Assert-Equal $padding.Count 4 "weight header padding stage=${Stage}" $fullPath
        Assert-Equal $stream.Position $ExpectedHeaderBytes "weight header byte count stage=${Stage}" $fullPath

        $weights = New-Object int[] $ExpectedWeightCount
        for ($i = 0; $i -lt $ExpectedWeightCount; $i++) {
            $weights[$i] = $reader.ReadInt32()
        }
        Assert-Equal $stream.Position $stream.Length "weight int32 payload count stage=${Stage}" $fullPath
        return [pscustomobject]@{
            path = $fullPath
            sha256 = $sha256
            magic = ("0x{0:X8}" -f $magic)
            version = [int]$version
            reserved = [int]$reserved
            tuple_set_hash = ("0x{0:X16}" -f $tupleHash)
            weight_count = [int]$weightCount
            file_length = [int64]$item.Length
            weights = $weights
        }
    } finally {
        $stream.Dispose()
    }
}

function Write-WeightFile {
    param(
        [string]$Path,
        [int[]]$Weights
    )
    $fullPath = Get-FullPath $Path
    $outputRoot = Get-FullPath $script:OutputDirPath
    if (-not $fullPath.StartsWith($outputRoot, [System.StringComparison]::OrdinalIgnoreCase)) {
        Fail "neutralized output containment" $fullPath $fullPath $outputRoot
    }
    $fileName = [System.IO.Path]::GetFileName($fullPath)
    if ($fileName -in @("bootstrap.bin", "candidate.bin", "checkpoint-2.bin") -or $fileName -like "checkpoint-*.bin") {
        Fail "forbidden neutralized output name" $fullPath $fileName "diagnostic-neutralized*.bin"
    }

    $parent = [System.IO.Path]::GetDirectoryName($fullPath)
    New-Item -ItemType Directory -Path $parent -Force | Out-Null
    $stream = [System.IO.File]::Open($fullPath, [System.IO.FileMode]::Create, [System.IO.FileAccess]::Write, [System.IO.FileShare]::None)
    try {
        $writer = [System.IO.BinaryWriter]::new($stream)
        $writer.Write([uint32]$ExpectedMagic)
        $writer.Write([uint16]$ExpectedVersion)
        $writer.Write([uint16]0)
        $writer.Write([uint64]$ExpectedTupleSetHash)
        $writer.Write([uint32]$ExpectedWeightCount)
        $writer.Write([byte[]]@(0, 0, 0, 0))
        foreach ($weight in $Weights) {
            $writer.Write([int32]$weight)
        }
    } finally {
        $stream.Dispose()
    }
    $item = Get-Item -LiteralPath $fullPath
    Assert-Equal $item.Length $ExpectedFileLength "neutralized file length" $fullPath
    return [pscustomobject]@{
        path = $fullPath
        sha256 = Get-FileSha256 $fullPath
        length = $item.Length
    }
}

function Invoke-SearchDiagnostics {
    param(
        [string]$WeightsPath,
        [string]$OutPath,
        [string]$RunLabel
    )
    $command = @(
        $BenchmarkExePath,
        "bitboard-search",
        "--case-id", $ExpectedCaseId,
        "--depth", "$ExpectedDepth",
        "--threads", "$ExpectedThreads",
        "--weights", $WeightsPath,
        "--format", "json",
        "--search-diagnostics"
    )
    $output = & $BenchmarkExePath bitboard-search --case-id $ExpectedCaseId --depth $ExpectedDepth --threads $ExpectedThreads --weights $WeightsPath --format json --search-diagnostics 2>&1
    $exitCode = $LASTEXITCODE
    $text = ($output | ForEach-Object { [string]$_ }) -join [Environment]::NewLine
    if ($exitCode -ne 0) {
        Fail "diagnostics command exit run=${RunLabel}" $WeightsPath $exitCode 0
    }
    try {
        $parsed = $text | ConvertFrom-Json
    } catch {
        Fail "diagnostics JSON parse run=${RunLabel}" $OutPath $_.Exception.Message "valid json"
    }
    $parent = [System.IO.Path]::GetDirectoryName($OutPath)
    New-Item -ItemType Directory -Path $parent -Force | Out-Null
    $text | Set-Content -LiteralPath $OutPath -Encoding utf8
    return [pscustomobject]@{
        command = ($command -join " ")
        path = Get-FullPath $OutPath
        sha256 = Get-FileSha256 $OutPath
        report = $parsed
    }
}

function Get-RootOrderInfo {
    param([object]$Report)
    $rootRows = @($Report.diagnostics.root_moves | Where-Object { [int]$_.depth -eq $ExpectedDepth })
    if ($rootRows.Count -eq 0) {
        Fail "root diagnostics depth rows" "diagnostics json" 0 ">0"
    }
    $finalAttempt = ($rootRows | Measure-Object -Property attempt -Maximum).Maximum
    $attemptRows = @($rootRows | Where-Object { [int]$_.attempt -eq [int]$finalAttempt } | Sort-Object -Property move_index)
    $rootOrder = (($attemptRows | ForEach-Object { $_.move }) -join " | ")
    $byMove = @{}
    foreach ($row in $attemptRows) {
        if (-not $byMove.ContainsKey([string]$row.move)) {
            $byMove[[string]$row.move] = $row
        }
    }
    $getRank = {
        param([string]$Move)
        if ($byMove.ContainsKey($Move)) {
            return ([int]$byMove[$Move].move_index + 1)
        }
        return $null
    }
    $getNodes = {
        param([string]$Move, [string]$PropertyName)
        if ($byMove.ContainsKey($Move)) {
            return [int64]$byMove[$Move].$PropertyName
        }
        return $null
    }
    return [pscustomobject]@{
        final_attempt = [int]$finalAttempt
        root_order = $rootOrder
        root_move_count = $attemptRows.Count
        baseline_rank = & $getRank $BaselineMove
        cp2_drift_rank = & $getRank $Cp2DriftMove
        final_drift_rank = & $getRank $FinalDriftMove
        baseline_nodes = & $getNodes $BaselineMove "nodes"
        baseline_qnodes = & $getNodes $BaselineMove "qnodes"
        cp2_drift_nodes = & $getNodes $Cp2DriftMove "nodes"
        cp2_drift_qnodes = & $getNodes $Cp2DriftMove "qnodes"
        final_drift_nodes = & $getNodes $FinalDriftMove "nodes"
        final_drift_qnodes = & $getNodes $FinalDriftMove "qnodes"
    }
}

function Get-StaticNeutralizationRows {
    param(
        [object[]]$StaticTupleRows,
        [int]$Seed,
        [string]$Stage,
        [int[]]$TupleIds
    )
    $comparisonId = Get-StageComparisonId $Stage
    $rows = @(
        $StaticTupleRows | Where-Object {
            [int]$_.seed -eq $Seed -and
            $_.comparison_id -eq $comparisonId -and
            $_.stage -eq $Stage -and
            ($TupleIds -contains [int]$_.tuple_id)
        }
    )
    return $rows
}

function Get-AttributionTupleRow {
    param(
        [hashtable]$TupleByKey,
        [int]$Seed,
        [string]$Stage,
        [string]$RootMoveLabel,
        [int]$TupleId
    )
    $key = "$Seed|$Stage|$RootMoveLabel|$TupleId"
    if (-not $TupleByKey.ContainsKey($key)) {
        Fail "attribution tuple lookup seed=${Seed} stage=${Stage} root=${RootMoveLabel} tuple=${TupleId}" "opening-child-attribution-tuples.csv" 0 1
    }
    return $TupleByKey[$key]
}

function Get-ComparisonRootLabels {
    param([string]$Stage)
    if ($Stage -eq "cp2") {
        return [pscustomobject]@{
            from_label = "baseline"
            to_label = "cp2"
        }
    }
    if ($Stage -eq "final") {
        return [pscustomobject]@{
            from_label = "baseline"
            to_label = "final"
        }
    }
    Fail "comparison root labels stage" "stage" $Stage "cp2|final"
}

function New-NeutralizedWeights {
    param(
        [int[]]$SourceWeights,
        [int[]]$BootstrapWeights,
        [object[]]$StaticRows,
        [hashtable]$AttributionTupleByKey,
        [int[]]$RequestedTupleIds,
        [int]$Seed,
        [string]$Stage,
        [string]$VariantId
    )
    $neutralized = [int[]]$SourceWeights.Clone()
    $deltaRows = New-Object System.Collections.Generic.List[object]
    $seen = @{}
    $appliedTupleIds = New-Object System.Collections.Generic.HashSet[int]
    $staticByTuple = @{}
    foreach ($row in $StaticRows) {
        $tupleId = [int]$row.tuple_id
        if (-not $staticByTuple.ContainsKey([string]$tupleId)) {
            $staticByTuple[[string]$tupleId] = $row
        }
    }
    $labels = Get-ComparisonRootLabels $Stage

    foreach ($tupleIdValue in $RequestedTupleIds) {
        $tupleId = [int]$tupleIdValue
        [void]$appliedTupleIds.Add($tupleId)
        $staticRow = $null
        if ($staticByTuple.ContainsKey([string]$tupleId)) {
            $staticRow = $staticByTuple[[string]$tupleId]
        }
        foreach ($side in @("from", "to")) {
            $rootLabel = if ($side -eq "from") { $labels.from_label } else { $labels.to_label }
            $attributionRow = Get-AttributionTupleRow $AttributionTupleByKey $Seed $Stage $rootLabel $tupleId
            $absoluteIndex = [int]$attributionRow.absolute_index
            $encodedIndex = [int]$attributionRow.encoded_index
            if ($absoluteIndex -lt 0 -or $absoluteIndex -ge $neutralized.Count) {
                Fail "absolute index range seed=${Seed} stage=${Stage} variant=${VariantId}" "static ablation tuple row" $absoluteIndex "0..$($neutralized.Count - 1)"
            }
            $original = [int]$neutralized[$absoluteIndex]
            $bootstrap = [int]$BootstrapWeights[$absoluteIndex]
            $key = [string]$absoluteIndex
            if ($seen.ContainsKey($key)) {
                if ([int]$seen[$key] -ne $bootstrap) {
                    Fail "conflicting neutralization target seed=${Seed} stage=${Stage} variant=${VariantId}" "absolute_index=$absoluteIndex" $bootstrap $seen[$key]
                }
                continue
            }
            $seen[$key] = $bootstrap
            $neutralized[$absoluteIndex] = $bootstrap
            $deltaRows.Add([pscustomobject][ordered]@{
                seed = $Seed
                stage = $Stage
                variant = $VariantId
                comparison_id = Get-StageComparisonId $Stage
                root_side = $side
                tuple_id = $tupleId
                encoded_index = $encodedIndex
                absolute_index = $absoluteIndex
                from_root_move = if ($null -ne $staticRow) { $staticRow.from_root_move } else { $null }
                to_root_move = if ($null -ne $staticRow) { $staticRow.to_root_move } else { $null }
                original_weight = $original
                bootstrap_weight = $bootstrap
                neutralized_weight = $bootstrap
                raw_delta_removed = $original - $bootstrap
                induced_pair_delta = if ($null -ne $staticRow) { [int]$staticRow.induced_pair_delta } else { $null }
                from_stage_delta = if ($null -ne $staticRow) { [int]$staticRow.from_stage_delta } else { $null }
                to_stage_delta = if ($null -ne $staticRow) { [int]$staticRow.to_stage_delta } else { $null }
                production_candidate = $false
                selection_gate_eligible = $false
            })
        }
    }

    return [pscustomobject]@{
        weights = $neutralized
        delta_rows = $deltaRows.ToArray()
        applied_tuple_ids = @($appliedTupleIds | ForEach-Object { [int]$_ } | Sort-Object)
        missing_tuple_ids = @()
        modified_weight_count = $seen.Count
    }
}

function Get-SelectedStaticFacts {
    param(
        [object[]]$StaticRows,
        [int]$Seed,
        [string]$Stage,
        [string]$VariantId
    )
    if ($StaticRows.Count -eq 0) {
        return [pscustomobject]@{
            stage_pair_delta = $null
            bootstrap_pair_delta = $null
            training_induced_pair_delta = $null
            selected_induced_delta_sum = 0
            ablated_stage_pair_delta = $null
            static_over_neutralization = $false
        }
    }
    $comparisonId = Get-StageComparisonId $Stage
    $sweepPath = Join-Path $script:Phase37Dir "opening-child-static-ablation-sweep.csv"
    $sweepRows = @(Import-Csv -LiteralPath $sweepPath | Where-Object {
        [int]$_.seed -eq $Seed -and $_.comparison_id -eq $comparisonId -and $_.topk -eq "all_negative"
    })
    if ($sweepRows.Count -ne 1) {
        Fail "static sweep fact lookup seed=${Seed} stage=${Stage} variant=${VariantId}" $sweepPath $sweepRows.Count 1
    }
    $stagePairDelta = [int]$sweepRows[0].stage_pair_delta
    $bootstrapPairDelta = [int]$sweepRows[0].bootstrap_pair_delta
    $trainingInducedPairDelta = [int]$sweepRows[0].training_induced_pair_delta
    $selectedInducedDeltaSum = 0
    foreach ($row in $StaticRows) {
        $selectedInducedDeltaSum += [int]$row.induced_pair_delta
    }
    $ablatedStagePairDelta = $stagePairDelta - $selectedInducedDeltaSum
    return [pscustomobject]@{
        stage_pair_delta = $stagePairDelta
        bootstrap_pair_delta = $bootstrapPairDelta
        training_induced_pair_delta = $trainingInducedPairDelta
        selected_induced_delta_sum = $selectedInducedDeltaSum
        ablated_stage_pair_delta = $ablatedStagePairDelta
        static_over_neutralization = [bool]($stagePairDelta -lt 0 -and $ablatedStagePairDelta -gt 0)
    }
}

$SeedValues = Convert-SeedArgs $Seeds
$StageValues = Convert-ListArgs $Stages
foreach ($stage in $StageValues) {
    if ($stage -notin @("cp2", "final")) {
        Fail "stage argument" "parameter Stages" $stage "cp2|final"
    }
}

$RepoRoot = Get-FullPath (Join-Path $PSScriptRoot "..")
$RunRootPath = Get-ExistingFullPath $RunRoot
$script:OutputDirPath = Get-FullPath $OutputDir
$BenchmarkExePath = Get-ExistingFullPath $BenchmarkExe
$script:Phase37Dir = Join-Path $RunRootPath "opening-diagnostics\phase3_7_static_ablation"
$Phase35Dir = Join-Path $RunRootPath "opening-diagnostics\phase3_5_attribution"

New-Item -ItemType Directory -Path $script:OutputDirPath -Force | Out-Null
$DiagnosticsDir = Join-Path $script:OutputDirPath "diagnostics-json"
$WeightsTempDir = Join-Path $script:OutputDirPath "weights-temp"
New-Item -ItemType Directory -Path $DiagnosticsDir -Force | Out-Null
New-Item -ItemType Directory -Path $WeightsTempDir -Force | Out-Null

$RequiredInputs = [ordered]@{
    task_plan = Read-RequiredTextFile (Join-Path $RepoRoot "task_plan.md") "task_plan"
    findings = Read-RequiredTextFile (Join-Path $RepoRoot "findings.md") "findings"
    progress = Read-RequiredTextFile (Join-Path $RepoRoot "progress.md") "progress"
    national_competition_technical_roadmap = Read-RequiredTextFile (Join-Path $RepoRoot "docs\national_competition_technical_roadmap.md") "national_competition_technical_roadmap"
    weights_manifest = Read-RequiredTextFile (Join-Path $RunRootPath "weights-manifest.json") "weights_manifest"
    selection_summary = Read-RequiredTextFile (Join-Path $RunRootPath "selection-summary.json") "selection_summary"
    opening_diagnostics_summary = Read-RequiredTextFile (Join-Path $RunRootPath "opening-diagnostics\opening-diagnostics-summary.md") "opening_diagnostics_summary"
    phase35_summary_csv = Read-RequiredTextFile (Join-Path $Phase35Dir "opening-child-attribution-summary.csv") "phase35_summary_csv"
    phase35_tuples_csv = Read-RequiredTextFile (Join-Path $Phase35Dir "opening-child-attribution-tuples.csv") "phase35_tuples_csv"
    phase35_top_deltas_csv = Read-RequiredTextFile (Join-Path $Phase35Dir "opening-child-attribution-top-deltas.csv") "phase35_top_deltas_csv"
    phase35_manifest = Read-RequiredTextFile (Join-Path $Phase35Dir "attribution-manifest.json") "phase35_manifest"
    phase37_summary_csv = Read-RequiredTextFile (Join-Path $Phase37Dir "opening-child-static-ablation-summary.csv") "phase37_summary_csv"
    phase37_tuples_csv = Read-RequiredTextFile (Join-Path $Phase37Dir "opening-child-static-ablation-tuples.csv") "phase37_tuples_csv"
    phase37_sweep_csv = Read-RequiredTextFile (Join-Path $Phase37Dir "opening-child-static-ablation-sweep.csv") "phase37_sweep_csv"
    phase37_summary_json = Read-RequiredTextFile (Join-Path $Phase37Dir "opening-child-static-ablation-summary.json") "phase37_summary_json"
    phase37_manifest = Read-RequiredTextFile (Join-Path $Phase37Dir "opening-child-static-ablation-manifest.json") "phase37_manifest"
}

$Manifest = $RequiredInputs.weights_manifest.content | ConvertFrom-Json
$SelectionSummary = $RequiredInputs.selection_summary.content | ConvertFrom-Json
$Phase35Manifest = $RequiredInputs.phase35_manifest.content | ConvertFrom-Json
$Phase37Manifest = $RequiredInputs.phase37_manifest.content | ConvertFrom-Json
$AttributionTupleRows = @(Import-Csv -LiteralPath $RequiredInputs.phase35_tuples_csv.path)
$StaticTupleRows = @(Import-Csv -LiteralPath $RequiredInputs.phase37_tuples_csv.path)

Assert-Equal $Manifest.purpose "selection" "weights manifest purpose" $RequiredInputs.weights_manifest.path
Assert-Equal $SelectionSummary.games_per_seed 32 "Phase 3.1 games_per_seed" $RequiredInputs.selection_summary.path
Assert-Equal $SelectionSummary.depth 4 "Phase 3.1 depth" $RequiredInputs.selection_summary.path
Assert-DoubleEqual ([double]$SelectionSummary.alpha) 0.005 "Phase 3.1 alpha" $RequiredInputs.selection_summary.path
Assert-DoubleEqual ([double]$SelectionSummary.lambda) 0.5 "Phase 3.1 lambda" $RequiredInputs.selection_summary.path
Assert-DoubleEqual ([double]$SelectionSummary.epsilon) 0.02 "Phase 3.1 epsilon" $RequiredInputs.selection_summary.path
Assert-Equal $SelectionSummary.epsilon_plies 6 "Phase 3.1 epsilon_plies" $RequiredInputs.selection_summary.path
Assert-DoubleEqual ([double]$SelectionSummary.terminal_reward) 1200.0 "Phase 3.1 terminal_reward" $RequiredInputs.selection_summary.path
Assert-DoubleEqual ([double]$SelectionSummary.td_error_clip) 0.0 "Phase 3.1 td_error_clip" $RequiredInputs.selection_summary.path
Assert-Equal $SelectionSummary.overall_rating "regress" "Phase 3.1 overall rating remains failed" $RequiredInputs.selection_summary.path
Assert-Equal $Phase35Manifest.tuple_set_hash $ExpectedTupleSetHashText "Phase 3.5 tuple_set_hash" $RequiredInputs.phase35_manifest.path
Assert-Equal $Phase35Manifest.weight_count $ExpectedWeightCount "Phase 3.5 weight_count" $RequiredInputs.phase35_manifest.path
Assert-Equal $Phase37Manifest.tuple_set_hash $ExpectedTupleSetHashText "Phase 3.7 tuple_set_hash" $RequiredInputs.phase37_manifest.path
Assert-Equal $Phase37Manifest.weight_count $ExpectedWeightCount "Phase 3.7 weight_count" $RequiredInputs.phase37_manifest.path
Assert-Equal $Phase37Manifest.boundaries.weights_written $false "Phase 3.7 weights_written boundary" $RequiredInputs.phase37_manifest.path
Assert-Equal $Phase37Manifest.boundaries.offline_neutralization_executed $false "Phase 3.7 neutralization boundary" $RequiredInputs.phase37_manifest.path

$AttributionTupleByKey = @{}
foreach ($row in $AttributionTupleRows) {
    $key = "$($row.seed)|$($row.stage)|$($row.root_move_label)|$($row.tuple_id)"
    if ($AttributionTupleByKey.ContainsKey($key)) {
        Fail "duplicate attribution tuple key" $RequiredInputs.phase35_tuples_csv.path $key "unique"
    }
    $AttributionTupleByKey[$key] = $row
}

foreach ($seed in $SeedValues) {
    if (@($SelectionSummary.seeds) -notcontains $seed) {
        Fail "seed presence in selection-summary" $RequiredInputs.selection_summary.path $seed ($SelectionSummary.seeds -join ",")
    }
}

$Variants = New-VariantDefinitions
$ArtifactRecords = @{}
$WeightRecords = @{}
$ArtifactValidationRows = New-Object System.Collections.Generic.List[object]
foreach ($seed in $SeedValues) {
    foreach ($stage in @("bootstrap") + $StageValues) {
        $artifact = Get-StageArtifact $Manifest $SelectionSummary $seed $stage
        $weightFile = Read-WeightFile $artifact $stage
        $key = "$seed|$stage"
        $ArtifactRecords[$key] = $artifact
        $WeightRecords[$key] = $weightFile
        $ArtifactValidationRows.Add([pscustomobject][ordered]@{
            seed = $seed
            stage = $stage
            artifact_kind = $artifact.artifact_kind
            checkpoint_games_completed = $artifact.checkpoint.games_completed
            path = $weightFile.path
            sha256 = $weightFile.sha256
            magic = $weightFile.magic
            version = $weightFile.version
            tuple_set_hash = $weightFile.tuple_set_hash
            weight_count = $weightFile.weight_count
            file_length = $weightFile.file_length
            hash_status = "pass"
            header_status = "pass"
            parameter_status = "pass"
        })
    }
}

$OriginalDiagnostics = @{}
$DiagnosticsRows = New-Object System.Collections.Generic.List[object]
$SummaryRows = New-Object System.Collections.Generic.List[object]
$WeightDeltaRows = New-Object System.Collections.Generic.List[object]
$ManifestArtifacts = New-Object System.Collections.Generic.List[object]
$DiagnosticsRunCount = 0

foreach ($seed in $SeedValues) {
    foreach ($stage in $StageValues) {
        $sourceWeight = $WeightRecords["$seed|$stage"]
        $originalJson = Join-Path $DiagnosticsDir ("seed-{0}.{1}.original.opening.root-diagnostics.json" -f $seed, $stage)
        $originalRun = Invoke-SearchDiagnostics $sourceWeight.path $originalJson "seed-${seed}-${stage}-original"
        $DiagnosticsRunCount++
        $originalInfo = Get-RootOrderInfo $originalRun.report
        $OriginalDiagnostics["$seed|$stage"] = [pscustomobject]@{
            run = $originalRun
            info = $originalInfo
        }

        foreach ($variant in $Variants) {
            $staticRows = @(Get-StaticNeutralizationRows $StaticTupleRows $seed $stage $variant.tuple_ids)
            $staticFacts = Get-SelectedStaticFacts $staticRows $seed $stage $variant.id
            $neutralized = New-NeutralizedWeights `
                -SourceWeights $sourceWeight.weights `
                -BootstrapWeights $WeightRecords["$seed|bootstrap"].weights `
                -StaticRows $staticRows `
                -AttributionTupleByKey $AttributionTupleByKey `
                -RequestedTupleIds $variant.tuple_ids `
                -Seed $seed `
                -Stage $stage `
                -VariantId $variant.id

            foreach ($deltaRow in $neutralized.delta_rows) {
                $WeightDeltaRows.Add($deltaRow)
            }

            $weightOutDir = Join-Path $WeightsTempDir (Join-Path $variant.id (Join-Path "seed-$seed" $stage))
            $weightOut = Join-Path $weightOutDir ("seed-{0}.{1}.{2}.diagnostic-neutralized.bin" -f $seed, $stage, $variant.id)
            $neutralizedFile = Write-WeightFile $weightOut $neutralized.weights
            $neutralizedReadbackArtifact = [pscustomobject]@{
                weights_path = $neutralizedFile.path
                binary_sha256 = $neutralizedFile.sha256
            }
            $neutralizedReadback = Read-WeightFile $neutralizedReadbackArtifact "neutralized-${stage}-$($variant.id)"

            $diagnosticsOut = Join-Path $DiagnosticsDir ("seed-{0}.{1}.{2}.neutralized.opening.root-diagnostics.json" -f $seed, $stage, $variant.id)
            $neutralizedRun = Invoke-SearchDiagnostics $neutralizedReadback.path $diagnosticsOut "seed-${seed}-${stage}-$($variant.id)"
            $DiagnosticsRunCount++
            $neutralizedInfo = Get-RootOrderInfo $neutralizedRun.report

            $rootOverNeutralization = [bool](
                $neutralizedRun.report.best_move -ne $BaselineMove -and
                $neutralizedRun.report.best_move -ne $originalRun.report.best_move
            )
            $bestMoveChanged = [bool]($neutralizedRun.report.best_move -ne $originalRun.report.best_move)
            $baselineRankImproved = $false
            if ($null -ne $originalInfo.baseline_rank -and $null -ne $neutralizedInfo.baseline_rank) {
                $baselineRankImproved = [bool]([int]$neutralizedInfo.baseline_rank -lt [int]$originalInfo.baseline_rank)
            }
            $nodesDelta = [int64]$neutralizedRun.report.nodes - [int64]$originalRun.report.nodes
            $qnodesDelta = [int64]$neutralizedRun.report.qnodes - [int64]$originalRun.report.qnodes
            $nodesRatio = if ([int64]$originalRun.report.nodes -gt 0) { [math]::Round(([double]$neutralizedRun.report.nodes / [double]$originalRun.report.nodes), 6) } else { $null }
            $qnodesRatio = if ([int64]$originalRun.report.qnodes -gt 0) { [math]::Round(([double]$neutralizedRun.report.qnodes / [double]$originalRun.report.qnodes), 6) } else { $null }
            $explainsRootDrift = [bool](
                $bestMoveChanged -or
                $baselineRankImproved -or
                $nodesDelta -lt 0 -or
                $qnodesDelta -lt 0
            )

            $SummaryRows.Add([pscustomobject][ordered]@{
                seed = $seed
                stage = $stage
                variant = $variant.id
                variant_label = $variant.label
                requested_tuple_ids = ($variant.tuple_ids -join "|")
                applied_tuple_ids = ($neutralized.applied_tuple_ids -join "|")
                missing_tuple_ids = ($neutralized.missing_tuple_ids -join "|")
                modified_weight_count = $neutralized.modified_weight_count
                original_best_move = $originalRun.report.best_move
                neutralized_best_move = $neutralizedRun.report.best_move
                best_move_changed = $bestMoveChanged
                original_baseline_order_rank = $originalInfo.baseline_rank
                neutralized_baseline_order_rank = $neutralizedInfo.baseline_rank
                baseline_rank_improved = $baselineRankImproved
                original_nodes = [int64]$originalRun.report.nodes
                neutralized_nodes = [int64]$neutralizedRun.report.nodes
                nodes_delta = $nodesDelta
                nodes_ratio = $nodesRatio
                original_qnodes = [int64]$originalRun.report.qnodes
                neutralized_qnodes = [int64]$neutralizedRun.report.qnodes
                qnodes_delta = $qnodesDelta
                qnodes_ratio = $qnodesRatio
                original_aspiration_retries = [int]$originalRun.report.aspiration_retries
                neutralized_aspiration_retries = [int]$neutralizedRun.report.aspiration_retries
                stage_pair_delta = $staticFacts.stage_pair_delta
                selected_induced_delta_sum = $staticFacts.selected_induced_delta_sum
                ablated_stage_pair_delta = $staticFacts.ablated_stage_pair_delta
                static_over_neutralization = $staticFacts.static_over_neutralization
                root_over_neutralization = $rootOverNeutralization
                explains_or_mitigates_root_drift = $explainsRootDrift
                production_candidate = $false
                selection_gate_eligible = $false
            })

            $DiagnosticsRows.Add([pscustomobject][ordered]@{
                seed = $seed
                stage = $stage
                variant = $variant.id
                original_json = $originalRun.path
                neutralized_json = $neutralizedRun.path
                original_weight_path = $sourceWeight.path
                neutralized_weight_path = $neutralizedFile.path
                original_best_move = $originalRun.report.best_move
                neutralized_best_move = $neutralizedRun.report.best_move
                original_score = [int]$originalRun.report.score
                neutralized_score = [int]$neutralizedRun.report.score
                original_root_order = $originalInfo.root_order
                neutralized_root_order = $neutralizedInfo.root_order
                original_baseline_order_rank = $originalInfo.baseline_rank
                neutralized_baseline_order_rank = $neutralizedInfo.baseline_rank
                original_cp2_drift_order_rank = $originalInfo.cp2_drift_rank
                neutralized_cp2_drift_order_rank = $neutralizedInfo.cp2_drift_rank
                original_final_drift_order_rank = $originalInfo.final_drift_rank
                neutralized_final_drift_order_rank = $neutralizedInfo.final_drift_rank
                original_nodes = [int64]$originalRun.report.nodes
                neutralized_nodes = [int64]$neutralizedRun.report.nodes
                original_qnodes = [int64]$originalRun.report.qnodes
                neutralized_qnodes = [int64]$neutralizedRun.report.qnodes
                original_aspiration_retries = [int]$originalRun.report.aspiration_retries
                neutralized_aspiration_retries = [int]$neutralizedRun.report.aspiration_retries
                original_baseline_child_nodes = $originalInfo.baseline_nodes
                neutralized_baseline_child_nodes = $neutralizedInfo.baseline_nodes
                original_baseline_child_qnodes = $originalInfo.baseline_qnodes
                neutralized_baseline_child_qnodes = $neutralizedInfo.baseline_qnodes
                original_cp2_drift_child_nodes = $originalInfo.cp2_drift_nodes
                neutralized_cp2_drift_child_nodes = $neutralizedInfo.cp2_drift_nodes
                original_cp2_drift_child_qnodes = $originalInfo.cp2_drift_qnodes
                neutralized_cp2_drift_child_qnodes = $neutralizedInfo.cp2_drift_qnodes
                original_final_drift_child_nodes = $originalInfo.final_drift_nodes
                neutralized_final_drift_child_nodes = $neutralizedInfo.final_drift_nodes
                original_final_drift_child_qnodes = $originalInfo.final_drift_qnodes
                neutralized_final_drift_child_qnodes = $neutralizedInfo.final_drift_qnodes
                root_over_neutralization = $rootOverNeutralization
            })

            $ManifestArtifacts.Add([pscustomobject][ordered]@{
                seed = $seed
                stage = $stage
                variant = $variant.id
                variant_tuple_ids = @($variant.tuple_ids)
                applied_tuple_ids = @($neutralized.applied_tuple_ids)
                missing_tuple_ids = @($neutralized.missing_tuple_ids)
                input_artifact = [ordered]@{
                    path = $sourceWeight.path
                    sha256 = $sourceWeight.sha256
                    artifact_kind = $ArtifactRecords["$seed|$stage"].artifact_kind
                    checkpoint_games_completed = $ArtifactRecords["$seed|$stage"].checkpoint.games_completed
                }
                output_neutralized_artifact = [ordered]@{
                    path = $neutralizedFile.path
                    sha256 = $neutralizedFile.sha256
                    length = $neutralizedFile.length
                    production_candidate = $false
                    selection_gate_eligible = $false
                }
                diagnostics = [ordered]@{
                    original_json = $originalRun.path
                    original_json_sha256 = $originalRun.sha256
                    neutralized_json = $neutralizedRun.path
                    neutralized_json_sha256 = $neutralizedRun.sha256
                }
                tuple_set_hash = $ExpectedTupleSetHashText
                weight_count = $ExpectedWeightCount
                command = $neutralizedRun.command
                validation = [ordered]@{
                    source_hash = "pass"
                    source_header = "pass"
                    output_hash = "captured"
                    output_header = "pass"
                    parameters = "pass"
                }
                production_candidate = $false
                selection_gate_eligible = $false
            })
        }
    }
}

$SummaryCsv = Join-Path $script:OutputDirPath "neutralization-summary.csv"
$RootDiagnosticsCsv = Join-Path $script:OutputDirPath "neutralization-root-diagnostics.csv"
$WeightDeltasCsv = Join-Path $script:OutputDirPath "neutralization-weight-deltas.csv"
$ManifestPath = Join-Path $script:OutputDirPath "neutralization-manifest.json"

$SummaryRows | Export-Csv -LiteralPath $SummaryCsv -NoTypeInformation
$DiagnosticsRows | Export-Csv -LiteralPath $RootDiagnosticsCsv -NoTypeInformation
$WeightDeltaRows | Export-Csv -LiteralPath $WeightDeltasCsv -NoTypeInformation

$GitCommit = (& git -C $RepoRoot rev-parse HEAD).Trim()
$GeneratedAt = (Get-Date).ToUniversalTime().ToString("o")
$InputFiles = [ordered]@{}
foreach ($key in $RequiredInputs.Keys) {
    $InputFiles[$key] = [ordered]@{
        path = $RequiredInputs[$key].path
        sha256 = $RequiredInputs[$key].sha256
        length = $RequiredInputs[$key].length
    }
}

$ManifestObject = [ordered]@{
    phase = "Phase 3.9 offline neutralization diagnostics execution"
    mode = "report_only_offline_diagnostics"
    generated_at = $GeneratedAt
    git_commit = $GitCommit
    run_root = $RunRootPath
    output_dir = $script:OutputDirPath
    benchmark_exe = $BenchmarkExePath
    tuple_set_hash = $ExpectedTupleSetHashText
    weight_count = $ExpectedWeightCount
    seeds = @($SeedValues)
    stages = @($StageValues)
    variants = @($Variants | ForEach-Object {
        [ordered]@{
            id = $_.id
            label = $_.label
            tuple_ids = @($_.tuple_ids)
            risk = $_.risk
            expected_target = $_.expected_target
        }
    })
    diagnostics = [ordered]@{
        case_id = $ExpectedCaseId
        depth = $ExpectedDepth
        threads = $ExpectedThreads
        format = "json"
        search_diagnostics = $true
        run_count = $DiagnosticsRunCount
    }
    input_files = $InputFiles
    artifact_validations = $ArtifactValidationRows.ToArray()
    artifacts = $ManifestArtifacts.ToArray()
    output_files = [ordered]@{
        neutralization_manifest_json = $ManifestPath
        neutralization_summary_csv = $SummaryCsv
        neutralization_root_diagnostics_csv = $RootDiagnosticsCsv
        neutralization_weight_deltas_csv = $WeightDeltasCsv
        diagnostics_json_dir = $DiagnosticsDir
        weights_temp_dir = $WeightsTempDir
    }
    checks = [ordered]@{
        source_artifact_count = $ArtifactValidationRows.Count
        neutralized_artifact_count = $ManifestArtifacts.Count
        diagnostics_run_count = $DiagnosticsRunCount
        summary_row_count = $SummaryRows.Count
        root_diagnostics_row_count = $DiagnosticsRows.Count
        weight_delta_row_count = $WeightDeltaRows.Count
        source_hash_status = "pass"
        source_header_status = "pass"
        output_header_status = "pass"
        json_parse_status = "pass"
    }
    boundaries = [ordered]@{
        production_candidate = $false
        selection_gate_eligible = $false
        selection_gate_modified = $false
        training_executed = $false
        selection_executed = $false
        games_per_seed_64_executed = $false
        phase4_entered = $false
        production_code_modified = $false
        benchmark_scripts_modified = $false
    }
    phase3_status = "failed"
    phase4_status = "blocked"
}
$ManifestObject | ConvertTo-Json -Depth 16 | Set-Content -LiteralPath $ManifestPath -Encoding utf8

Write-Host "[PASS] offline neutralization diagnostics generated"
Write-Host "output_dir=$script:OutputDirPath"
Write-Host "neutralized_bins=$($ManifestArtifacts.Count)"
Write-Host "diagnostics_runs=$DiagnosticsRunCount"
Write-Host "summary_rows=$($SummaryRows.Count)"
Write-Host "root_diagnostics_rows=$($DiagnosticsRows.Count)"
Write-Host "weight_delta_rows=$($WeightDeltaRows.Count)"
