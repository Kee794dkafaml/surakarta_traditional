[CmdletBinding()]
param(
    [string]$ProjectRoot = ".",

    [string]$RunRoot = ".\build-local\training\20260522-172134",

    [string]$OutputDir = ".\build-local\training\20260522-172134\opening-diagnostics\phase3_13_opening_guard"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$AllowedStatuses = @("explain", "watch", "regress", "unknown")
$StatusSeverity = @{
    explain = 0
    unknown = 1
    watch = 2
    regress = 3
}

$BaselineMove = "(0, 1) -> (1, 2) (B)"
$Cp2DriftMove = "(5, 1) -> (4, 2) (B)"
$FinalDriftMove = "(3, 1) -> (2, 2) (B)"

function Fail {
    param([string]$Message)
    throw "[opening-guard-diagnostics] $Message"
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

function Read-RequiredFile {
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
    }
}

function Read-RequiredCsv {
    param(
        [string]$Path,
        [string]$Label
    )
    $file = Read-RequiredFile $Path $Label
    $rows = @(Import-Csv -LiteralPath $file.path)
    if ($rows.Count -eq 0) {
        Fail "required CSV '$Label' has no rows: $($file.path)"
    }
    return [pscustomobject]@{
        file = $file
        rows = $rows
    }
}

function Read-RequiredJson {
    param(
        [string]$Path,
        [string]$Label
    )
    $file = Read-RequiredFile $Path $Label
    $content = Get-Content -LiteralPath $file.path -Raw
    return [pscustomobject]@{
        file = $file
        value = ($content | ConvertFrom-Json)
    }
}

function ConvertTo-NullableIntText {
    param([object]$Value)
    if ($null -eq $Value) { return "" }
    $text = "$Value".Trim()
    if ($text.Length -eq 0) { return "" }
    return ([int]$text).ToString([System.Globalization.CultureInfo]::InvariantCulture)
}

function ConvertTo-NullableDouble {
    param([object]$Value)
    if ($null -eq $Value) { return $null }
    $text = "$Value".Trim()
    if ($text.Length -eq 0) { return $null }
    return [double]::Parse($text, [System.Globalization.CultureInfo]::InvariantCulture)
}

function Get-RatioText {
    param([Nullable[double]]$Value)
    if ($null -eq $Value) { return "" }
    return ([math]::Round([double]$Value, 6)).ToString("0.######", [System.Globalization.CultureInfo]::InvariantCulture)
}

function ConvertTo-BoolValue {
    param([object]$Value)
    if ($null -eq $Value) { return $false }
    $text = "$Value".Trim()
    return ($text -ieq "true")
}

function Get-StageComparisonId {
    param([string]$Stage)
    if ($Stage -eq "cp2") { return "C1_cp2_onset" }
    if ($Stage -eq "final") { return "C2_final_gate_blocker" }
    return ""
}

function Get-MetricKey {
    param(
        [object]$Row
    )
    return "$($Row.seed)|$($Row.stage)|$($Row.variant)"
}

function Get-WorstStatus {
    param([string[]]$Statuses)
    $worst = "explain"
    foreach ($status in $Statuses) {
        $candidate = if ($AllowedStatuses -contains $status) { $status } else { "unknown" }
        if ($StatusSeverity[$candidate] -gt $StatusSeverity[$worst]) {
            $worst = $candidate
        }
    }
    return $worst
}

function Get-CostStatus {
    param(
        [Nullable[double]]$NodesRatio,
        [Nullable[double]]$QnodesRatio,
        [Nullable[double]]$QnodesPerNodeRatio,
        [int]$AspirationRetryDelta,
        [bool]$RootOverNeutralization,
        [string]$Stage
    )
    if ($RootOverNeutralization) {
        return "regress"
    }
    if (($null -ne $NodesRatio -and $NodesRatio -gt 1.25) -or
        ($null -ne $QnodesRatio -and $QnodesRatio -gt 1.25)) {
        return "regress"
    }
    if (($null -ne $NodesRatio -and $NodesRatio -gt 1.10) -or
        ($null -ne $QnodesRatio -and $QnodesRatio -gt 1.10) -or
        ($null -ne $QnodesPerNodeRatio -and $QnodesPerNodeRatio -gt 1.10) -or
        ($AspirationRetryDelta -gt 0)) {
        return "watch"
    }
    if ($Stage -eq "final") {
        return "watch"
    }
    return "explain"
}

function Get-BestMoveStatus {
    param(
        [string]$Stage,
        [string]$CandidateBestMove,
        [bool]$RootOverNeutralization
    )
    if ($CandidateBestMove -eq $BaselineMove) {
        return "explain"
    }
    if ($RootOverNeutralization) {
        return "regress"
    }
    if ($Stage -eq "cp2" -and $CandidateBestMove -eq $BaselineMove) {
        return "explain"
    }
    if ($Stage -eq "final" -and $CandidateBestMove -eq $FinalDriftMove) {
        return "watch"
    }
    if ($Stage -eq "final") {
        return "regress"
    }
    return "watch"
}

function Get-RootOrderStatus {
    param(
        [string]$Stage,
        [string]$BaselineRank,
        [string]$CandidateRank,
        [bool]$RootOverNeutralization
    )
    if ($RootOverNeutralization) {
        return "regress"
    }
    if ($BaselineRank -eq "1") {
        return "explain"
    }
    if ($Stage -eq "final") {
        return "watch"
    }
    if ($CandidateRank -eq "1") {
        return "watch"
    }
    return "unknown"
}

function Get-StaticSupportStatus {
    param(
        [string]$Stage,
        [hashtable]$StaticSummaryByComparison
    )
    $comparisonId = Get-StageComparisonId $Stage
    if ($comparisonId.Length -eq 0 -or -not $StaticSummaryByComparison.ContainsKey($comparisonId)) {
        return "unknown"
    }
    $summary = $StaticSummaryByComparison[$comparisonId]
    if ($summary.decision -eq "success" -and [int]$summary.k5_success_seed_count -eq 3 -and [int]$summary.k10_success_seed_count -eq 3) {
        return "explain"
    }
    if ($summary.decision -eq "success") {
        return "watch"
    }
    return "regress"
}

function Get-OverNeutralizationStatus {
    param(
        [bool]$StaticOverNeutralization,
        [bool]$RootOverNeutralization
    )
    if ($RootOverNeutralization) {
        return "regress"
    }
    if ($StaticOverNeutralization) {
        return "watch"
    }
    return "explain"
}

function Get-ExplanationScope {
    param(
        [string]$Stage,
        [string]$GuardStatus,
        [bool]$RootOverNeutralization,
        [string]$CandidateBestMove,
        [string]$CostStatus
    )
    if ($Stage -eq "cp2" -and $GuardStatus -eq "explain") {
        return "cp2_first_drift;root_order_inflation"
    }
    if ($Stage -eq "cp2" -and $GuardStatus -eq "watch") {
        return "cp2_first_drift;static_over_neutralization_watch"
    }
    if ($Stage -eq "final" -and $RootOverNeutralization) {
        return "final_side_effect;root_over_neutralization"
    }
    if ($Stage -eq "final" -and $CandidateBestMove -eq $FinalDriftMove) {
        if ($CostStatus -eq "watch") {
            return "root_cost_partial;final_partial_explanation"
        }
        return "final_partial_explanation"
    }
    return "insufficient_evidence"
}

function New-OutputFileRecord {
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
    }
}

