[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$RunRoot,

    [string]$GuardOutputDir,

    [string]$RunnerOutputDir,

    [string]$OpeningGuardScript = ".\tools\opening-guard-diagnostics.ps1",

    [switch]$Force
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$AllowedStatuses = @("explain", "watch", "regress", "unknown")

function Fail {
    param([string]$Message)
    throw "[candidate-acceptance-guard-runner] $Message"
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

function Read-RequiredJson {
    param(
        [string]$Path,
        [string]$Label
    )
    $fullPath = Get-ExistingFullPath $Path
    $content = Get-Content -LiteralPath $fullPath -Raw
    try {
        $value = $content | ConvertFrom-Json
    } catch {
        Fail "required JSON '$Label' is invalid: $fullPath"
    }
    return [pscustomobject]@{
        label = $Label
        path = $fullPath
        sha256 = Get-FileSha256 $fullPath
        value = $value
    }
}

function New-FileRecord {
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

function Get-StatusCountObject {
    param(
        [object[]]$Rows,
        [string]$Stage
    )
    $result = [ordered]@{}
    foreach ($status in $AllowedStatuses) {
        $result[$status] = 0
    }
    foreach ($row in @($Rows | Where-Object { $_.stage -eq $Stage })) {
        $status = "$($row.guard_status)"
        if ($AllowedStatuses -notcontains $status) {
            $status = "unknown"
        }
        $result[$status] = [int]$row.count
    }
    return [pscustomobject]$result
}

function Get-DiscoveredSelectionSummary {
    param([string]$Root)
    $preferred = Join-Path $Root "selection-summary.json"
    if (Test-Path -LiteralPath $preferred -PathType Leaf) {
        return (Get-ExistingFullPath $preferred)
    }

    $matches = @(
        Get-ChildItem -LiteralPath $Root -Recurse -File -Filter "selection-summary.json" |
            Where-Object { $_.FullName -notlike "*\opening-diagnostics\*" } |
            Sort-Object FullName
    )
    if ($matches.Count -eq 1) {
        return $matches[0].FullName
    }
    if ($matches.Count -gt 1) {
        Fail "multiple selection summary artifacts discovered; pass a RunRoot with a single selection-summary.json"
    }
    Fail "selection-summary.json not found under RunRoot; refusing to fabricate candidate acceptance result"
}

function Write-JsonFile {
    param(
        [object]$Value,
        [string]$Path,
        [int]$Depth = 12
    )
    $Value | ConvertTo-Json -Depth $Depth | Set-Content -LiteralPath $Path -Encoding UTF8
}

$ScriptPath = $PSCommandPath
if ([string]::IsNullOrWhiteSpace($ScriptPath)) {
    $ScriptPath = Join-Path (Get-Location).ProviderPath "tools\candidate-acceptance-guard-runner.ps1"
}
$ScriptPath = Get-FullPath $ScriptPath
$ProjectRoot = Split-Path -Parent (Split-Path -Parent $ScriptPath)

$RunRoot = Get-ExistingFullPath $RunRoot
if ([string]::IsNullOrWhiteSpace($GuardOutputDir)) {
    $GuardOutputDir = Join-Path $RunRoot "opening-diagnostics\phase3_13_opening_guard"
}
if ([string]::IsNullOrWhiteSpace($RunnerOutputDir)) {
    $RunnerOutputDir = Join-Path $RunRoot "opening-diagnostics\phase3_16_guard_runner"
}
$GuardOutputDir = Get-FullPath $GuardOutputDir
$RunnerOutputDir = Get-FullPath $RunnerOutputDir

if (-not [System.IO.Path]::IsPathRooted($OpeningGuardScript)) {
    $OpeningGuardScript = Join-Path (Get-Location).ProviderPath $OpeningGuardScript
}
$OpeningGuardScript = Get-ExistingFullPath $OpeningGuardScript

if ($RunnerOutputDir -eq $GuardOutputDir) {
    Fail "RunnerOutputDir must be separate from GuardOutputDir"
}
if ($RunnerOutputDir -eq $RunRoot) {
    Fail "RunnerOutputDir must not be RunRoot"
}
if ((Test-Path -LiteralPath $RunnerOutputDir) -and -not $Force.IsPresent) {
    Fail "RunnerOutputDir already exists; pass -Force to overwrite runner report files only: $RunnerOutputDir"
}

$selectionSummaryPath = Get-DiscoveredSelectionSummary $RunRoot
$selectionSummaryTxtPath = Join-Path $RunRoot "selection-summary.txt"
$selectionSummary = Read-RequiredJson $selectionSummaryPath "selection_summary_json"
if ($null -eq $selectionSummary.value.scorecard) {
    Fail "selection summary has no scorecard object; refusing to fabricate scorecard artifact"
}
$selectionSummaryHashBefore = Get-FileSha256 $selectionSummary.path

$selectionInputFiles = @()
$selectionInputFiles += (New-FileRecord -Path $selectionSummary.path -Label "selection_summary_json")
if (Test-Path -LiteralPath $selectionSummaryTxtPath -PathType Leaf) {
    $selectionInputFiles += (New-FileRecord -Path $selectionSummaryTxtPath -Label "selection_summary_txt")
}

$guardSummaryPath = Join-Path $GuardOutputDir "opening-guard-summary.json"
$guardManifestPath = Join-Path $GuardOutputDir "opening-guard-manifest.json"
$guardMetricsPath = Join-Path $GuardOutputDir "opening-guard-metrics.csv"
$guardExecutionStatus = "reused_existing"
$guardInvocationOutput = @()
$guardCommandLine = "pwsh -NoProfile -ExecutionPolicy Bypass -File `"$OpeningGuardScript`" -RunRoot `"$RunRoot`" -OutputDir `"$GuardOutputDir`""

if ((Test-Path -LiteralPath $guardSummaryPath -PathType Leaf) -and
    (Test-Path -LiteralPath $guardManifestPath -PathType Leaf)) {
    $guardExecutionStatus = "reused_existing"
} else {
    if (Test-Path -LiteralPath $GuardOutputDir) {
        $existingChildren = @(Get-ChildItem -LiteralPath $GuardOutputDir -Force)
        if ($existingChildren.Count -gt 0) {
            Fail "GuardOutputDir exists but required guard artifacts are missing; refusing to overwrite existing guard output: $GuardOutputDir"
        }
    }
    $guardExecutionStatus = "generated_by_opening_guard_script"
    $guardInvocationOutput = @(& $OpeningGuardScript -RunRoot $RunRoot -OutputDir $GuardOutputDir 2>&1)
}

if (-not (Test-Path -LiteralPath $guardSummaryPath -PathType Leaf)) {
    Fail "opening-guard-summary.json missing after guard step: $guardSummaryPath"
}
if (-not (Test-Path -LiteralPath $guardManifestPath -PathType Leaf)) {
    Fail "opening-guard-manifest.json missing after guard step: $guardManifestPath"
}
if (-not (Test-Path -LiteralPath $guardMetricsPath -PathType Leaf)) {
    Fail "opening-guard-metrics.csv missing after guard step: $guardMetricsPath"
}

$openingGuardSummary = Read-RequiredJson $guardSummaryPath "opening_guard_summary_json"
$openingGuardManifest = Read-RequiredJson $guardManifestPath "opening_guard_manifest_json"
$guardMetrics = @(Import-Csv -LiteralPath $guardMetricsPath)
if ($guardMetrics.Count -eq 0) {
    Fail "opening-guard-metrics.csv has no rows: $guardMetricsPath"
}

if ($openingGuardSummary.value.report_only -ne $true -or
    $openingGuardSummary.value.selection_gate_eligible -ne $false -or
    $openingGuardSummary.value.production_candidate -ne $false) {
    Fail "opening guard summary does not declare report-only non-candidate boundaries"
}

New-Item -ItemType Directory -Force -Path $RunnerOutputDir | Out-Null

$selectionSummaryHashAfter = Get-FileSha256 $selectionSummary.path
$selectionResultUnchanged = ($selectionSummaryHashBefore -eq $selectionSummaryHashAfter)
$gateNotModified = $selectionResultUnchanged

$cp2Counts = Get-StatusCountObject $openingGuardSummary.value.stage_status_counts "cp2"
$finalCounts = Get-StatusCountObject $openingGuardSummary.value.stage_status_counts "final"
$variantStatuses = @($openingGuardSummary.value.variant_guard_status | Sort-Object variant)

$riskNotes = @(
    "Report-only wrapper only; it does not modify selection-summary.json, selection-summary.txt, scorecard, overall_rating, strength_rating, or perf_rating.",
    "Phase 3.1 selection remains regress; Phase 3 status remains not_passed.",
    "Phase 4 remains blocked.",
    "cp2 has strong report-only explanation, but final remains partial and has regress side-effect risk.",
    "All variant aggregate guard_status values remain regress because final side-effect risk is present."
)

$summaryObject = [pscustomobject]@{
    phase = "Phase 3.16 candidate acceptance guard runner"
    mode = "report_only_candidate_acceptance_guard"
    generated_at = (Get-Date).ToUniversalTime().ToString("o")
    run_root = $RunRoot
    guard_output_dir = $GuardOutputDir
    runner_output_dir = $RunnerOutputDir
    guard_execution_status = $guardExecutionStatus
    selection_summary_path = $selectionSummary.path
    selection_summary_sha256_before = $selectionSummaryHashBefore
    selection_summary_sha256_after = $selectionSummaryHashAfter
    selection_result_unchanged = $selectionResultUnchanged
    gate_not_modified = $gateNotModified
    production_candidate = $false
    selection_gate_eligible = $false
    report_only = $true
    phase3_status = "not_passed"
    phase4_status = "blocked"
    selection = [pscustomobject]@{
        overall_rating = $selectionSummary.value.overall_rating
        strength_rating = $selectionSummary.value.strength_rating
        perf_rating = $selectionSummary.value.perf_rating
        scorecard = $selectionSummary.value.scorecard
        totals = $selectionSummary.value.totals
        games_per_seed = $selectionSummary.value.games_per_seed
        seeds = $selectionSummary.value.seeds
    }
    opening_guard = [pscustomobject]@{
        summary_path = $openingGuardSummary.path
        manifest_path = $openingGuardManifest.path
        metrics_path = (Get-ExistingFullPath $guardMetricsPath)
        cp2 = $cp2Counts
        final = $finalCounts
        variant_statuses = $variantStatuses
        cp2_strong_explanation_verified = $openingGuardSummary.value.cp2_strong_explanation_verified
        final_partial_explanation_verified = $openingGuardSummary.value.final_partial_explanation_verified
    }
    risk_notes = $riskNotes
}

$summaryJsonPath = Join-Path $RunnerOutputDir "candidate-acceptance-guard-summary.json"
$summaryMdPath = Join-Path $RunnerOutputDir "candidate-acceptance-guard-summary.md"
$manifestPath = Join-Path $RunnerOutputDir "candidate-acceptance-guard-manifest.json"

Write-JsonFile $summaryObject $summaryJsonPath

$variantLines = @()
foreach ($variant in $variantStatuses) {
    $variantLines += "| $($variant.variant) | $($variant.guard_status) | $($variant.explain) | $($variant.watch) | $($variant.regress) | $($variant.unknown) |"
}

$summaryMd = @(
    "# Phase 3.16 Candidate Acceptance Guard Runner",
    "",
    "## Boundary",
    "",
    "- report_only=true",
    "- selection_gate_eligible=false",
    "- production_candidate=false",
    "- selection_result_unchanged=$($selectionResultUnchanged.ToString().ToLowerInvariant())",
    "- gate_not_modified=$($gateNotModified.ToString().ToLowerInvariant())",
    "- phase3_status=not_passed",
    "- phase4_status=blocked",
    "",
    "## Selection Result",
    "",
    "- overall_rating=$($selectionSummary.value.overall_rating)",
    "- strength_rating=$($selectionSummary.value.strength_rating)",
    "- perf_rating=$($selectionSummary.value.perf_rating)",
    "- scorecard_pass=$($selectionSummary.value.scorecard.pass)",
    "- scorecard_watch=$($selectionSummary.value.scorecard.watch)",
    "- scorecard_regress=$($selectionSummary.value.scorecard.regress)",
    "",
    "## Opening Guard Counts",
    "",
    "| stage | explain | watch | regress | unknown |",
    "|---|---:|---:|---:|---:|",
    "| cp2 | $($cp2Counts.explain) | $($cp2Counts.watch) | $($cp2Counts.regress) | $($cp2Counts.unknown) |",
    "| final | $($finalCounts.explain) | $($finalCounts.watch) | $($finalCounts.regress) | $($finalCounts.unknown) |",
    "",
    "## Variant Statuses",
    "",
    "| variant | guard_status | explain | watch | regress | unknown |",
    "|---|---|---:|---:|---:|---:|"
) + $variantLines + @(
    "",
    "## Risk Notes",
    "",
    "- Report-only wrapper only; selection result and gate fields are unchanged.",
    "- cp2 explanation is strong, but final remains partial and includes regress side-effect risk.",
    "- All variant aggregate statuses remain regress, so this report cannot be treated as candidate acceptance.",
    "- Phase 3 remains not passed; Phase 4 remains blocked.",
    "",
    "## Inputs",
    "",
    "- selection_summary=$($selectionSummary.path)",
    "- opening_guard_summary=$($openingGuardSummary.path)",
    "- opening_guard_manifest=$($openingGuardManifest.path)",
    "- opening_guard_metrics=$(Get-ExistingFullPath $guardMetricsPath)",
    "",
    "## Guard Execution",
    "",
    "- status=$guardExecutionStatus",
    "- command=$guardCommandLine"
)
$summaryMd | Set-Content -LiteralPath $summaryMdPath -Encoding UTF8

$outputFilesWithoutManifest = @()
$outputFilesWithoutManifest += (New-FileRecord -Path $summaryMdPath -Label "candidate_acceptance_guard_summary_md")
$outputFilesWithoutManifest += (New-FileRecord -Path $summaryJsonPath -Label "candidate_acceptance_guard_summary_json")

$gitCommit = (& git -C $ProjectRoot rev-parse HEAD).Trim()
$commandLine = "pwsh -NoProfile -ExecutionPolicy Bypass -File `"$ScriptPath`" -RunRoot `"$RunRoot`" -GuardOutputDir `"$GuardOutputDir`" -RunnerOutputDir `"$RunnerOutputDir`" -OpeningGuardScript `"$OpeningGuardScript`""
if ($Force.IsPresent) {
    $commandLine += " -Force"
}

$manifestInputFiles = @()
$manifestInputFiles += (New-FileRecord -Path $ScriptPath -Label "candidate_acceptance_guard_runner_script")
$manifestInputFiles += (New-FileRecord -Path $OpeningGuardScript -Label "opening_guard_script")
$manifestInputFiles += (New-FileRecord -Path $guardSummaryPath -Label "opening_guard_summary_json")
$manifestInputFiles += (New-FileRecord -Path $guardManifestPath -Label "opening_guard_manifest_json")
$manifestInputFiles += (New-FileRecord -Path $guardMetricsPath -Label "opening_guard_metrics_csv")
$manifestInputFiles += $selectionInputFiles

$manifest = [pscustomobject]@{
    phase = "Phase 3.16 candidate acceptance guard runner"
    mode = "report_only_candidate_acceptance_guard"
    generated_at = (Get-Date).ToUniversalTime().ToString("o")
    git_commit = $gitCommit
    script_path = $ScriptPath
    opening_guard_script_path = $OpeningGuardScript
    command_line = $commandLine
    opening_guard_command_line = $guardCommandLine
    project_root = $ProjectRoot
    run_root = $RunRoot
    guard_output_dir = $GuardOutputDir
    runner_output_dir = $RunnerOutputDir
    guard_execution_status = $guardExecutionStatus
    selection_result_unchanged = $selectionResultUnchanged
    gate_not_modified = $gateNotModified
    production_candidate = $false
    selection_gate_eligible = $false
    report_only = $true
    phase3_status = "not_passed"
    phase4_status = "blocked"
    input_files = $manifestInputFiles
    output_files = $outputFilesWithoutManifest
    self_manifest_path = $manifestPath
    self_manifest_sha256_note = "Compute after file generation; self hash is intentionally not embedded to avoid stale self-reference."
    guard_invocation_output = @($guardInvocationOutput | ForEach-Object { "$_" })
}
Write-JsonFile $manifest $manifestPath

Write-Host "[candidate-acceptance-guard-runner] wrote $summaryMdPath"
Write-Host "[candidate-acceptance-guard-runner] wrote $summaryJsonPath"
Write-Host "[candidate-acceptance-guard-runner] wrote $manifestPath"
Write-Host "[candidate-acceptance-guard-runner] guard_execution_status=$guardExecutionStatus"
Write-Host "[candidate-acceptance-guard-runner] selection_result_unchanged=$($selectionResultUnchanged.ToString().ToLowerInvariant())"
Write-Host "[candidate-acceptance-guard-runner] gate_not_modified=$($gateNotModified.ToString().ToLowerInvariant())"
