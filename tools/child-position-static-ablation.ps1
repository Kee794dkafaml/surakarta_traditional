[CmdletBinding()]
param(
    [string]$AttributionDir = ".\build-local\training\20260522-172134\opening-diagnostics\phase3_5_attribution",

    [string]$OutputDir = ".\build-local\training\20260522-172134\opening-diagnostics\phase3_7_static_ablation",

    [string[]]$Seeds = @("20260423", "20260424", "20260425"),

    [string[]]$TopK = @("1", "3", "5", "10", "20", "all_negative")
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$ExpectedSummaryRows = 27
$ExpectedTupleRows = 1215
$ExpectedTopDeltaRows = 1440
$ExpectedActiveTupleCount = 45
$ExpectedTupleSetHash = "0x1D4A6EBF2BAF6484"
$ExpectedWeightCount = 16605
$RequiredTopK = @("1", "3", "5", "10", "20", "all_negative")

function Fail {
    param([string]$Message)
    throw "[child-position-static-ablation] $Message"
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
    param([string]$Path)
    $fullPath = Get-ExistingFullPath $Path
    $item = Get-Item -LiteralPath $fullPath
    return [pscustomobject]@{
        path = $fullPath
        sha256 = Get-FileSha256 $fullPath
        length = $item.Length
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

function Assert-Equal {
    param(
        [object]$Actual,
        [object]$Expected,
        [string]$Label
    )
    if ($Actual -ne $Expected) {
        Fail "$Label mismatch: actual='$Actual' expected='$Expected'"
    }
}

function ConvertTo-IntValue {
    param([object]$Value)
    return [int]$Value
}

function ConvertTo-DoubleValue {
    param([object]$Value)
    return [double]$Value
}

function Get-Sum {
    param(
        [object[]]$Rows,
        [string]$PropertyName
    )
    $sum = 0
    foreach ($row in $Rows) {
        $sum += [int]$row.$PropertyName
    }
    return $sum
}

function Get-RatioText {
    param([double]$Value)
    return ([math]::Round($Value, 6)).ToString("0.######", [System.Globalization.CultureInfo]::InvariantCulture)
}

function Get-TopKRankValue {
    param([string]$Label)
    if ($Label -eq "all_negative") {
        return 999999
    }
    return [int]$Label
}

function Get-TopKMembership {
    param(
        [int]$Rank,
        [string[]]$TopKValues
    )
    $members = New-Object System.Collections.Generic.List[string]
    foreach ($k in $TopKValues) {
        if ($k -eq "all_negative") {
            $members.Add($k)
        } elseif ($Rank -le [int]$k) {
            $members.Add($k)
        }
    }
    return ($members.ToArray() -join "|")
}

function New-ComparisonDefinitions {
    return @(
        [pscustomobject]@{
            comparison_id = "C1_cp2_onset"
            comparison_kind = "main"
            stage = "cp2"
            from_label = "baseline"
            to_label = "cp2"
            purpose = "checkpoint-2 first drift"
            is_control = $false
        },
        [pscustomobject]@{
            comparison_id = "C2_final_gate_blocker"
            comparison_kind = "main"
            stage = "final"
            from_label = "baseline"
            to_label = "final"
            purpose = "final selection blocker"
            is_control = $false
        },
        [pscustomobject]@{
            comparison_id = "C3_final_cp2_residue"
            comparison_kind = "auxiliary"
            stage = "final"
            from_label = "baseline"
            to_label = "cp2"
            purpose = "early drift residue in final weights"
            is_control = $false
        },
        [pscustomobject]@{
            comparison_id = "C4_final_transition"
            comparison_kind = "auxiliary"
            stage = "final"
            from_label = "cp2"
            to_label = "final"
            purpose = "transition from cp2 drift child to final drift child"
            is_control = $false
        },
        [pscustomobject]@{
            comparison_id = "N1_bootstrap_cp2_sanity"
            comparison_kind = "control"
            stage = "bootstrap"
            from_label = "baseline"
            to_label = "cp2"
            purpose = "bootstrap baseline-to-cp2 sanity control"
            is_control = $true
        },
        [pscustomobject]@{
            comparison_id = "N1_bootstrap_final_sanity"
            comparison_kind = "control"
            stage = "bootstrap"
            from_label = "baseline"
            to_label = "final"
            purpose = "bootstrap baseline-to-final sanity control"
            is_control = $true
        },
        [pscustomobject]@{
            comparison_id = "N2_cp2_final_negative_control"
            comparison_kind = "control"
            stage = "cp2"
            from_label = "baseline"
            to_label = "final"
            purpose = "cp2 final-drift negative control"
            is_control = $true
        }
    )
}

function Get-Key {
    param(
        [object]$Seed,
        [string]$Stage,
        [string]$RootMoveLabel
    )
    return "$Seed|$Stage|$RootMoveLabel"
}

function Get-TupleKey {
    param(
        [object]$Seed,
        [string]$Stage,
        [string]$RootMoveLabel,
        [object]$TupleId
    )
    return "$Seed|$Stage|$RootMoveLabel|$TupleId"
}

function Get-SummaryRow {
    param(
        [hashtable]$SummaryByKey,
        [string]$Seed,
        [string]$Stage,
        [string]$RootMoveLabel
    )
    $key = Get-Key $Seed $Stage $RootMoveLabel
    if (-not $SummaryByKey.ContainsKey($key)) {
        Fail "missing summary row for $key"
    }
    return $SummaryByKey[$key]
}

function Get-TupleRow {
    param(
        [hashtable]$TupleByKey,
        [string]$Seed,
        [string]$Stage,
        [string]$RootMoveLabel,
        [int]$TupleId
    )
    $key = Get-TupleKey $Seed $Stage $RootMoveLabel $TupleId
    if (-not $TupleByKey.ContainsKey($key)) {
        Fail "missing tuple row for $key"
    }
    return $TupleByKey[$key]
}

function Get-KResult {
    param(
        [bool]$IsControl,
        [int]$OriginalStagePairDelta,
        [int]$AblatedStagePairDelta,
        [double]$StageDriftReductionRatio,
        [string]$TopKLabel
    )
    if ($IsControl) {
        if ($OriginalStagePairDelta -ge 0 -and $AblatedStagePairDelta -ge 0) {
            return "success"
        }
        return "failure"
    }

    if ($OriginalStagePairDelta -ge 0) {
        return "failure"
    }

    if ($AblatedStagePairDelta -ge 0 -or $StageDriftReductionRatio -ge 0.6) {
        return "success"
    }

    if (($TopKLabel -eq "20" -or $TopKLabel -eq "all_negative") -and $StageDriftReductionRatio -lt 0.3) {
        return "failure"
    }

    return "inconclusive"
}

function Get-StableTupleList {
    param(
        [object[]]$Rows,
        [int]$MinSeedCount,
        [int]$MaxRank
    )
    $seen = @{}
    foreach ($row in $Rows) {
        if ([int]$row.rank -gt $MaxRank) {
            continue
        }
        $key = "$($row.tuple_id)|$($row.seed)"
        if (-not $seen.ContainsKey($key)) {
            $seen[$key] = $row
        }
    }

    $counts = @{}
    foreach ($entry in $seen.Values) {
        $tupleId = [string]$entry.tuple_id
        if (-not $counts.ContainsKey($tupleId)) {
            $counts[$tupleId] = 0
        }
        $counts[$tupleId] += 1
    }

    $stable = New-Object System.Collections.Generic.List[int]
    foreach ($tupleId in $counts.Keys) {
        if ($counts[$tupleId] -ge $MinSeedCount) {
            $stable.Add([int]$tupleId)
        }
    }
    return (($stable.ToArray() | Sort-Object) -join ",")
}

$SeedValues = Convert-ListArgs $Seeds
$TopKValues = Convert-ListArgs $TopK
foreach ($k in $RequiredTopK) {
    if (@($TopKValues) -notcontains $k) {
        Fail "TopK must include '$k'"
    }
}
foreach ($k in $TopKValues) {
    if ($k -ne "all_negative") {
        [void][int]$k
    }
}

$repoRoot = Get-FullPath (Join-Path $PSScriptRoot "..")
$attributionDirPath = Get-ExistingFullPath $AttributionDir
$outputDirPath = Get-FullPath $OutputDir
New-Item -ItemType Directory -Path $outputDirPath -Force | Out-Null

$scriptPath = Get-ExistingFullPath $PSCommandPath
$summaryCsvPath = Join-Path $attributionDirPath "opening-child-attribution-summary.csv"
$tupleCsvPath = Join-Path $attributionDirPath "opening-child-attribution-tuples.csv"
$topDeltasCsvPath = Join-Path $attributionDirPath "opening-child-attribution-top-deltas.csv"
$attributionManifestPath = Join-Path $attributionDirPath "attribution-manifest.json"

$inputFiles = [ordered]@{
    attribution_summary_csv = Read-RequiredFile $summaryCsvPath
    attribution_tuple_csv = Read-RequiredFile $tupleCsvPath
    attribution_top_deltas_csv = Read-RequiredFile $topDeltasCsvPath
    attribution_manifest_json = Read-RequiredFile $attributionManifestPath
    task_plan = Read-RequiredFile (Join-Path $repoRoot "task_plan.md")
    findings = Read-RequiredFile (Join-Path $repoRoot "findings.md")
    progress = Read-RequiredFile (Join-Path $repoRoot "progress.md")
    national_competition_technical_roadmap = Read-RequiredFile (Join-Path $repoRoot "docs\national_competition_technical_roadmap.md")
}

$summaryRows = @(Import-Csv -LiteralPath $inputFiles.attribution_summary_csv.path)
$tupleInputRows = @(Import-Csv -LiteralPath $inputFiles.attribution_tuple_csv.path)
$topDeltaRows = @(Import-Csv -LiteralPath $inputFiles.attribution_top_deltas_csv.path)
$attributionManifest = Get-Content -LiteralPath $inputFiles.attribution_manifest_json.path -Raw | ConvertFrom-Json

Assert-Equal $summaryRows.Count $ExpectedSummaryRows "attribution summary row count"
Assert-Equal $tupleInputRows.Count $ExpectedTupleRows "attribution tuple row count"
Assert-Equal $topDeltaRows.Count $ExpectedTopDeltaRows "attribution top-deltas row count"
Assert-Equal $attributionManifest.tuple_set_hash $ExpectedTupleSetHash "attribution manifest tuple_set_hash"
Assert-Equal $attributionManifest.weight_count $ExpectedWeightCount "attribution manifest weight_count"
Assert-Equal $attributionManifest.checks.summary_row_count $ExpectedSummaryRows "attribution manifest summary row count"
Assert-Equal $attributionManifest.checks.tuple_detail_row_count $ExpectedTupleRows "attribution manifest tuple detail row count"
Assert-Equal $attributionManifest.checks.active_tuple_count_all_rows $ExpectedActiveTupleCount "attribution manifest active tuple count"

$summaryByKey = @{}
foreach ($row in $summaryRows) {
    Assert-Equal $row.active_tuple_count $ExpectedActiveTupleCount "active_tuple_count seed=$($row.seed) stage=$($row.stage) root=$($row.root_move_label)"
    Assert-Equal $row.tuple_set_hash $ExpectedTupleSetHash "tuple_set_hash seed=$($row.seed) stage=$($row.stage) root=$($row.root_move_label)"
    Assert-Equal ([int]$row.weight_count) $ExpectedWeightCount "weight_count seed=$($row.seed) stage=$($row.stage) root=$($row.root_move_label)"
    $key = Get-Key $row.seed $row.stage $row.root_move_label
    if ($summaryByKey.ContainsKey($key)) {
        Fail "duplicate summary key $key"
    }
    $summaryByKey[$key] = $row
}

$tupleByKey = @{}
foreach ($row in $tupleInputRows) {
    $key = Get-TupleKey $row.seed $row.stage $row.root_move_label $row.tuple_id
    if ($tupleByKey.ContainsKey($key)) {
        Fail "duplicate tuple key $key"
    }
    $tupleByKey[$key] = $row
}

$comparisonDefinitions = New-ComparisonDefinitions
$tupleOutputRows = New-Object System.Collections.Generic.List[object]
$sweepRows = New-Object System.Collections.Generic.List[object]
$comparisonSeedFacts = New-Object System.Collections.Generic.List[object]

foreach ($comparison in $comparisonDefinitions) {
    foreach ($seed in $SeedValues) {
        $fromStageSummary = Get-SummaryRow $summaryByKey $seed $comparison.stage $comparison.from_label
        $toStageSummary = Get-SummaryRow $summaryByKey $seed $comparison.stage $comparison.to_label
        $fromBootstrapSummary = Get-SummaryRow $summaryByKey $seed "bootstrap" $comparison.from_label
        $toBootstrapSummary = Get-SummaryRow $summaryByKey $seed "bootstrap" $comparison.to_label

        $stagePairDelta = (ConvertTo-IntValue $toStageSummary.total_static_score) - (ConvertTo-IntValue $fromStageSummary.total_static_score)
        $bootstrapPairDelta = (ConvertTo-IntValue $toBootstrapSummary.total_static_score) - (ConvertTo-IntValue $fromBootstrapSummary.total_static_score)
        $trainingInducedPairDelta = $stagePairDelta - $bootstrapPairDelta

        $allTupleRows = New-Object System.Collections.Generic.List[object]
        for ($tupleId = 0; $tupleId -lt $ExpectedActiveTupleCount; $tupleId++) {
            $fromStageTuple = Get-TupleRow $tupleByKey $seed $comparison.stage $comparison.from_label $tupleId
            $toStageTuple = Get-TupleRow $tupleByKey $seed $comparison.stage $comparison.to_label $tupleId
            $fromBootstrapTuple = Get-TupleRow $tupleByKey $seed "bootstrap" $comparison.from_label $tupleId
            $toBootstrapTuple = Get-TupleRow $tupleByKey $seed "bootstrap" $comparison.to_label $tupleId

            $fromStageDelta = (ConvertTo-IntValue $fromStageTuple.side_to_move_contribution) - (ConvertTo-IntValue $fromBootstrapTuple.side_to_move_contribution)
            $toStageDelta = (ConvertTo-IntValue $toStageTuple.side_to_move_contribution) - (ConvertTo-IntValue $toBootstrapTuple.side_to_move_contribution)
            $inducedPairDelta = $toStageDelta - $fromStageDelta

            $allTupleRows.Add([pscustomobject][ordered]@{
                seed = $seed
                comparison_id = $comparison.comparison_id
                comparison_kind = $comparison.comparison_kind
                is_control = [bool]$comparison.is_control
                stage = $comparison.stage
                from_root_move_label = $comparison.from_label
                to_root_move_label = $comparison.to_label
                from_root_move_role = $fromStageSummary.root_move_role
                to_root_move_role = $toStageSummary.root_move_role
                from_root_move = $fromStageSummary.root_move
                to_root_move = $toStageSummary.root_move
                tuple_id = $tupleId
                from_stage_delta = $fromStageDelta
                to_stage_delta = $toStageDelta
                induced_pair_delta = $inducedPairDelta
                abs_induced_pair_delta = [math]::Abs($inducedPairDelta)
                from_stage_contribution = ConvertTo-IntValue $fromStageTuple.side_to_move_contribution
                to_stage_contribution = ConvertTo-IntValue $toStageTuple.side_to_move_contribution
                from_bootstrap_contribution = ConvertTo-IntValue $fromBootstrapTuple.side_to_move_contribution
                to_bootstrap_contribution = ConvertTo-IntValue $toBootstrapTuple.side_to_move_contribution
                from_encoded_index = ConvertTo-IntValue $fromStageTuple.encoded_index
                to_encoded_index = ConvertTo-IntValue $toStageTuple.encoded_index
                from_absolute_index = ConvertTo-IntValue $fromStageTuple.absolute_index
                to_absolute_index = ConvertTo-IntValue $toStageTuple.absolute_index
                descriptor_label = $toStageTuple.descriptor_label
                cells = $toStageTuple.cells
            })
        }

        $tupleInducedSum = Get-Sum ($allTupleRows.ToArray()) "induced_pair_delta"
        Assert-Equal $tupleInducedSum $trainingInducedPairDelta "tuple induced pair delta sum seed=$seed comparison=$($comparison.comparison_id)"

        $negativeRows = @(
            $allTupleRows.ToArray() |
                Where-Object { [int]$_.induced_pair_delta -lt 0 } |
                Sort-Object -Property `
                    @{ Expression = { [math]::Abs([int]$_.induced_pair_delta) }; Descending = $true },
                    @{ Expression = { [int]$_.tuple_id }; Descending = $false },
                    @{ Expression = { [int]$_.to_absolute_index }; Descending = $false },
                    @{ Expression = { [int]$_.from_absolute_index }; Descending = $false }
        )

        $rank = 1
        foreach ($row in $negativeRows) {
            $tupleOutputRows.Add([pscustomobject][ordered]@{
                seed = $row.seed
                comparison_id = $row.comparison_id
                comparison_kind = $row.comparison_kind
                is_control = $row.is_control
                stage = $row.stage
                from_root_move_label = $row.from_root_move_label
                to_root_move_label = $row.to_root_move_label
                from_root_move_role = $row.from_root_move_role
                to_root_move_role = $row.to_root_move_role
                from_root_move = $row.from_root_move
                to_root_move = $row.to_root_move
                tuple_id = $row.tuple_id
                rank = $rank
                topk_membership = Get-TopKMembership $rank $TopKValues
                from_stage_delta = $row.from_stage_delta
                to_stage_delta = $row.to_stage_delta
                induced_pair_delta = $row.induced_pair_delta
                abs_induced_pair_delta = $row.abs_induced_pair_delta
                from_stage_contribution = $row.from_stage_contribution
                to_stage_contribution = $row.to_stage_contribution
                from_bootstrap_contribution = $row.from_bootstrap_contribution
                to_bootstrap_contribution = $row.to_bootstrap_contribution
                from_encoded_index = $row.from_encoded_index
                to_encoded_index = $row.to_encoded_index
                from_absolute_index = $row.from_absolute_index
                to_absolute_index = $row.to_absolute_index
                descriptor_label = $row.descriptor_label
                cells = $row.cells
            })
            $rank++
        }

        $negativeInducedSum = Get-Sum $negativeRows "induced_pair_delta"
        $comparisonSeedFacts.Add([pscustomobject][ordered]@{
            seed = $seed
            comparison_id = $comparison.comparison_id
            comparison_kind = $comparison.comparison_kind
            is_control = [bool]$comparison.is_control
            stage = $comparison.stage
            from_root_move_label = $comparison.from_label
            to_root_move_label = $comparison.to_label
            from_root_move_role = $fromStageSummary.root_move_role
            to_root_move_role = $toStageSummary.root_move_role
            stage_pair_delta = $stagePairDelta
            bootstrap_pair_delta = $bootstrapPairDelta
            training_induced_pair_delta = $trainingInducedPairDelta
            negative_candidate_count = $negativeRows.Count
            negative_induced_sum = $negativeInducedSum
        })

        foreach ($k in $TopKValues) {
            if ($k -eq "all_negative") {
                $selected = @($negativeRows)
            } else {
                $selected = @($negativeRows | Select-Object -First ([int]$k))
            }
            $selectedInducedSum = Get-Sum (@($selected)) "induced_pair_delta"
            $ablatedTrainingInducedPairDelta = $trainingInducedPairDelta - $selectedInducedSum
            $ablatedStagePairDelta = $stagePairDelta - $selectedInducedSum
            $stageDriftReductionRatio = 0.0
            if ($stagePairDelta -lt 0) {
                $stageDriftReductionRatio = [math]::Min(1.0, ([double][math]::Abs($selectedInducedSum) / [double][math]::Abs($stagePairDelta)))
            }
            $inducedReductionRatio = 0.0
            if ($trainingInducedPairDelta -lt 0) {
                $inducedReductionRatio = [math]::Min(1.0, ([double][math]::Abs($selectedInducedSum) / [double][math]::Abs($trainingInducedPairDelta)))
            }
            $signFlipOrTie = ($stagePairDelta -lt 0 -and $ablatedStagePairDelta -ge 0)
            $overNeutralization = ($stagePairDelta -lt 0 -and $ablatedStagePairDelta -gt 0)
            $kResult = Get-KResult ([bool]$comparison.is_control) $stagePairDelta $ablatedStagePairDelta $stageDriftReductionRatio $k

            $sweepRows.Add([pscustomobject][ordered]@{
                seed = $seed
                comparison_id = $comparison.comparison_id
                comparison_kind = $comparison.comparison_kind
                is_control = [bool]$comparison.is_control
                stage = $comparison.stage
                from_root_move_label = $comparison.from_label
                to_root_move_label = $comparison.to_label
                from_root_move_role = $fromStageSummary.root_move_role
                to_root_move_role = $toStageSummary.root_move_role
                stage_pair_delta = $stagePairDelta
                bootstrap_pair_delta = $bootstrapPairDelta
                training_induced_pair_delta = $trainingInducedPairDelta
                negative_candidate_count = $negativeRows.Count
                topk = $k
                selected_count = @($selected).Count
                selected_tuple_ids = ((@($selected) | ForEach-Object { [string]$_.tuple_id }) -join "|")
                selected_induced_delta_sum = $selectedInducedSum
                ablated_training_induced_pair_delta = $ablatedTrainingInducedPairDelta
                ablated_stage_pair_delta = $ablatedStagePairDelta
                stage_drift_reduction_ratio = Get-RatioText $stageDriftReductionRatio
                induced_reduction_ratio = Get-RatioText $inducedReductionRatio
                sign_flip_or_tie = $signFlipOrTie
                over_neutralization = $overNeutralization
                k_result = $kResult
            })
        }
    }
}

$summaryOutputRows = New-Object System.Collections.Generic.List[object]
foreach ($comparison in $comparisonDefinitions) {
    $facts = @($comparisonSeedFacts.ToArray() | Where-Object { $_.comparison_id -eq $comparison.comparison_id })
    $sweeps = @($sweepRows.ToArray() | Where-Object { $_.comparison_id -eq $comparison.comparison_id })
    $tupleCandidates = @($tupleOutputRows.ToArray() | Where-Object { $_.comparison_id -eq $comparison.comparison_id })

    $successCountsByK = [ordered]@{}
    foreach ($k in $TopKValues) {
        $successCountsByK[$k] = @($sweeps | Where-Object { $_.topk -eq $k -and $_.k_result -eq "success" }).Count
    }

    $seedSuccessWithin20 = 0
    $seedAllNegativeSuccess = 0
    $minSuccessKBySeed = New-Object System.Collections.Generic.List[string]
    foreach ($seed in $SeedValues) {
        $seedSweeps = @($sweeps | Where-Object { $_.seed -eq $seed })
        $successRows = @(
            $seedSweeps |
                Where-Object { $_.k_result -eq "success" } |
                Sort-Object -Property @{ Expression = { Get-TopKRankValue $_.topk }; Descending = $false }
        )
        if ($successRows.Count -gt 0) {
            $minK = [string]$successRows[0].topk
            $minSuccessKBySeed.Add("${seed}:$minK")
            if ($minK -ne "all_negative" -and [int]$minK -le 20) {
                $seedSuccessWithin20++
            }
        } else {
            $minSuccessKBySeed.Add("${seed}:none")
        }

        $allNegativeRow = @($seedSweeps | Where-Object { $_.topk -eq "all_negative" })[0]
        if ($null -ne $allNegativeRow -and $allNegativeRow.k_result -eq "success") {
            $seedAllNegativeSuccess++
        }
    }

    $decision = "inconclusive"
    $decisionNotes = ""
    if ([bool]$comparison.is_control) {
        $controlFailures = @($sweeps | Where-Object { $_.k_result -eq "failure" }).Count
        if ($controlFailures -eq 0) {
            $decision = "success"
            $decisionNotes = "control passed; no drift-favoring stage pair delta"
        } else {
            $decision = "failure"
            $decisionNotes = "control failed; stop before interpreting ablation"
        }
    } else {
        if ($seedSuccessWithin20 -ge 2) {
            $decision = "success"
            $decisionNotes = ">=2/3 seeds reach >=60% drift reduction or tie/non-drift by K<=20"
        } elseif ($seedAllNegativeSuccess -ge 2) {
            $decision = "inconclusive"
            $decisionNotes = "needs all_negative rather than bounded TopK"
        } else {
            $k20Rows = @($sweeps | Where-Object { $_.topk -eq "20" })
            $poorK20 = @($k20Rows | Where-Object { (ConvertTo-DoubleValue $_.stage_drift_reduction_ratio) -lt 0.3 }).Count
            if ($poorK20 -ge 2) {
                $decision = "failure"
                $decisionNotes = "K=20 fails to reduce drift by 30% in >=2/3 seeds"
            } else {
                $decision = "inconclusive"
                $decisionNotes = "seed split or medium reduction"
            }
        }
    }

    $offlineSignal = "not_a_basis"
    if ($comparison.comparison_kind -eq "main" -and $decision -eq "success") {
        $offlineSignal = "supports_offline_neutralization_design_review"
    } elseif ($comparison.comparison_kind -eq "auxiliary" -and $decision -eq "success") {
        $offlineSignal = "supports_path_explanation_only"
    } elseif ($comparison.comparison_kind -eq "control" -and $decision -eq "success") {
        $offlineSignal = "control_pass"
    } elseif ($decision -eq "failure") {
        $offlineSignal = "stop"
    }

    $stagePairDeltas = (($facts | ForEach-Object { "$($_.seed):$($_.stage_pair_delta)" }) -join "|")
    $bootstrapPairDeltas = (($facts | ForEach-Object { "$($_.seed):$($_.bootstrap_pair_delta)" }) -join "|")
    $trainingInducedDeltas = (($facts | ForEach-Object { "$($_.seed):$($_.training_induced_pair_delta)" }) -join "|")
    $negativeCounts = (($facts | ForEach-Object { "$($_.seed):$($_.negative_candidate_count)" }) -join "|")

    $comparisonStopRule = if ($offlineSignal -eq "stop") {
        "stop"
    } else {
        "do_not_execute_weight_neutralization_without_separate_design_review"
    }

    $summaryOutputRows.Add([pscustomobject][ordered]@{
        comparison_id = $comparison.comparison_id
        comparison_kind = $comparison.comparison_kind
        is_control = [bool]$comparison.is_control
        stage = $comparison.stage
        from_root_move_label = $comparison.from_label
        to_root_move_label = $comparison.to_label
        seed_count = $facts.Count
        stage_pair_deltas = $stagePairDeltas
        bootstrap_pair_deltas = $bootstrapPairDeltas
        training_induced_pair_deltas = $trainingInducedDeltas
        negative_candidate_counts = $negativeCounts
        k1_success_seed_count = $successCountsByK["1"]
        k3_success_seed_count = $successCountsByK["3"]
        k5_success_seed_count = $successCountsByK["5"]
        k10_success_seed_count = $successCountsByK["10"]
        k20_success_seed_count = $successCountsByK["20"]
        all_negative_success_seed_count = $successCountsByK["all_negative"]
        min_success_k_by_seed = ($minSuccessKBySeed.ToArray() -join "|")
        top10_tuple_ids_3of3 = Get-StableTupleList $tupleCandidates 3 10
        top10_tuple_ids_2of3 = Get-StableTupleList $tupleCandidates 2 10
        decision = $decision
        decision_notes = $decisionNotes
        offline_neutralization_design_signal = $offlineSignal
        stop_rule = $comparisonStopRule
    })
}

$mainSummaries = @($summaryOutputRows.ToArray() | Where-Object { $_.comparison_kind -eq "main" })
$controlSummaries = @($summaryOutputRows.ToArray() | Where-Object { $_.comparison_kind -eq "control" })
$mainSuccess = (@($mainSummaries | Where-Object { $_.decision -eq "success" }).Count -eq $mainSummaries.Count)
$controlsPass = (@($controlSummaries | Where-Object { $_.decision -eq "success" }).Count -eq $controlSummaries.Count)

$globalDecision = if ($mainSuccess -and $controlsPass) {
    "success_for_offline_neutralization_design_review"
} elseif (-not $controlsPass) {
    "failure_stop_controls_failed"
} else {
    "inconclusive_stop_before_offline_neutralization"
}
$designReviewAllowed = [bool]($globalDecision -eq "success_for_offline_neutralization_design_review")
$globalStopRule = if ($designReviewAllowed) {
    "do_not_execute_weight_neutralization_without_separate_design_review"
} else {
    "stop"
}
$topKPolicyArray = [string[]]@($TopKValues)
$comparisonDefinitionArray = [object[]]@($comparisonDefinitions)
$summaryOutputArray = [object[]]@($summaryOutputRows.ToArray())

$summaryCsvOut = Join-Path $outputDirPath "opening-child-static-ablation-summary.csv"
$tupleCsvOut = Join-Path $outputDirPath "opening-child-static-ablation-tuples.csv"
$sweepCsvOut = Join-Path $outputDirPath "opening-child-static-ablation-sweep.csv"
$summaryJsonOut = Join-Path $outputDirPath "opening-child-static-ablation-summary.json"
$manifestJsonOut = Join-Path $outputDirPath "opening-child-static-ablation-manifest.json"

$summaryOutputRows | Export-Csv -LiteralPath $summaryCsvOut -NoTypeInformation
$tupleOutputRows | Export-Csv -LiteralPath $tupleCsvOut -NoTypeInformation
$sweepRows | Export-Csv -LiteralPath $sweepCsvOut -NoTypeInformation

$gitCommit = (& git -C $repoRoot rev-parse HEAD).Trim()
$generatedAt = (Get-Date).ToUniversalTime().ToString("o")

$inputFileManifest = [ordered]@{}
foreach ($key in $inputFiles.Keys) {
    $inputFileManifest[$key] = [ordered]@{
        path = $inputFiles[$key].path
        sha256 = $inputFiles[$key].sha256
        length = $inputFiles[$key].length
    }
}

$summaryObject = [ordered]@{
    generated_at = $generatedAt
    phase = "Phase 3.7 child-position static ablation execution"
    mode = "report_only_table_static_ablation"
    attribution_dir = $attributionDirPath
    output_dir = $outputDirPath
    git_commit = $gitCommit
    attribution_manifest_git_commit = $attributionManifest.git_commit
    attribution_manifest_sha256 = $inputFiles.attribution_manifest_json.sha256
    tuple_detail_sha256 = $inputFiles.attribution_tuple_csv.sha256
    tuple_set_hash = $ExpectedTupleSetHash
    weight_count = $ExpectedWeightCount
    topk_policy = $topKPolicyArray
    global_decision = $globalDecision
    phase3_status = "failed"
    phase4_status = "blocked"
    offline_neutralization_design_review_allowed = $designReviewAllowed
    offline_neutralization_execution_allowed = $false
    weights_written = $false
    candidate_weights_written = $false
    search_executed = $false
    training_executed = $false
    games_per_seed_64_executed = $false
    selection_gate_modified = $false
    static_ablation_executed = $true
    comparisons = $comparisonDefinitionArray
    summary = $summaryOutputArray
    stop_rule = $globalStopRule
}
$summaryObject | ConvertTo-Json -Depth 14 | Set-Content -LiteralPath $summaryJsonOut -Encoding utf8

$manifestObject = [ordered]@{
    script_path = $scriptPath
    script_sha256 = Get-FileSha256 $scriptPath
    attribution_dir = $attributionDirPath
    output_dir = $outputDirPath
    generated_at = $generatedAt
    phase = "Phase 3.7 child-position static ablation execution"
    mode = "report_only_table_static_ablation"
    git_commit = $gitCommit
    attribution_manifest_git_commit = $attributionManifest.git_commit
    attribution_manifest_sha256 = $inputFiles.attribution_manifest_json.sha256
    tuple_detail_sha256 = $inputFiles.attribution_tuple_csv.sha256
    tuple_set_hash = $ExpectedTupleSetHash
    weight_count = $ExpectedWeightCount
    seeds = @($SeedValues)
    topk_policy = $topKPolicyArray
    comparisons = $comparisonDefinitionArray
    input_files = $inputFileManifest
    output_files = [ordered]@{
        summary_csv = $summaryCsvOut
        tuple_csv = $tupleCsvOut
        sweep_csv = $sweepCsvOut
        summary_json = $summaryJsonOut
        manifest_json = $manifestJsonOut
    }
    checks = [ordered]@{
        attribution_summary_row_count = $summaryRows.Count
        attribution_tuple_row_count = $tupleInputRows.Count
        attribution_top_delta_row_count = $topDeltaRows.Count
        static_ablation_summary_row_count = $summaryOutputRows.Count
        static_ablation_tuple_row_count = $tupleOutputRows.Count
        static_ablation_sweep_row_count = $sweepRows.Count
        active_tuple_count = $ExpectedActiveTupleCount
        attribution_manifest_hash_status = "captured"
        tuple_detail_hash_status = "captured"
    }
    boundaries = [ordered]@{
        production_code_modified = $false
        selection_gate_modified = $false
        weights_written = $false
        candidate_weights_written = $false
        offline_neutralization_executed = $false
        search_executed = $false
        training_executed = $false
        games_per_seed_64_executed = $false
        phase4_entered = $false
    }
    global_decision = $globalDecision
    phase3_status = "failed"
    phase4_status = "blocked"
    stop_rule = $globalStopRule
}
$manifestObject | ConvertTo-Json -Depth 14 | Set-Content -LiteralPath $manifestJsonOut -Encoding utf8

Write-Host "[PASS] child-position static ablation generated"
Write-Host "summary_rows=$($summaryOutputRows.Count)"
Write-Host "tuple_rows=$($tupleOutputRows.Count)"
Write-Host "sweep_rows=$($sweepRows.Count)"
Write-Host "global_decision=$globalDecision"
Write-Host "output_dir=$outputDirPath"