$ProjectRoot = Get-ExistingFullPath $ProjectRoot
$RunRoot = Get-ExistingFullPath $RunRoot
$OutputDir = Get-FullPath $OutputDir
$OpeningDiagnosticsDir = Join-Path $RunRoot "opening-diagnostics"
$Phase37Dir = Join-Path $OpeningDiagnosticsDir "phase3_7_static_ablation"
$Phase39Dir = Join-Path $OpeningDiagnosticsDir "phase3_9_offline_neutralization"
$ScriptPath = $PSCommandPath
if ([string]::IsNullOrWhiteSpace($ScriptPath)) {
    $ScriptPath = Join-Path $ProjectRoot "tools\opening-guard-diagnostics.ps1"
}
$ScriptPath = Get-FullPath $ScriptPath

$inputFiles = @(
    Read-RequiredFile (Join-Path $ProjectRoot "task_plan.md") "task_plan",
    Read-RequiredFile (Join-Path $ProjectRoot "findings.md") "findings",
    Read-RequiredFile (Join-Path $ProjectRoot "progress.md") "progress",
    Read-RequiredFile (Join-Path $ProjectRoot "docs\national_competition_technical_roadmap.md") "national_competition_technical_roadmap",
    Read-RequiredFile (Join-Path $RunRoot "weights-manifest.json") "weights_manifest",
    Read-RequiredFile (Join-Path $OpeningDiagnosticsDir "opening-diagnostics-summary.md") "opening_diagnostics_summary_md",
    Read-RequiredFile (Join-Path $OpeningDiagnosticsDir "opening-diagnostics-summary.csv") "opening_diagnostics_summary_csv",
    Read-RequiredFile (Join-Path $Phase37Dir "opening-child-static-ablation-summary.csv") "phase37_static_ablation_summary_csv",
    Read-RequiredFile (Join-Path $Phase37Dir "opening-child-static-ablation-sweep.csv") "phase37_static_ablation_sweep_csv",
    Read-RequiredFile (Join-Path $Phase39Dir "neutralization-manifest.json") "phase39_neutralization_manifest",
    Read-RequiredFile (Join-Path $Phase39Dir "neutralization-summary.csv") "phase39_neutralization_summary_csv",
    Read-RequiredFile (Join-Path $Phase39Dir "neutralization-root-diagnostics.csv") "phase39_neutralization_root_diagnostics_csv"
)

