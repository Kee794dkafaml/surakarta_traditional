param(
    [string]$BuildDir,
    [int]$Games = 4,
    [int]$Depth = 4,
    [int]$Seed = 20260423,
    [int]$CheckpointEvery = 2
)

. (Join-Path $PSScriptRoot 'common.ps1')

$repoRoot = Get-SurakartaRepoRoot
if (-not $BuildDir) {
    $BuildDir = Join-Path $repoRoot 'build-vs-train-release'
}

$benchmarkExe = Join-Path $BuildDir 'bin\surakarta-benchmark.exe'
$outputRoot = Join-Path $repoRoot 'build-local\training'
$timestamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$runRoot = Join-Path $outputRoot $timestamp
$weightsDir = Join-Path $runRoot 'weights'
$checkpointsDir = Join-Path $runRoot 'checkpoints'
$logsDir = Join-Path $runRoot 'logs'
$middlegameFile = Join-Path $repoRoot 'test\test_data\game1.txt'

Invoke-SurakartaConfigure -Configuration 'Release' -BuildDir $BuildDir
Invoke-SurakartaBuild -BuildDir $BuildDir -Targets @('surakarta-benchmark')

New-Item -ItemType Directory -Force -Path $weightsDir, $checkpointsDir, $logsDir | Out-Null
Stop-SurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot | Out-Null

$bootstrapWeights = Join-Path $weightsDir 'bootstrap.bin'
$candidateWeights = Join-Path $weightsDir 'candidate.bin'
$evalJsonPath = Join-Path $runRoot 'eval-summary.json'
$evalTextPath = Join-Path $runRoot 'eval-summary.txt'

function Invoke-LoggedBenchmarkCommand {
    param(
        [string]$Name,
        [string[]]$Arguments
    )

    $stdoutLog = Join-Path $logsDir "$Name.stdout.log"
    $stderrLog = Join-Path $logsDir "$Name.stderr.log"

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

$bootstrap = Invoke-LoggedBenchmarkCommand -Name 'bootstrap-export' -Arguments @(
    'bitboard-train',
    '--output', $bootstrapWeights,
    '--games', '0',
    '--format', 'json'
)

$trainArgs = @(
    'bitboard-train',
    '--output', $candidateWeights,
    '--weights', $bootstrapWeights,
    '--games', "$Games",
    '--depth', "$Depth",
    '--seed', "$Seed",
    '--format', 'json'
)
if ($CheckpointEvery -gt 0) {
    $trainArgs += @('--checkpoint-every', "$CheckpointEvery", '--checkpoint-dir', $checkpointsDir)
}
$candidate = Invoke-LoggedBenchmarkCommand -Name 'candidate-train' -Arguments $trainArgs

$null = Invoke-LoggedBenchmarkCommand -Name 'candidate-load-smoke' -Arguments @(
    'bitboard-search',
    '--weights', $candidateWeights,
    '--format', 'json',
    '--case-id', 'middlegame',
    '--file', $middlegameFile,
    '--depth', "$Depth",
    '--threads', '1'
)

$evalJson = Invoke-LoggedBenchmarkCommand -Name 'candidate-eval-json' -Arguments @(
    'bitboard-eval',
    '--candidate', $candidateWeights,
    '--baseline', $bootstrapWeights,
    '--depth', "$Depth",
    '--format', 'json'
)
$evalJson.StdOut | Set-Content -Path $evalJsonPath

$evalText = Invoke-LoggedBenchmarkCommand -Name 'candidate-eval-text' -Arguments @(
    'bitboard-eval',
    '--candidate', $candidateWeights,
    '--baseline', $bootstrapWeights,
    '--depth', "$Depth",
    '--format', 'text'
)
$evalText.StdOut | Set-Content -Path $evalTextPath

Assert-NoSurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot

Write-Host "[PASS] benchmark training loop"
Write-Host "Run root: $runRoot"
Write-Host "Bootstrap: $bootstrapWeights"
Write-Host "Candidate: $candidateWeights"
Write-Host "Eval JSON: $evalJsonPath"
Write-Host "Eval Text: $evalTextPath"
