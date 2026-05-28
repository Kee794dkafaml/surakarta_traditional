[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$RunRoot,

    [Parameter(Mandatory = $true)]
    [string]$OutputDir,

    [string[]]$Seeds = @("20260423", "20260424", "20260425"),

    [string[]]$Stages = @("bootstrap", "cp2", "final"),

    [string[]]$RootMoves = @(
        "baseline:(0,1)->(1,2)",
        "cp2:(5,1)->(4,2)",
        "final:(3,1)->(2,2)"
    ),

    [int]$TopK = 20
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$BoardSize = 6
$ExpectedTupleSetHash = [uint64]0x1D4A6EBF2BAF6484
$ExpectedTupleSetHashText = "0x1D4A6EBF2BAF6484"
$ExpectedMagic = [uint32]0x5455504C
$ExpectedVersion = [uint16]1
$ExpectedWeightCount = [uint32]16605
$ExpectedHeaderBytes = 24
$ExpectedFileLength = $ExpectedHeaderBytes + ([int]$ExpectedWeightCount * 4)

function Fail {
    param([string]$Message)
    throw "[opening-attribution] $Message"
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

function Assert-DoubleEqual {
    param(
        [double]$Actual,
        [double]$Expected,
        [string]$Label
    )
    if ([math]::Abs($Actual - $Expected) -gt 0.000000001) {
        Fail "$Label mismatch: actual='$Actual' expected='$Expected'"
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
    param([string]$Path)
    $fullPath = Get-ExistingFullPath $Path
    $content = Get-Content -LiteralPath $fullPath -Raw
    return [pscustomobject]@{
        path = $fullPath
        sha256 = Get-FileSha256 $fullPath
        length = (Get-Item -LiteralPath $fullPath).Length
        content = $content
    }
}

function New-TupleDescriptors {
    return @(
        @(0, 1, 6, 7), @(1, 2, 7, 8), @(2, 3, 8, 9), @(3, 4, 9, 10), @(4, 5, 10, 11),
        @(6, 7, 12, 13), @(7, 8, 13, 14), @(8, 9, 14, 15), @(9, 10, 15, 16), @(10, 11, 16, 17),
        @(12, 13, 18, 19), @(13, 14, 19, 20), @(14, 15, 20, 21), @(15, 16, 21, 22), @(16, 17, 22, 23),
        @(18, 19, 24, 25), @(19, 20, 25, 26), @(20, 21, 26, 27), @(21, 22, 27, 28), @(22, 23, 28, 29),
        @(24, 25, 30, 31), @(25, 26, 31, 32), @(26, 27, 32, 33), @(27, 28, 33, 34), @(28, 29, 34, 35),
        @(0, 1, 2, 3, 4, 5), @(6, 7, 8, 9, 10, 11), @(12, 13, 14, 15, 16, 17),
        @(18, 19, 20, 21, 22, 23), @(24, 25, 26, 27, 28, 29), @(30, 31, 32, 33, 34, 35),
        @(0, 6, 12, 18, 24, 30), @(1, 7, 13, 19, 25, 31), @(2, 8, 14, 20, 26, 32),
        @(3, 9, 15, 21, 27, 33), @(4, 10, 16, 22, 28, 34), @(5, 11, 17, 23, 29, 35),
        @(1, 2, 6, 7, 8, 12), @(3, 4, 9, 10, 11, 17), @(18, 24, 25, 26, 30, 31),
        @(24, 28, 29, 33, 34, 35), @(1, 7, 13, 19, 25, 31), @(4, 10, 16, 22, 28, 34),
        @(6, 12, 13, 14, 20, 26), @(9, 15, 21, 22, 23, 29)
    )
}

function Invoke-FnvStep {
    param(
        [uint64]$Hash,
        [uint64]$Value
    )
    $prime = [System.Numerics.BigInteger]1099511628211
    $mod = [System.Numerics.BigInteger]::Pow([System.Numerics.BigInteger]2, 64)
    $mixed = $Hash -bxor $Value
    return [uint64]([System.Numerics.BigInteger]::Remainder(([System.Numerics.BigInteger]$mixed * $prime), $mod))
}

function Get-TupleSetHash {
    param([object[]]$Descriptors)
    $hash = [uint64]1469598103934665603
    foreach ($descriptor in $Descriptors) {
        $hash = Invoke-FnvStep $hash ([uint64]$descriptor.Count)
        foreach ($square in $descriptor) {
            $hash = Invoke-FnvStep $hash ([uint64]([int]$square + 1))
        }
    }
    return $hash
}

function Get-TupleOffsets {
    param([object[]]$Descriptors)
    $offsets = New-Object int[] $Descriptors.Count
    $offset = 0
    for ($i = 0; $i -lt $Descriptors.Count; $i++) {
        $offsets[$i] = $offset
        $span = 1
        for ($j = 0; $j -lt $Descriptors[$i].Count; $j++) {
            $span *= 3
        }
        $offset += $span
    }
    return [pscustomobject]@{
        offsets = $offsets
        total = $offset
    }
}

function ConvertTo-Square {
    param(
        [int]$X,
        [int]$Y
    )
    if ($X -lt 0 -or $X -ge $BoardSize -or $Y -lt 0 -or $Y -ge $BoardSize) {
        Fail "square out of range: ($X,$Y)"
    }
    return ($Y * $BoardSize) + $X
}

function Format-Square {
    param([int]$Square)
    $x = $Square % $BoardSize
    $y = [math]::Floor($Square / $BoardSize)
    return "($x,$y)"
}

function Format-Cells {
    param([object[]]$Cells)
    return (($Cells | ForEach-Object { Format-Square ([int]$_) }) -join "|")
}

function Parse-RootMove {
    param([string]$Spec)
    $pattern = '^([^:]+):\((\d+),\s*(\d+)\)->\((\d+),\s*(\d+)\)$'
    $match = [regex]::Match($Spec, $pattern)
    if (-not $match.Success) {
        Fail "invalid root move spec '$Spec'; expected label:(x,y)->(x,y)"
    }
    $label = $match.Groups[1].Value.Trim(" `t'`"")
    $fromX = [int]$match.Groups[2].Value
    $fromY = [int]$match.Groups[3].Value
    $toX = [int]$match.Groups[4].Value
    $toY = [int]$match.Groups[5].Value
    $from = ConvertTo-Square $fromX $fromY
    $to = ConvertTo-Square $toX $toY
    return [pscustomobject]@{
        label = $label
        role = if ($label -eq "baseline") { "baseline_child" } elseif ($label -eq "cp2") { "cp2_drift_child" } elseif ($label -eq "final") { "final_drift_child" } else { "${label}_child" }
        from_x = $fromX
        from_y = $fromY
        to_x = $toX
        to_y = $toY
        from_square = $from
        to_square = $to
        move = "($fromX, $fromY) -> ($toX, $toY) (B)"
        compact = "($fromX,$fromY)->($toX,$toY)"
    }
}

function Convert-SeedArgs {
    param([string[]]$SeedArgs)
    $values = New-Object System.Collections.Generic.List[int]
    foreach ($arg in $SeedArgs) {
        foreach ($part in ($arg -split ",")) {
            $trimmed = $part.Trim()
            if ($trimmed.Length -eq 0) {
                continue
            }
            $values.Add([int]$trimmed)
        }
    }
    return [int[]]@($values)
}

function Convert-StageArgs {
    param([string[]]$StageArgs)
    $values = New-Object System.Collections.Generic.List[string]
    foreach ($arg in $StageArgs) {
        foreach ($part in ($arg -split ",")) {
            $trimmed = $part.Trim()
            if ($trimmed.Length -eq 0) {
                continue
            }
            $values.Add($trimmed)
        }
    }
    return [string[]]@($values)
}

function Convert-RootMoveArgs {
    param([string[]]$RootMoveArgs)
    $combined = $RootMoveArgs -join ","
    $pattern = '[^:,\s]+:\(\d+\s*,\s*\d+\)->\(\d+\s*,\s*\d+\)'
    $matches = [regex]::Matches($combined, $pattern)
    $values = New-Object System.Collections.Generic.List[string]
    foreach ($match in $matches) {
        $values.Add($match.Value)
    }
    if ($values.Count -eq 0) {
        Fail "no valid root move specs found"
    }
    return [string[]]@($values)
}

function New-OpeningBoard {
    $board = New-Object int[] ($BoardSize * $BoardSize)
    for ($y = 0; $y -lt $BoardSize; $y++) {
        for ($x = 0; $x -lt $BoardSize; $x++) {
            $square = ConvertTo-Square $x $y
            if ($y -lt 2) {
                $board[$square] = 1
            } elseif ($y -ge ($BoardSize - 2)) {
                $board[$square] = 2
            } else {
                $board[$square] = 0
            }
        }
    }
    return $board
}

function Copy-Board {
    param([int[]]$Board)
    return [int[]]$Board.Clone()
}

function Apply-RootMove {
    param(
        [int[]]$Board,
        [object]$Move
    )
    $child = Copy-Board $Board
    if ($child[$Move.from_square] -ne 1) {
        Fail "root move '$($Move.move)' from square is not a black piece"
    }
    if ($child[$Move.to_square] -ne 0) {
        Fail "root move '$($Move.move)' to square is not empty"
    }
    $child[$Move.from_square] = 0
    $child[$Move.to_square] = 1
    return [pscustomobject]@{
        board = $child
        side_to_move = "White"
    }
}

function Count-BoardColor {
    param(
        [int[]]$Board,
        [int]$Color
    )
    $count = 0
    foreach ($cell in $Board) {
        if ($cell -eq $Color) {
            $count++
        }
    }
    return $count
}

function Encode-Tuple {
    param(
        [int[]]$Board,
        [object[]]$Descriptor
    )
    $encoded = 0
    $factor = 1
    foreach ($square in $Descriptor) {
        $state = $Board[[int]$square]
        $encoded += $state * $factor
        $factor *= 3
    }
    return $encoded
}

function Read-WeightFile {
    param(
        [string]$Path,
        [string]$ExpectedSha256,
        [int]$ExpectedSeed,
        [string]$Stage
    )
    $fullPath = Get-ExistingFullPath $Path
    $item = Get-Item -LiteralPath $fullPath
    Assert-Equal $item.Length $ExpectedFileLength "weight file length for seed $ExpectedSeed stage $Stage"

    $sha256 = Get-FileSha256 $fullPath
    Assert-Equal $sha256 $ExpectedSha256 "SHA256 for seed $ExpectedSeed stage $Stage"

    $stream = [System.IO.File]::Open($fullPath, [System.IO.FileMode]::Open, [System.IO.FileAccess]::Read, [System.IO.FileShare]::Read)
    try {
        $reader = [System.IO.BinaryReader]::new($stream)
        $magic = $reader.ReadUInt32()
        $version = $reader.ReadUInt16()
        $reserved = $reader.ReadUInt16()
        $tupleHash = $reader.ReadUInt64()
        $weightCount = $reader.ReadUInt32()
        $padding = $reader.ReadBytes(4)
        Assert-Equal $magic $ExpectedMagic "weight magic for seed $ExpectedSeed stage $Stage"
        Assert-Equal $version $ExpectedVersion "weight version for seed $ExpectedSeed stage $Stage"
        Assert-Equal $tupleHash $ExpectedTupleSetHash "tuple_set_hash for seed $ExpectedSeed stage $Stage"
        Assert-Equal $weightCount $ExpectedWeightCount "weight_count for seed $ExpectedSeed stage $Stage"
        Assert-Equal $padding.Count 4 "weight header padding length for seed $ExpectedSeed stage $Stage"
        Assert-Equal $stream.Position $ExpectedHeaderBytes "weight payload offset for seed $ExpectedSeed stage $Stage"

        $weights = New-Object int[] ([int]$ExpectedWeightCount)
        for ($i = 0; $i -lt [int]$ExpectedWeightCount; $i++) {
            $weights[$i] = $reader.ReadInt32()
        }
        Assert-Equal $stream.Position $stream.Length "weight payload length for seed $ExpectedSeed stage $Stage"
        return [pscustomobject]@{
            path = $fullPath
            sha256 = $sha256
            magic = ("0x{0:X8}" -f $magic)
            version = [int]$version
            reserved = [int]$reserved
            tuple_set_hash = ("0x{0:X16}" -f $tupleHash)
            weight_count = [int]$weightCount
            weights = $weights
        }
    } finally {
        $stream.Dispose()
    }
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
        Fail "expected exactly one manifest artifact for seed $Seed stage $Stage, found $($matches.Count)"
    }

    $artifact = $matches[0]
    Assert-Equal $artifact.seed $Seed "artifact seed for stage $Stage"
    Assert-Equal $artifact.depth $SelectionSummary.depth "artifact depth for seed $Seed stage $Stage"
    Assert-DoubleEqual ([double]$artifact.alpha) ([double]$SelectionSummary.alpha) "artifact alpha for seed $Seed stage $Stage"
    Assert-DoubleEqual ([double]$artifact.lambda) ([double]$SelectionSummary.lambda) "artifact lambda for seed $Seed stage $Stage"
    Assert-DoubleEqual ([double]$artifact.epsilon) ([double]$SelectionSummary.epsilon) "artifact epsilon for seed $Seed stage $Stage"
    Assert-Equal $artifact.epsilon_plies $SelectionSummary.epsilon_plies "artifact epsilon_plies for seed $Seed stage $Stage"
    Assert-DoubleEqual ([double]$artifact.terminal_reward) ([double]$SelectionSummary.terminal_reward) "artifact terminal_reward for seed $Seed stage $Stage"
    Assert-DoubleEqual ([double]$artifact.td_error_clip) ([double]$SelectionSummary.td_error_clip) "artifact td_error_clip for seed $Seed stage $Stage"
    Assert-Equal $artifact.terminal_only_warmup $SelectionSummary.terminal_only_warmup "artifact terminal_only_warmup for seed $Seed stage $Stage"
    Assert-Equal $artifact.near_terminal_curriculum $SelectionSummary.near_terminal_curriculum "artifact near_terminal_curriculum for seed $Seed stage $Stage"
    Assert-Equal $artifact.checkpoint_every $SelectionSummary.checkpoint_every "artifact checkpoint_every for seed $Seed stage $Stage"
    Assert-Equal $artifact.tuple_set_hash $ExpectedTupleSetHashText "artifact tuple_set_hash for seed $Seed stage $Stage"
    Assert-Equal $artifact.weight_count ([int]$ExpectedWeightCount) "artifact weight_count for seed $Seed stage $Stage"

    if ($Stage -eq "bootstrap") {
        Assert-Equal $artifact.artifact_kind "bootstrap" "artifact kind for seed $Seed stage $Stage"
        Assert-Equal $artifact.games 0 "artifact games for seed $Seed stage $Stage"
        Assert-Equal $artifact.checkpoint.games_completed 0 "checkpoint games_completed for seed $Seed stage $Stage"
    } elseif ($Stage -eq "cp2") {
        Assert-Equal $artifact.artifact_kind "checkpoint" "artifact kind for seed $Seed stage $Stage"
        Assert-Equal $artifact.games $SelectionSummary.games_per_seed "artifact games for seed $Seed stage $Stage"
        Assert-Equal $artifact.checkpoint.games_completed 2 "checkpoint games_completed for seed $Seed stage $Stage"
    } elseif ($Stage -eq "final") {
        Assert-Equal $artifact.artifact_kind "candidate" "artifact kind for seed $Seed stage $Stage"
        Assert-Equal $artifact.games $SelectionSummary.games_per_seed "artifact games for seed $Seed stage $Stage"
        Assert-Equal $artifact.checkpoint.games_completed $SelectionSummary.games_per_seed "checkpoint games_completed for seed $Seed stage $Stage"
    } else {
        Fail "unsupported stage '$Stage'"
    }

    if ([string]::IsNullOrWhiteSpace($artifact.weights_path)) {
        Fail "manifest artifact for seed $Seed stage $Stage does not include weights_path"
    }
    if ([string]::IsNullOrWhiteSpace($artifact.binary_sha256)) {
        Fail "manifest artifact for seed $Seed stage $Stage does not include binary_sha256"
    }

    $weightFile = Read-WeightFile $artifact.weights_path $artifact.binary_sha256 $Seed $Stage
    return [pscustomobject]@{
        artifact = $artifact
        weight_file = $weightFile
    }
}

function Get-AttributionRows {
    param(
        [int]$Seed,
        [string]$Stage,
        [object]$Artifact,
        [int[]]$Weights,
        [object]$RootMove,
        [object[]]$Descriptors,
        [int[]]$Offsets
    )

    $root = New-OpeningBoard
    $child = Apply-RootMove $root $RootMove
    $board = [int[]]$child.board
    $blackCount = Count-BoardColor $board 1
    $whiteCount = Count-BoardColor $board 2
    $material = ($blackCount - $whiteCount) * 100
    $sideSign = if ($child.side_to_move -eq "Black") { 1 } else { -1 }
    $tupleScore = 0
    $tupleRows = New-Object System.Collections.Generic.List[object]

    for ($tupleId = 0; $tupleId -lt $Descriptors.Count; $tupleId++) {
        $descriptor = @($Descriptors[$tupleId])
        $encoded = Encode-Tuple $board $descriptor
        $absolute = $Offsets[$tupleId] + $encoded
        if ($absolute -lt 0 -or $absolute -ge $Weights.Count) {
            Fail "active tuple absolute index out of range: tuple_id=$tupleId absolute=$absolute"
        }
        $weight = [int]$Weights[$absolute]
        $tupleScore += $weight
        $tupleRows.Add([pscustomobject]@{
            seed = $Seed
            stage = $Stage
            root_move_label = $RootMove.label
            root_move_role = $RootMove.role
            root_move = $RootMove.move
            child_side_to_move = $child.side_to_move
            tuple_id = $tupleId
            offset = $Offsets[$tupleId]
            encoded_index = $encoded
            absolute_index = $absolute
            weight = $weight
            contribution = $weight
            side_to_move_contribution = $sideSign * $weight
            descriptor_label = ("tuple_{0:D2}_len_{1}" -f $tupleId, $descriptor.Count)
            cells = (Format-Cells $descriptor)
        })
    }

    $activeTupleCount = $tupleRows.Count
    Assert-Equal $activeTupleCount 45 "active tuple count for seed $Seed stage $Stage root $($RootMove.label)"
    $blackPerspective = $material + $tupleScore
    $total = $sideSign * $blackPerspective

    $summary = [pscustomobject]@{
        seed = $Seed
        stage = $Stage
        artifact_path = $Artifact.weight_file.path
        artifact_sha256 = $Artifact.weight_file.sha256
        artifact_kind = $Artifact.artifact.artifact_kind
        checkpoint_games_completed = $Artifact.artifact.checkpoint.games_completed
        games = $Artifact.artifact.games
        depth = $Artifact.artifact.depth
        alpha = $Artifact.artifact.alpha
        lambda = $Artifact.artifact.lambda
        epsilon = $Artifact.artifact.epsilon
        epsilon_plies = $Artifact.artifact.epsilon_plies
        terminal_reward = $Artifact.artifact.terminal_reward
        td_error_clip = $Artifact.artifact.td_error_clip
        terminal_only_warmup = $Artifact.artifact.terminal_only_warmup
        near_terminal_curriculum = $Artifact.artifact.near_terminal_curriculum
        root_move_label = $RootMove.label
        root_move_role = $RootMove.role
        root_move = $RootMove.move
        child_side_to_move = $child.side_to_move
        black_count = $blackCount
        white_count = $whiteCount
        material_score = $material
        tuple_score = $tupleScore
        black_perspective_score = $blackPerspective
        total_static_score = $total
        active_tuple_count = $activeTupleCount
        tuple_set_hash = $ExpectedTupleSetHashText
        weight_count = [int]$ExpectedWeightCount
    }

    return [pscustomobject]@{
        summary = $summary
        tuples = $tupleRows.ToArray()
    }
}

function Get-StageDeltaRows {
    param(
        [hashtable]$ByKey,
        [int]$Seed,
        [string]$RootMoveLabel,
        [string]$FromStage,
        [string]$ToStage,
        [int]$TopK
    )
    $from = $ByKey["$Seed|$FromStage|$RootMoveLabel"]
    $to = $ByKey["$Seed|$ToStage|$RootMoveLabel"]
    if ($null -eq $from -or $null -eq $to) {
        Fail "missing attribution rows for stage delta seed=$Seed root=$RootMoveLabel $FromStage->$ToStage"
    }
    $sideSign = if ($to.summary.child_side_to_move -eq "Black") { 1 } else { -1 }
    $deltas = New-Object System.Collections.Generic.List[object]
    for ($i = 0; $i -lt $to.tuples.Count; $i++) {
        $fromTuple = $from.tuples[$i]
        $toTuple = $to.tuples[$i]
        $rawDelta = [int]$toTuple.contribution - [int]$fromTuple.contribution
        $staticDelta = $sideSign * $rawDelta
        $deltas.Add([pscustomobject]@{
            comparison_type = "${ToStage}_vs_${FromStage}_same_seed_same_root_move"
            seed = $Seed
            root_move_label = $RootMoveLabel
            root_move = $to.summary.root_move
            from_stage = $FromStage
            to_stage = $ToStage
            from_root_move_label = $RootMoveLabel
            to_root_move_label = $RootMoveLabel
            tuple_id = $toTuple.tuple_id
            from_encoded_index = $fromTuple.encoded_index
            to_encoded_index = $toTuple.encoded_index
            from_absolute_index = $fromTuple.absolute_index
            to_absolute_index = $toTuple.absolute_index
            from_weight = $fromTuple.weight
            to_weight = $toTuple.weight
            raw_black_perspective_delta = $rawDelta
            total_static_delta_contribution = $staticDelta
            comparison_total_static_delta = [int]$to.summary.total_static_score - [int]$from.summary.total_static_score
            descriptor_label = $toTuple.descriptor_label
            cells = $toTuple.cells
        })
    }
    return Select-TopDeltas $deltas $TopK
}

function Get-RootMoveDeltaRows {
    param(
        [hashtable]$ByKey,
        [int]$Seed,
        [string]$Stage,
        [string]$FromRootMoveLabel,
        [string]$ToRootMoveLabel,
        [int]$TopK
    )
    $from = $ByKey["$Seed|$Stage|$FromRootMoveLabel"]
    $to = $ByKey["$Seed|$Stage|$ToRootMoveLabel"]
    if ($null -eq $from -or $null -eq $to) {
        Fail "missing attribution rows for root move delta seed=$Seed stage=$Stage $FromRootMoveLabel->$ToRootMoveLabel"
    }
    $sideSign = if ($to.summary.child_side_to_move -eq "Black") { 1 } else { -1 }
    $deltas = New-Object System.Collections.Generic.List[object]
    for ($i = 0; $i -lt $to.tuples.Count; $i++) {
        $fromTuple = $from.tuples[$i]
        $toTuple = $to.tuples[$i]
        $rawDelta = [int]$toTuple.contribution - [int]$fromTuple.contribution
        $staticDelta = $sideSign * $rawDelta
        $deltas.Add([pscustomobject]@{
            comparison_type = "${ToRootMoveLabel}_child_vs_${FromRootMoveLabel}_child_same_seed_same_stage"
            seed = $Seed
            root_move_label = $ToRootMoveLabel
            root_move = $to.summary.root_move
            from_stage = $Stage
            to_stage = $Stage
            from_root_move_label = $FromRootMoveLabel
            to_root_move_label = $ToRootMoveLabel
            tuple_id = $toTuple.tuple_id
            from_encoded_index = $fromTuple.encoded_index
            to_encoded_index = $toTuple.encoded_index
            from_absolute_index = $fromTuple.absolute_index
            to_absolute_index = $toTuple.absolute_index
            from_weight = $fromTuple.weight
            to_weight = $toTuple.weight
            raw_black_perspective_delta = $rawDelta
            total_static_delta_contribution = $staticDelta
            comparison_total_static_delta = [int]$to.summary.total_static_score - [int]$from.summary.total_static_score
            descriptor_label = $toTuple.descriptor_label
            cells = $toTuple.cells
        })
    }
    return Select-TopDeltas $deltas $TopK
}

function Select-TopDeltas {
    param(
        [System.Collections.Generic.List[object]]$Deltas,
        [int]$TopK
    )
    $rows = New-Object System.Collections.Generic.List[object]
    $positive = @($Deltas | Sort-Object -Property total_static_delta_contribution -Descending | Select-Object -First $TopK)
    $negative = @($Deltas | Sort-Object -Property total_static_delta_contribution | Select-Object -First $TopK)
    $rank = 1
    foreach ($row in $positive) {
        $rows.Add(($row | Select-Object *, @{Name = "direction"; Expression = { "positive" } }, @{Name = "rank"; Expression = { $rank } }))
        $rank++
    }
    $rank = 1
    foreach ($row in $negative) {
        $rows.Add(($row | Select-Object *, @{Name = "direction"; Expression = { "negative" } }, @{Name = "rank"; Expression = { $rank } }))
        $rank++
    }
    return $rows.ToArray()
}

function Get-DeltaSummary {
    param([object[]]$Rows)
    if ($Rows.Count -eq 0) {
        return @()
    }
    $groups = $Rows | Group-Object comparison_type, seed, from_stage, to_stage, from_root_move_label, to_root_move_label
    $summaries = New-Object System.Collections.Generic.List[object]
    foreach ($group in $groups) {
        $sample = $group.Group[0]
        $uniqueTuples = @{}
        foreach ($row in $group.Group) {
            $key = [string]$row.tuple_id
            if (-not $uniqueTuples.ContainsKey($key)) {
                $uniqueTuples[$key] = [math]::Abs([int]$row.total_static_delta_contribution)
            }
        }
        $absValues = @($uniqueTuples.Values | Sort-Object -Descending)
        $absSum = 0
        foreach ($value in $absValues) {
            $absSum += [double]$value
        }
        $top1 = if ($absValues.Count -gt 0) { [double]$absValues[0] } else { 0.0 }
        $top3 = 0.0
        for ($i = 0; $i -lt [math]::Min(3, $absValues.Count); $i++) {
            $top3 += [double]$absValues[$i]
        }
        $top1Share = if ($absSum -gt 0) { $top1 / $absSum } else { 0.0 }
        $top3Share = if ($absSum -gt 0) { $top3 / $absSum } else { 0.0 }
        $dominance = if ($top1Share -ge 0.35 -or $top3Share -ge 0.60) {
            "yes"
        } elseif ($top3Share -ge 0.40) {
            "watch"
        } else {
            "no"
        }
        $summaries.Add([pscustomobject]@{
            comparison_type = $sample.comparison_type
            seed = $sample.seed
            from_stage = $sample.from_stage
            to_stage = $sample.to_stage
            from_root_move_label = $sample.from_root_move_label
            to_root_move_label = $sample.to_root_move_label
            comparison_total_static_delta = $sample.comparison_total_static_delta
            abs_delta_sum = [int]$absSum
            top1_abs_share = [math]::Round($top1Share, 6)
            top3_abs_share = [math]::Round($top3Share, 6)
            few_tuple_dominance = $dominance
        })
    }
    return $summaries.ToArray()
}

$SeedValues = Convert-SeedArgs $Seeds
$StageValues = Convert-StageArgs $Stages
$RootMoveValues = Convert-RootMoveArgs $RootMoves

$runRootPath = Get-ExistingFullPath $RunRoot
$outputDirPath = Get-FullPath $OutputDir
New-Item -ItemType Directory -Path $outputDirPath -Force | Out-Null

$repoRoot = Get-FullPath (Join-Path $PSScriptRoot "..")
$scriptPath = Get-ExistingFullPath $PSCommandPath

$requiredInputs = [ordered]@{}
$requiredInputs["weights_manifest"] = Read-RequiredTextFile (Join-Path $runRootPath "weights-manifest.json")
$requiredInputs["selection_summary"] = Read-RequiredTextFile (Join-Path $runRootPath "selection-summary.json")
$requiredInputs["opening_diagnostics_summary"] = Read-RequiredTextFile (Join-Path $runRootPath "opening-diagnostics\opening-diagnostics-summary.md")
$requiredInputs["task_plan"] = Read-RequiredTextFile (Join-Path $repoRoot "task_plan.md")
$requiredInputs["findings"] = Read-RequiredTextFile (Join-Path $repoRoot "findings.md")
$requiredInputs["progress"] = Read-RequiredTextFile (Join-Path $repoRoot "progress.md")

$manifest = $requiredInputs["weights_manifest"].content | ConvertFrom-Json
$selectionSummary = $requiredInputs["selection_summary"].content | ConvertFrom-Json
$openingSummaryText = $requiredInputs["opening_diagnostics_summary"].content

Assert-Equal $manifest.purpose "selection" "weights manifest purpose"
foreach ($seed in $SeedValues) {
    if (@($selectionSummary.seeds) -notcontains $seed) {
        Fail "seed $seed is not present in selection-summary.json"
    }
}

$tuples = New-TupleDescriptors
Assert-Equal $tuples.Count 45 "tuple descriptor count"
$tupleHash = Get-TupleSetHash $tuples
Assert-Equal $tupleHash $ExpectedTupleSetHash "computed tuple_set_hash"
$offsetResult = Get-TupleOffsets $tuples
Assert-Equal $offsetResult.total ([int]$ExpectedWeightCount) "tuple offset total weight count"
$offsets = [int[]]$offsetResult.offsets

$parsedRootMoves = @($RootMoveValues | ForEach-Object { Parse-RootMove $_ })
Assert-Equal $parsedRootMoves.Count 3 "root move count"
foreach ($rootMove in $parsedRootMoves) {
    if (-not $openingSummaryText.Contains($rootMove.move)) {
        Fail "opening diagnostics summary does not contain root move '$($rootMove.move)'"
    }
}

$summaryRows = New-Object System.Collections.Generic.List[object]
$tupleRows = New-Object System.Collections.Generic.List[object]
$byKey = @{}
$artifactValidations = New-Object System.Collections.Generic.List[object]
$artifactBySeedStage = @{}

foreach ($seed in $SeedValues) {
    foreach ($stage in $StageValues) {
        if ($StageValues.Count -gt 0 -and @("bootstrap", "cp2", "final") -notcontains $stage) {
            Fail "unsupported stage '$stage'"
        }
        $artifactInfo = Get-StageArtifact $manifest $selectionSummary $seed $stage
        $artifactBySeedStage["$seed|$stage"] = $artifactInfo
        $artifactValidations.Add([pscustomobject]@{
            seed = $seed
            stage = $stage
            artifact_kind = $artifactInfo.artifact.artifact_kind
            checkpoint_games_completed = $artifactInfo.artifact.checkpoint.games_completed
            artifact_path = $artifactInfo.weight_file.path
            artifact_sha256 = $artifactInfo.weight_file.sha256
            hash_status = "pass"
            header_status = "pass"
        })

        foreach ($rootMove in $parsedRootMoves) {
            $rows = Get-AttributionRows $seed $stage $artifactInfo $artifactInfo.weight_file.weights $rootMove $tuples $offsets
            $summaryRows.Add($rows.summary)
            foreach ($tupleRow in $rows.tuples) {
                $tupleRows.Add($tupleRow)
            }
            $byKey["$seed|$stage|$($rootMove.label)"] = $rows
        }
    }
}

Assert-Equal $summaryRows.Count 27 "summary row count"
Assert-Equal $tupleRows.Count 1215 "tuple detail row count"
foreach ($row in $summaryRows) {
    Assert-Equal $row.active_tuple_count 45 "summary active_tuple_count seed=$($row.seed) stage=$($row.stage) root=$($row.root_move_label)"
}

$topDeltaRows = New-Object System.Collections.Generic.List[object]
foreach ($seed in $SeedValues) {
    foreach ($rootMove in $parsedRootMoves) {
        foreach ($row in (Get-StageDeltaRows $byKey $seed $rootMove.label "bootstrap" "cp2" $TopK)) {
            $topDeltaRows.Add($row)
        }
        foreach ($row in (Get-StageDeltaRows $byKey $seed $rootMove.label "bootstrap" "final" $TopK)) {
            $topDeltaRows.Add($row)
        }
    }
    foreach ($stage in $StageValues) {
        foreach ($row in (Get-RootMoveDeltaRows $byKey $seed $stage "baseline" "cp2" $TopK)) {
            $topDeltaRows.Add($row)
        }
        foreach ($row in (Get-RootMoveDeltaRows $byKey $seed $stage "baseline" "final" $TopK)) {
            $topDeltaRows.Add($row)
        }
    }
}

$deltaSummaries = Get-DeltaSummary ($topDeltaRows.ToArray())

$summaryCsv = Join-Path $outputDirPath "opening-child-attribution-summary.csv"
$tupleCsv = Join-Path $outputDirPath "opening-child-attribution-tuples.csv"
$summaryJson = Join-Path $outputDirPath "opening-child-attribution-summary.json"
$topDeltasCsv = Join-Path $outputDirPath "opening-child-attribution-top-deltas.csv"
$attributionManifestPath = Join-Path $outputDirPath "attribution-manifest.json"

$summaryRows | Export-Csv -LiteralPath $summaryCsv -NoTypeInformation
$tupleRows | Export-Csv -LiteralPath $tupleCsv -NoTypeInformation
$topDeltaRows | Export-Csv -LiteralPath $topDeltasCsv -NoTypeInformation

$gitCommit = (& git -C $repoRoot rev-parse HEAD).Trim()
$generatedAt = (Get-Date).ToUniversalTime().ToString("o")

$inputFileManifest = [ordered]@{}
foreach ($key in $requiredInputs.Keys) {
    $inputFileManifest[$key] = [ordered]@{
        path = $requiredInputs[$key].path
        sha256 = $requiredInputs[$key].sha256
        length = $requiredInputs[$key].length
    }
}

$summaryObject = [ordered]@{
    generated_at = $generatedAt
    run_root = $runRootPath
    output_dir = $outputDirPath
    summary_row_count = $summaryRows.Count
    tuple_detail_row_count = $tupleRows.Count
    active_tuple_count = 45
    tuple_set_hash = $ExpectedTupleSetHashText
    weight_count = [int]$ExpectedWeightCount
    summary = $summaryRows.ToArray()
    delta_summaries = @($deltaSummaries)
}
$summaryObject | ConvertTo-Json -Depth 12 | Set-Content -LiteralPath $summaryJson -Encoding utf8

$attributionManifest = [ordered]@{
    script_path = $scriptPath
    script_sha256 = Get-FileSha256 $scriptPath
    run_root = $runRootPath
    output_dir = $outputDirPath
    seeds = @($SeedValues)
    stages = @($StageValues)
    root_moves = @($parsedRootMoves | Select-Object label, role, move, compact, from_square, to_square)
    tuple_set_hash = $ExpectedTupleSetHashText
    weight_count = [int]$ExpectedWeightCount
    generated_at = $generatedAt
    git_commit = $gitCommit
    manifest_git_commit = $manifest.git_commit
    phase = "Phase 3.5 opening attribution-only execution"
    artifact_validations = $artifactValidations.ToArray()
    input_files = $inputFileManifest
    output_files = [ordered]@{
        summary_csv = $summaryCsv
        tuple_detail_csv = $tupleCsv
        summary_json = $summaryJson
        top_deltas_csv = $topDeltasCsv
        attribution_manifest_json = $attributionManifestPath
    }
    checks = [ordered]@{
        summary_row_count = $summaryRows.Count
        tuple_detail_row_count = $tupleRows.Count
        active_tuple_count_all_rows = 45
        artifact_hash_status = "pass"
        weight_header_status = "pass"
    }
}
$attributionManifest | ConvertTo-Json -Depth 12 | Set-Content -LiteralPath $attributionManifestPath -Encoding utf8

Write-Host "[PASS] opening attribution generated"
Write-Host "summary_rows=$($summaryRows.Count)"
Write-Host "tuple_detail_rows=$($tupleRows.Count)"
Write-Host "top_delta_rows=$($topDeltaRows.Count)"
Write-Host "output_dir=$outputDirPath"