$weightsManifest = Read-RequiredJson (Join-Path $RunRoot "weights-manifest.json") "weights_manifest"
$openingSummary = Read-RequiredCsv (Join-Path $OpeningDiagnosticsDir "opening-diagnostics-summary.csv") "opening_diagnostics_summary_csv"
$staticSummary = Read-RequiredCsv (Join-Path $Phase37Dir "opening-child-static-ablation-summary.csv") "phase37_static_ablation_summary_csv"
$staticSweep = Read-RequiredCsv (Join-Path $Phase37Dir "opening-child-static-ablation-sweep.csv") "phase37_static_ablation_sweep_csv"
$neutralizationManifest = Read-RequiredJson (Join-Path $Phase39Dir "neutralization-manifest.json") "phase39_neutralization_manifest"
$neutralizationSummary = Read-RequiredCsv (Join-Path $Phase39Dir "neutralization-summary.csv") "phase39_neutralization_summary_csv"
$neutralizationRoot = Read-RequiredCsv (Join-Path $Phase39Dir "neutralization-root-diagnostics.csv") "phase39_neutralization_root_diagnostics_csv"

if ($weightsManifest.value.purpose -ne "selection") {
    Fail "weights-manifest purpose must be selection, actual '$($weightsManifest.value.purpose)'"
}
if ($neutralizationManifest.value.mode -ne "report_only_offline_diagnostics") {
    Fail "Phase 3.9 manifest mode must be report_only_offline_diagnostics"
}

$staticSummaryByComparison = @{}
foreach ($row in $staticSummary.rows) {
    $staticSummaryByComparison[$row.comparison_id] = $row
}

$openingBySeedStage = @{}
foreach ($row in $openingSummary.rows) {
    $openingBySeedStage["$($row.seed)|$($row.stage)"] = $row
}

$rootByKey = @{}
foreach ($row in $neutralizationRoot.rows) {
    $rootByKey[(Get-MetricKey $row)] = $row
}

