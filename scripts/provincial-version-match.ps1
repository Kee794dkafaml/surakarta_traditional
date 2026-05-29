param(
    [Parameter(Mandatory=$true)][string]$EngineA,
    [Parameter(Mandatory=$true)][string]$EngineB,
    [Parameter(Mandatory=$true)][string]$Arbiter,
    [string]$LabelA = "engine_a",
    [string]$LabelB = "engine_b",
    [int]$Games = 20,
    [uint32]$Seed = 20260529,
    [int]$Depth = 3,
    [int]$Threads = 1,
    [int]$Aspiration = 32,
    [int]$MoveTimeoutSeconds = 10,
    [int]$PlyCap = 128,
    [string]$TestDataDir = "",
    [Parameter(Mandatory=$true)][string]$OutputJson,
    [Parameter(Mandatory=$true)][string]$TranscriptPath
)

$ErrorActionPreference = "Stop"

function Assert-FileExists([string]$Path, [string]$Label) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
        throw "$Label not found: $Path"
    }
}

function Invoke-EngineJson {
    param(
        [Parameter(Mandatory=$true)][string]$Exe,
        [Parameter(Mandatory=$true)][string[]]$Arguments,
        [Parameter(Mandatory=$true)][int]$TimeoutSeconds,
        [Parameter(Mandatory=$true)][string]$WorkDir
    )

    $stdout = Join-Path $WorkDir ([guid]::NewGuid().ToString() + ".out")
    $stderr = Join-Path $WorkDir ([guid]::NewGuid().ToString() + ".err")
    $process = Start-Process -FilePath $Exe -ArgumentList $Arguments -NoNewWindow -PassThru `
        -RedirectStandardOutput $stdout -RedirectStandardError $stderr
    if (-not $process.WaitForExit($TimeoutSeconds * 1000)) {
        Stop-Process -Id $process.Id -Force -ErrorAction SilentlyContinue
        return [pscustomobject]@{
            TimedOut = $true
            Crashed = $false
            ExitCode = $null
            Stdout = if (Test-Path -LiteralPath $stdout) { Get-Content -LiteralPath $stdout -Raw } else { "" }
            Stderr = if (Test-Path -LiteralPath $stderr) { Get-Content -LiteralPath $stderr -Raw } else { "" }
            Json = $null
            Command = "$Exe $($Arguments -join ' ')"
        }
    }
    $process.WaitForExit()
    $process.Refresh()

    $outText = if (Test-Path -LiteralPath $stdout) { Get-Content -LiteralPath $stdout -Raw } else { "" }
    $errText = if (Test-Path -LiteralPath $stderr) { Get-Content -LiteralPath $stderr -Raw } else { "" }
    $json = $null
    $exitCode = if ($null -eq $process.ExitCode) { 0 } else { $process.ExitCode }
    $crashed = $exitCode -ne 0
    if (-not [string]::IsNullOrWhiteSpace($outText)) {
        try {
            $json = $outText | ConvertFrom-Json
        } catch {
            $crashed = $true
        }
    } else {
        $crashed = $true
    }

    [pscustomobject]@{
        TimedOut = $false
        Crashed = $crashed
        ExitCode = $exitCode
        Stdout = $outText
        Stderr = $errText
        Json = $json
        Command = "$Exe $($Arguments -join ' ')"
    }
}

function Convert-BestMoveToCompact([string]$MoveText) {
    $match = [regex]::Match($MoveText, '^\((\d+),\s*(\d+)\)\s*->\s*\((\d+),\s*(\d+)\)\s*\(([BW])\)$')
    if (-not $match.Success) {
        return $null
    }
    "{0},{1},{2},{3},{4}" -f $match.Groups[1].Value,$match.Groups[2].Value,$match.Groups[3].Value,$match.Groups[4].Value,$match.Groups[5].Value
}

function Get-CurrentPlayerFromFile([string]$Path) {
    $line = Get-Content -LiteralPath $Path | Where-Object { $_ -like "current_player:*" } | Select-Object -First 1
    if ($line -match 'current_player:\s*([BW])') {
        return $Matches[1]
    }
    throw "failed to read current_player from $Path"
}

function Get-OutcomeForA([string]$Winner, [string]$AColor, [bool]$AbnormalLoss, [string]$AbnormalSide) {
    if ($AbnormalLoss) {
        if ($AbnormalSide -eq "A") { return "loss" }
        if ($AbnormalSide -eq "B") { return "win" }
    }
    if ($Winner -eq ".") { return "draw" }
    if ($Winner -eq $AColor) { return "win" }
    return "loss"
}

Assert-FileExists $EngineA "EngineA"
Assert-FileExists $EngineB "EngineB"
Assert-FileExists $Arbiter "Arbiter"
if ([string]::IsNullOrWhiteSpace($TestDataDir)) {
    $TestDataDir = Join-Path (Split-Path -Parent $PSScriptRoot) "test\test_data"
}
New-Item -ItemType Directory -Force -Path (Split-Path -Parent $OutputJson) | Out-Null
New-Item -ItemType Directory -Force -Path (Split-Path -Parent $TranscriptPath) | Out-Null

$openingFiles = @(
    (Join-Path $TestDataDir "game1.txt"),
    (Join-Path $TestDataDir "game3.txt"),
    (Join-Path $TestDataDir "game6.txt"),
    (Join-Path $TestDataDir "game8.txt"),
    (Join-Path $TestDataDir "game10.txt"),
    (Join-Path $TestDataDir "screenshot_2026_05_06_same_file_capture.txt")
)
$openingFiles = $openingFiles | Where-Object { Test-Path -LiteralPath $_ -PathType Leaf }
if ($openingFiles.Count -eq 0) {
    throw "no opening files available"
}

$runDir = Join-Path (Split-Path -Parent $OutputJson) ("run-" + [guid]::NewGuid().ToString("N"))
New-Item -ItemType Directory -Force -Path $runDir | Out-Null

$summary = [ordered]@{
    a_wins = 0
    b_wins = 0
    draws = 0
    illegal_moves = 0
    timeouts = 0
    crashes = 0
    total_plies = 0
    average_plies = 0.0
}
$gamesOut = New-Object System.Collections.ArrayList
$transcript = New-Object System.Collections.ArrayList
[void]$transcript.Add("# Provincial P7 version match transcript")
[void]$transcript.Add("label_a=$LabelA")
[void]$transcript.Add("label_b=$LabelB")
[void]$transcript.Add("seed=$Seed games=$Games depth=$Depth threads=$Threads aspiration=$Aspiration timeout=$MoveTimeoutSeconds ply_cap=$PlyCap")

for ($gameIndex = 0; $gameIndex -lt $Games; $gameIndex++) {
    $pairIndex = [math]::Floor($gameIndex / 2)
    $opening = $openingFiles[($Seed + $pairIndex) % $openingFiles.Count]
    $aColor = if (($gameIndex % 2) -eq 0) { "B" } else { "W" }
    $bColor = if ($aColor -eq "B") { "W" } else { "B" }
    $currentFile = Join-Path $runDir ("game_{0:D3}_start.txt" -f $gameIndex)

    Copy-Item -LiteralPath $opening -Destination $currentFile -Force
    $openingLabel = Split-Path -Leaf $opening

    $moves = New-Object System.Collections.ArrayList
    $finalReason = "ply_cap"
    $winner = "."
    $abnormal = $false
    $abnormalSide = ""
    $illegal = $false
    $timeout = $false
    $crash = $false

    [void]$transcript.Add("")
    [void]$transcript.Add("[game $gameIndex] opening=$openingLabel A=$aColor B=$bColor")

    for ($ply = 0; $ply -lt $PlyCap; $ply++) {
        $side = Get-CurrentPlayerFromFile $currentFile
        $engineSide = if ($side -eq $aColor) { "A" } else { "B" }
        $engineExe = if ($engineSide -eq "A") { $EngineA } else { $EngineB }
        $engineLabel = if ($engineSide -eq "A") { $LabelA } else { $LabelB }
        $search = Invoke-EngineJson -Exe $engineExe -Arguments @(
            "bitboard-search", "--file", $currentFile, "--depth", "$Depth", "--threads", "$Threads",
            "--aspiration", "$Aspiration", "--format", "json", "--case-id", "g${gameIndex}_p${ply}"
        ) -TimeoutSeconds $MoveTimeoutSeconds -WorkDir $runDir

        if ($search.TimedOut) {
            $timeout = $true
            $abnormal = $true
            $abnormalSide = $engineSide
            $finalReason = "timeout"
            break
        }
        if ($search.Crashed -or $null -eq $search.Json) {
            $crash = $true
            $abnormal = $true
            $abnormalSide = $engineSide
            $finalReason = "crash"
            break
        }

        $bestMove = [string]$search.Json.best_move
        $compactMove = Convert-BestMoveToCompact $bestMove
        if ($null -eq $compactMove) {
            $illegal = $true
            $abnormal = $true
            $abnormalSide = $engineSide
            $finalReason = "invalid_best_move_format"
            break
        }

        $nextFile = Join-Path $runDir ("game_{0:D3}_ply_{1:D3}.txt" -f $gameIndex, ($ply + 1))
        $apply = Invoke-EngineJson -Exe $Arbiter -Arguments @(
            "bitboard-apply-move", "--file", $currentFile, "--move", $compactMove,
            "--output", $nextFile, "--format", "json"
        ) -TimeoutSeconds $MoveTimeoutSeconds -WorkDir $runDir

        if ($apply.TimedOut) {
            $timeout = $true
            $abnormal = $true
            $abnormalSide = "arbiter"
            $finalReason = "arbiter_timeout"
            break
        }
        if ($apply.Crashed -or $null -eq $apply.Json) {
            $crash = $true
            $abnormal = $true
            $abnormalSide = "arbiter"
            $finalReason = "arbiter_crash"
            break
        }
        if (-not [bool]$apply.Json.legal) {
            $illegal = $true
            $abnormal = $true
            $abnormalSide = $engineSide
            $finalReason = "illegal_move"
            break
        }

        [void]$moves.Add([pscustomobject]@{
            ply = $ply
            side = $side
            engine_side = $engineSide
            engine = $engineLabel
            move = $bestMove
            score = $search.Json.score
            nodes = $search.Json.nodes
            move_reason = $apply.Json.move_reason
        })
        [void]$transcript.Add(("{0}. {1} {2} {3} score={4}" -f ($ply + 1),$side,$engineLabel,$bestMove,$search.Json.score))
        $currentFile = $nextFile

        if ([bool]$apply.Json.end) {
            $finalReason = [string]$apply.Json.end_reason
            $winner = [string]$apply.Json.winner
            break
        }
    }

    if ($finalReason -eq "ply_cap") {
        $winner = "."
    }
    if ($illegal) { $summary.illegal_moves++ }
    if ($timeout) { $summary.timeouts++ }
    if ($crash) { $summary.crashes++ }

    $outcomeA = Get-OutcomeForA -Winner $winner -AColor $aColor -AbnormalLoss $abnormal -AbnormalSide $abnormalSide
    if ($outcomeA -eq "win") {
        $summary.a_wins++
    } elseif ($outcomeA -eq "loss") {
        $summary.b_wins++
    } else {
        $summary.draws++
    }
    $summary.total_plies += $moves.Count
    [void]$transcript.Add("[result] outcome_a=$outcomeA winner=$winner reason=$finalReason plies=$($moves.Count) illegal=$illegal timeout=$timeout crash=$crash")

    [void]$gamesOut.Add([pscustomobject]@{
        game_index = $gameIndex
        opening = $openingLabel
        engine_a_color = $aColor
        engine_b_color = $bColor
        outcome_for_a = $outcomeA
        winner = $winner
        final_reason = $finalReason
        plies = $moves.Count
        illegal = $illegal
        timeout = $timeout
        crash = $crash
        abnormal_side = $abnormalSide
        moves = @($moves)
    })
}

if ($Games -gt 0) {
    $summary.average_plies = [math]::Round($summary.total_plies / [double]$Games, 3)
}

$report = [ordered]@{
    phase = "provincial_p7_version_match_gate"
    generated_at = (Get-Date).ToString("o")
    label_a = $LabelA
    label_b = $LabelB
    engine_a = (Resolve-Path -LiteralPath $EngineA).Path
    engine_b = (Resolve-Path -LiteralPath $EngineB).Path
    arbiter = (Resolve-Path -LiteralPath $Arbiter).Path
    seed = $Seed
    games_requested = $Games
    depth = $Depth
    threads = $Threads
    aspiration = $Aspiration
    move_timeout_seconds = $MoveTimeoutSeconds
    ply_cap = $PlyCap
    openings = @($openingFiles)
    summary = $summary
    games = @($gamesOut)
}

($report | ConvertTo-Json -Depth 20) | Set-Content -LiteralPath $OutputJson -Encoding UTF8
$transcript | Set-Content -LiteralPath $TranscriptPath -Encoding UTF8
$report | ConvertTo-Json -Depth 20
