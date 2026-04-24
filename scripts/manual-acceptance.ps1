param(
    [string]$BuildDir,
    [int]$Depth = 4,
    [int]$Threads = 1
)

. (Join-Path $PSScriptRoot 'common.ps1')

$repoRoot = Get-SurakartaRepoRoot
if (-not $BuildDir) {
    $BuildDir = Join-Path $repoRoot 'build-vs-manual-release'
}

$guiExe = Join-Path $BuildDir 'bin\surakarta-gui.exe'
$timestamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$runRoot = Join-Path $repoRoot "build-local\manual-acceptance\$timestamp"
$screenshotsDir = Join-Path $runRoot 'screenshots'
$commandsPath = Join-Path $runRoot 'commands.txt'
$resultPath = Join-Path $runRoot 'result.txt'
$gitCommitPath = Join-Path $runRoot 'git-commit.txt'
$checklistTemplate = Join-Path $repoRoot 'test\manual_acceptance_checklist.md'
$checklistCopy = Join-Path $runRoot 'checklist.md'
$game1 = Join-Path $repoRoot 'test\test_data\game1.txt'
$game2 = Join-Path $repoRoot 'test\test_data\game2.txt'
$game6 = Join-Path $repoRoot 'test\test_data\game6.txt'
$game7 = Join-Path $repoRoot 'test\test_data\game7.txt'

Invoke-SurakartaConfigure -Configuration 'Release' -BuildDir $BuildDir
Invoke-SurakartaBuild -BuildDir $BuildDir -Targets @('surakarta-gui')

New-Item -ItemType Directory -Force -Path $runRoot, $screenshotsDir | Out-Null
Stop-SurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot | Out-Null

if (-not (Test-Path $checklistTemplate)) {
    throw "Missing manual acceptance checklist template: $checklistTemplate"
}
Copy-Item -Path $checklistTemplate -Destination $checklistCopy -Force

$gitCommit = 'UNAVAILABLE'
$gitStatus = ''
if (Test-Path (Join-Path $repoRoot '.git')) {
    try {
        $gitCommit = (& git -C $repoRoot rev-parse HEAD).Trim()
        $gitStatus = (& git -C $repoRoot status --short) -join [Environment]::NewLine
    } catch {
        $gitCommit = 'UNAVAILABLE'
        $gitStatus = $_.Exception.Message
    }
}

@(
    "build_command: pwsh ./scripts/manual-acceptance.ps1 -BuildDir `"$BuildDir`" -Depth $Depth -Threads $Threads"
    "launch_command: `"$guiExe`" --human black --depth $Depth --threads $Threads"
    "load_game1: use GUI Load Position -> $game1"
    "load_game2: use GUI Load Position -> $game2"
    "load_game6: use GUI Load Position -> $game6"
    "game7_reference: use GUI Load Position -> $game7"
    "screenshots_dir: $screenshotsDir"
) | Set-Content -Path $commandsPath

@(
    "git_commit: $gitCommit"
    "git_status:"
    $gitStatus
) | Set-Content -Path $gitCommitPath

@(
    "Manual acceptance result: PENDING"
    "Fill checklist.md with actual observations."
    "Store screenshots under screenshots\\."
    "Required screenshots:"
    "- opening-move.png"
    "- game1-targets.png"
    "- game6-checkmate.png"
    "- game6-stalemate.png"
) | Set-Content -Path $resultPath

Start-Process -FilePath $guiExe `
    -ArgumentList @('--human', 'black', '--depth', "$Depth", '--threads', "$Threads") `
    -WorkingDirectory (Join-Path $BuildDir 'bin') | Out-Null

Write-Host "[PASS] manual acceptance scaffold ready"
Write-Host "Run root: $runRoot"
Write-Host "Checklist copy: $checklistCopy"
Write-Host "Result file: $resultPath"