$metrics = New-Object System.Collections.Generic.List[object]
foreach ($row in $neutralizationSummary.rows) {
    $key = Get-MetricKey $row
    if (-not $rootByKey.ContainsKey($key)) {
        Fail "missing neutralization root diagnostics row for key '$key'"
    }
    $root = $rootByKey[$key]
    $seed = [int]$row.seed
    $stage = $row.stage
    $variant = $row.variant
    $originalStageKey = "$seed|$stage"
    if (-not $openingBySeedStage.ContainsKey($originalStageKey)) {
        Fail "missing opening summary row for key '$originalStageKey'"
    }
    $opening = $openingBySeedStage[$originalStageKey]

    $candidateBestMove = $row.neutralized_best_move
    $baselineRank = ConvertTo-NullableIntText $row.neutralized_baseline_order_rank
    $candidateRank = "1"
    $nodesRatio = ConvertTo-NullableDouble $row.nodes_ratio
    $qnodesRatio = ConvertTo-NullableDouble $row.qnodes_ratio
    $originalNodes = ConvertTo-NullableDouble $row.original_nodes
    $neutralizedNodes = ConvertTo-NullableDouble $row.neutralized_nodes
    $originalQnodes = ConvertTo-NullableDouble $row.original_qnodes
    $neutralizedQnodes = ConvertTo-NullableDouble $row.neutralized_qnodes
    $qnodesPerNodeRatio = $null
    if ($null -ne $originalNodes -and $null -ne $neutralizedNodes -and $null -ne $originalQnodes -and $null -ne $neutralizedQnodes -and $originalNodes -gt 0 -and $neutralizedNodes -gt 0) {
        $originalQpn = $originalQnodes / $originalNodes
        $neutralizedQpn = $neutralizedQnodes / $neutralizedNodes
        if ($originalQpn -gt 0) {
            $qnodesPerNodeRatio = $neutralizedQpn / $originalQpn
        }
    }

    $originalRetries = [int]$row.original_aspiration_retries
    $neutralizedRetries = [int]$row.neutralized_aspiration_retries
    $aspirationRetryDelta = $neutralizedRetries - $originalRetries
    $staticOverNeutralization = ConvertTo-BoolValue $row.static_over_neutralization
    $rootOverNeutralization = ConvertTo-BoolValue $row.root_over_neutralization

    $bestMoveStatus = Get-BestMoveStatus $stage $candidateBestMove $rootOverNeutralization
    $rootOrderStatus = Get-RootOrderStatus $stage $baselineRank $candidateRank $rootOverNeutralization
    $staticSupportStatus = Get-StaticSupportStatus $stage $staticSummaryByComparison
    $overNeutralizationStatus = Get-OverNeutralizationStatus $staticOverNeutralization $rootOverNeutralization
    $costStatus = Get-CostStatus $nodesRatio $qnodesRatio $qnodesPerNodeRatio $aspirationRetryDelta $rootOverNeutralization $stage
    $guardStatus = Get-WorstStatus @($bestMoveStatus, $rootOrderStatus, $staticSupportStatus, $overNeutralizationStatus, $costStatus)
    $explanationScope = Get-ExplanationScope $stage $guardStatus $rootOverNeutralization $candidateBestMove $costStatus

    $artifactRole = if ($stage -eq "cp2") { "neutralized_diagnostic_checkpoint" } else { "neutralized_diagnostic_candidate" }

    $metrics.Add([pscustomobject]@{
        seed = $seed
        stage = $stage
        artifact_role = $artifactRole
        variant = $variant
        baseline_best_move = $BaselineMove
        candidate_best_move = $candidateBestMove
        best_move_status = $bestMoveStatus
        baseline_rank = $baselineRank
        candidate_rank = $candidateRank
        root_order_status = $rootOrderStatus
        nodes_ratio = Get-RatioText $nodesRatio
        qnodes_ratio = Get-RatioText $qnodesRatio
        qnodes_per_node_ratio = Get-RatioText $qnodesPerNodeRatio
        aspiration_retry_delta = $aspirationRetryDelta
        static_support_status = $staticSupportStatus
        over_neutralization_status = $overNeutralizationStatus
        guard_status = $guardStatus
        explanation_scope = $explanationScope
        source_best_move = $row.original_best_move
        original_baseline_rank = ConvertTo-NullableIntText $row.original_baseline_order_rank
        source_nodes = ConvertTo-NullableIntText $row.original_nodes
        candidate_nodes = ConvertTo-NullableIntText $row.neutralized_nodes
        source_qnodes = ConvertTo-NullableIntText $row.original_qnodes
        candidate_qnodes = ConvertTo-NullableIntText $row.neutralized_qnodes
        root_cost_status = $costStatus
        static_over_neutralization = $staticOverNeutralization.ToString().ToLowerInvariant()
        root_over_neutralization = $rootOverNeutralization.ToString().ToLowerInvariant()
        original_diagnostic_best_move = $opening.best_move
        original_diagnostic_nodes = $opening.nodes
        original_diagnostic_qnodes = $opening.qnodes
    })
}

if ($metrics.Count -eq 0) {
    Fail "no metrics rows generated"
}

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null
$metricsPath = Join-Path $OutputDir "opening-guard-metrics.csv"
$summaryPath = Join-Path $OutputDir "opening-guard-summary.json"
$manifestPath = Join-Path $OutputDir "opening-guard-manifest.json"

$metrics | Export-Csv -LiteralPath $metricsPath -NoTypeInformation -Encoding UTF8

$stageStatusCounts = @()
foreach ($group in ($metrics | Group-Object stage, guard_status)) {
    $stageStatusCounts += [pscustomobject]@{
        stage = $group.Group[0].stage
        guard_status = $group.Group[0].guard_status
        count = $group.Count
    }
}

