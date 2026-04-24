param(
    [string]$BuildDir,
    [int]$Games = 4,
    [int]$Depth = 4,
    [int]$Seed = 20260423,
    [int]$CheckpointEvery = 2
)

. (Join-Path $PSScriptRoot 'common.ps1')

if ($Games -le 0) {
    throw 'Training smoke requires -Games to be greater than 0.'
}

if ($CheckpointEvery -le 0) {
    throw 'Training smoke requires -CheckpointEvery to be greater than 0 so at least one checkpoint is archived.'
}

$repoRoot = Get-SurakartaRepoRoot
if (-not $BuildDir) {
    $BuildDir = Join-Path $repoRoot 'build-vs-train-release'
}

$benchmarkExe = Join-Path $BuildDir 'bin\surakarta-benchmark.exe'
$outputRoot = Join-Path $repoRoot 'build-local\training'
$timestamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$sessionRoot = Join-Path $outputRoot $timestamp
$middlegameFile = Join-Path $repoRoot 'test\test_data\game1.txt'
$reproJsonPath = Join-Path $sessionRoot 'repro-summary.json'
$reproTextPath = Join-Path $sessionRoot 'repro-summary.txt'

function New-TrainingRunLayout {
    param(
        [string]$Root,
        [string]$Name
    )

    $runRoot = Join-Path $Root $Name
    $weightsDir = Join-Path $runRoot 'weights'
    $checkpointsDir = Join-Path $runRoot 'checkpoints'
    $logsDir = Join-Path $runRoot 'logs'

    New-Item -ItemType Directory -Force -Path $weightsDir, $checkpointsDir, $logsDir | Out-Null

    return [pscustomobject]@{
        Name = $Name
        RunRoot = $runRoot
        WeightsDir = $weightsDir
        CheckpointsDir = $checkpointsDir
        LogsDir = $logsDir
        BootstrapWeights = Join-Path $weightsDir 'bootstrap.bin'
        CandidateWeights = Join-Path $weightsDir 'candidate.bin'
        EvalJsonPath = Join-Path $runRoot 'eval-summary.json'
        EvalTextPath = Join-Path $runRoot 'eval-summary.txt'
    }
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

    Wait-Process -Id $process.Id
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

function Get-CheckpointHashes {
    param([string]$Directory)

    $files = @(Get-ChildItem -Path $Directory -Filter '*.bin' -File | Sort-Object Name)
    if ($files.Count -eq 0) {
        throw "Training smoke requires at least one checkpoint in $Directory."
    }

    $hashes = [ordered]@{}
    foreach ($file in $files) {
        $hashes[$file.Name] = Get-FileSha256 -Path $file.FullName
    }
    return $hashes
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

function Get-StringSha256 {
    param([string]$Content)

    $bytes = [System.Text.Encoding]::UTF8.GetBytes($Content)
    $sha256 = [System.Security.Cryptography.SHA256]::Create()
    try {
        $hashBytes = $sha256.ComputeHash($bytes)
    } finally {
        $sha256.Dispose()
    }

    return ([System.BitConverter]::ToString($hashBytes)).Replace('-', '')
}

function Normalize-SignatureString {
    param(
        [AllowNull()]
        [string]$Value,
        [hashtable]$PathTokens
    )

    if ($null -eq $Value) {
        return $null
    }

    $normalized = $Value
    foreach ($path in ($PathTokens.Keys | Sort-Object { $_.Length } -Descending)) {
        if (-not [string]::IsNullOrEmpty($path)) {
            $normalized = $normalized.Replace($path, $PathTokens[$path])
        }
    }

    return $normalized
}

function Normalize-EvalJsonValue {
    param(
        $Value,
        [hashtable]$PathTokens
    )

    if ($null -eq $Value) {
        return $null
    }

    if ($Value -is [System.Collections.IDictionary]) {
        $normalizedObject = [ordered]@{}
        foreach ($key in $Value.Keys) {
            if ($key -eq 'search_seconds') {
                continue
            }

            $normalizedObject[$key] = Normalize-EvalJsonValue -Value $Value[$key] -PathTokens $PathTokens
        }
        return $normalizedObject
    }

    if (($Value -is [System.Collections.IEnumerable]) -and -not ($Value -is [string])) {
        $normalizedItems = @()
        foreach ($item in $Value) {
            $normalizedItems += ,(Normalize-EvalJsonValue -Value $item -PathTokens $PathTokens)
        }
        return $normalizedItems
    }

    if ($Value -is [string]) {
        return Normalize-SignatureString -Value $Value -PathTokens $PathTokens
    }

    return $Value
}

function Get-EvalSignatureTokens {
    param([pscustomobject]$Layout)

    return @{
        $Layout.CandidateWeights = '<CANDIDATE_WEIGHTS>'
        $Layout.BootstrapWeights = '<BASELINE_WEIGHTS>'
        $Layout.RunRoot = '<RUN_ROOT>'
    }
}

function Get-NormalizedEvalJsonHash {
    param([pscustomobject]$Layout)

    $raw = Get-Content -Path $Layout.EvalJsonPath -Raw
    $payload = $raw | ConvertFrom-Json -AsHashtable
    $normalizedPayload = Normalize-EvalJsonValue -Value $payload -PathTokens (Get-EvalSignatureTokens -Layout $Layout)
    $normalizedJson = $normalizedPayload | ConvertTo-Json -Depth 16 -Compress
    return Get-StringSha256 -Content $normalizedJson
}

function Get-NormalizedEvalTextHash {
    param([pscustomobject]$Layout)

    $raw = Get-Content -Path $Layout.EvalTextPath -Raw
    $normalized = Normalize-SignatureString -Value $raw -PathTokens (Get-EvalSignatureTokens -Layout $Layout)
    return Get-StringSha256 -Content $normalized
}

function Run-TrainingSmokePass {
    param([pscustomobject]$Layout)

    $null = Invoke-LoggedBenchmarkCommand -Name 'bootstrap-export' -LogsDir $Layout.LogsDir -Arguments @(
        'bitboard-train',
        '--output', $Layout.BootstrapWeights,
        '--games', '0',
        '--format', 'json'
    )

    $trainArgs = @(
        'bitboard-train',
        '--output', $Layout.CandidateWeights,
        '--weights', $Layout.BootstrapWeights,
        '--games', "$Games",
        '--depth', "$Depth",
        '--seed', "$Seed",
        '--format', 'json',
        '--checkpoint-every', "$CheckpointEvery",
        '--checkpoint-dir', $Layout.CheckpointsDir
    )
    $candidate = Invoke-LoggedBenchmarkCommand -Name 'candidate-train' -LogsDir $Layout.LogsDir -Arguments $trainArgs
    $candidateSummary = $candidate.StdOut | ConvertFrom-Json

    $null = Invoke-LoggedBenchmarkCommand -Name 'candidate-search-load-smoke' -LogsDir $Layout.LogsDir -Arguments @(
        'bitboard-search',
        '--weights', $Layout.CandidateWeights,
        '--format', 'json',
        '--case-id', 'middlegame',
        '--file', $middlegameFile,
        '--depth', "$Depth",
        '--threads', '1'
    )

    $null = Invoke-LoggedBenchmarkCommand -Name 'candidate-benchmark-load-smoke' -LogsDir $Layout.LogsDir -Arguments @(
        'bitboard-benchmark',
        '--weights', $Layout.CandidateWeights,
        '--format', 'json',
        '--case-id', 'middlegame',
        '--file', $middlegameFile,
        '--depth', "$Depth",
        '--threads', '1',
        '--movegen-iters', '256'
    )

    $evalJson = Invoke-LoggedBenchmarkCommand -Name 'candidate-eval-json' -LogsDir $Layout.LogsDir -Arguments @(
        'bitboard-eval',
        '--candidate', $Layout.CandidateWeights,
        '--baseline', $Layout.BootstrapWeights,
        '--depth', "$Depth",
        '--format', 'json'
    )
    $evalJson.StdOut | Set-Content -Path $Layout.EvalJsonPath

    $evalText = Invoke-LoggedBenchmarkCommand -Name 'candidate-eval-text' -LogsDir $Layout.LogsDir -Arguments @(
        'bitboard-eval',
        '--candidate', $Layout.CandidateWeights,
        '--baseline', $Layout.BootstrapWeights,
        '--depth', "$Depth",
        '--format', 'text'
    )
    $evalText.StdOut | Set-Content -Path $Layout.EvalTextPath

    $bootstrapHash = Get-FileSha256 -Path $Layout.BootstrapWeights
    $candidateHash = Get-FileSha256 -Path $Layout.CandidateWeights
    if ($bootstrapHash -eq $candidateHash) {
        throw "Training smoke requires candidate weights to differ from bootstrap weights in $($Layout.RunRoot)."
    }

    Assert-JsonPayloadPresent -Path $Layout.EvalJsonPath -Label "$($Layout.Name) eval-summary.json"
    if (-not (Test-Path $Layout.EvalTextPath)) {
        throw "Missing eval text report: $($Layout.EvalTextPath)"
    }

    $checkpointHashes = Get-CheckpointHashes -Directory $Layout.CheckpointsDir
    $signature = [ordered]@{
        bootstrap = $bootstrapHash
        candidate = $candidateHash
        eval_json = Get-NormalizedEvalJsonHash -Layout $Layout
        eval_text = Get-NormalizedEvalTextHash -Layout $Layout
        checkpoints = $checkpointHashes
    }

    return [pscustomobject]@{
        Name = $Layout.Name
        RunRoot = $Layout.RunRoot
        CandidateSummary = $candidateSummary
        Signature = $signature
    }
}

function Compare-RunSignatures {
    param(
        [pscustomobject]$First,
        [pscustomobject]$Second
    )

    $left = $First.Signature | ConvertTo-Json -Depth 6 -Compress
    $right = $Second.Signature | ConvertTo-Json -Depth 6 -Compress
    return $left -eq $right
}

Invoke-SurakartaConfigure -Configuration 'Release' -BuildDir $BuildDir
Invoke-SurakartaBuild -BuildDir $BuildDir -Targets @('surakarta-benchmark')

New-Item -ItemType Directory -Force -Path $sessionRoot | Out-Null
Stop-SurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot | Out-Null

$runOne = Run-TrainingSmokePass -Layout (New-TrainingRunLayout -Root $sessionRoot -Name 'run-1')
$runTwo = Run-TrainingSmokePass -Layout (New-TrainingRunLayout -Root $sessionRoot -Name 'run-2')

if (-not (Compare-RunSignatures -First $runOne -Second $runTwo)) {
    throw "Training smoke is not reproducible for seed $Seed. Compare $($runOne.RunRoot) and $($runTwo.RunRoot)."
}

$reproSummary = [ordered]@{
    seed = $Seed
    games = $Games
    depth = $Depth
    checkpoint_every = $CheckpointEvery
    reproducible = $true
    runs = @(
        [ordered]@{
            name = $runOne.Name
            root = $runOne.RunRoot
            signature = $runOne.Signature
        },
        [ordered]@{
            name = $runTwo.Name
            root = $runTwo.RunRoot
            signature = $runTwo.Signature
        }
    )
}

$reproSummary | ConvertTo-Json -Depth 8 | Set-Content -Path $reproJsonPath
@(
    "seed: $Seed"
    "games: $Games"
    "depth: $Depth"
    "checkpoint_every: $CheckpointEvery"
    "reproducible: True"
    "run-1: $($runOne.RunRoot)"
    "run-2: $($runTwo.RunRoot)"
) | Set-Content -Path $reproTextPath

Assert-NoSurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot

Write-Host "[PASS] benchmark training smoke gate"
Write-Host "Session root: $sessionRoot"
Write-Host "Repro summary JSON: $reproJsonPath"
Write-Host "Repro summary TXT: $reproTextPath"