$variantStatusRows = @()
foreach ($group in ($metrics | Group-Object variant)) {
    $variantGuardStatus = Get-WorstStatus @($group.Group | ForEach-Object { $_.guard_status })
    $variantStatusRows += [pscustomobject]@{
        variant = $group.Name
        guard_status = $variantGuardStatus
        explain = @($group.Group | Where-Object { $_.guard_status -eq "explain" }).Count
        watch = @($group.Group | Where-Object { $_.guard_status -eq "watch" }).Count
        regress = @($group.Group | Where-Object { $_.guard_status -eq "regress" }).Count
        unknown = @($group.Group | Where-Object { $_.guard_status -eq "unknown" }).Count
    }
}

$cp2Rows = @($metrics | Where-Object { $_.stage -eq "cp2" })
$finalRows = @($metrics | Where-Object { $_.stage -eq "final" })
$cp2StrongExplanation = (
    @($cp2Rows | Where-Object { $_.best_move_status -eq "explain" -and $_.root_order_status -eq "explain" }).Count -ge 6
)
$finalPartialExplanation = (
    @($finalRows | Where-Object { $_.guard_status -eq "regress" }).Count -gt 0 -and
    @($finalRows | Where-Object { $_.explanation_scope -like "*final_partial_explanation*" -or $_.explanation_scope -like "*root_cost_partial*" }).Count -gt 0
)

$summary = [pscustomobject]@{
    phase = "Phase 3.13-B opening guard diagnostics"
    mode = "report_only_opening_guard"
    generated_at = (Get-Date).ToUniversalTime().ToString("o")
    git_commit = $weightsManifest.value.git_commit
    output_dir = $OutputDir
    metrics_count = $metrics.Count
    status_semantics = $AllowedStatuses
    production_candidate = $false
    selection_gate_eligible = $false
    report_only = $true
    phase3_status = "not_passed"
    phase4_status = "blocked"
    stage_status_counts = $stageStatusCounts
    variant_guard_status = $variantStatusRows
    cp2_strong_explanation_verified = $cp2StrongExplanation
    final_partial_explanation_verified = $finalPartialExplanation
    notes = @(
        "Report-only diagnostics only; not a selection gate.",
        "Guard statuses do not modify overall_rating, strength_rating, or perf_rating.",
        "No training, no selection, no GamesPerSeed=64, no weight generation."
    )
}
$summary | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $summaryPath -Encoding UTF8

$outputFiles = @(
    New-OutputFileRecord $metricsPath "opening_guard_metrics_csv",
    New-OutputFileRecord $summaryPath "opening_guard_summary_json"
)

$manifest = [pscustomobject]@{
    phase = "Phase 3.13-B opening guard diagnostics"
    mode = "report_only_opening_guard"
    generated_at = (Get-Date).ToUniversalTime().ToString("o")
    git_commit = $weightsManifest.value.git_commit
    script_path = $ScriptPath
    execution_command = "pwsh -NoProfile -File .\tools\opening-guard-diagnostics.ps1"
    project_root = $ProjectRoot
    run_root = $RunRoot
    output_dir = $OutputDir
    input_files = $inputFiles
    output_files = $outputFiles
    source_row_counts = [pscustomobject]@{
        opening_diagnostics_summary = $openingSummary.rows.Count
        phase37_static_summary = $staticSummary.rows.Count
        phase37_static_sweep = $staticSweep.rows.Count
        phase39_neutralization_summary = $neutralizationSummary.rows.Count
        phase39_neutralization_root_diagnostics = $neutralizationRoot.rows.Count
    }
    output_row_counts = [pscustomobject]@{
        opening_guard_metrics = $metrics.Count
    }
    production_candidate = $false
    selection_gate_eligible = $false
    report_only = $true
    phase3_status = "not_passed"
    phase4_status = "blocked"
    status_semantics = $AllowedStatuses
    guard_scope = @(
        "opening_best_move_guard",
        "root_order_guard",
        "root_cost_guard",
        "static_drift_guard",
        "final_candidate_side_effect_guard"
    )
}
$manifest | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $manifestPath -Encoding UTF8

Write-Host "[opening-guard-diagnostics] wrote $metricsPath"
Write-Host "[opening-guard-diagnostics] wrote $summaryPath"
Write-Host "[opening-guard-diagnostics] wrote $manifestPath"
Write-Host "[opening-guard-diagnostics] metrics_count=$($metrics.Count)"
