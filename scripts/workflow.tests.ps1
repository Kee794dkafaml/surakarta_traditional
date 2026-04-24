$here = Split-Path -Parent $MyInvocation.MyCommand.Path

function New-BuildHarness {
    param(
        [string]$Sandbox,
        [switch]$CleanFirst
    )

    $commonPath = Join-Path $here 'common.ps1'
    $cleanFirstArgument = ''
    if ($CleanFirst.IsPresent) {
        $cleanFirstArgument = ' -CleanFirst'
    }

    @"
. '$commonPath'

function Add-Call {
    param([string]`$Line)

    Add-Content -Path (Join-Path `$PSScriptRoot 'calls.log') -Value `$Line
}

function Import-SurakartaDevShell {
    param([string]`$BuildDir)

    Add-Call "import|`$BuildDir"
}

function Get-NinjaPath {
    param([string]`$BuildDir)

    return 'fake-ninja'
}

function fake-ninja {
    param([Parameter(ValueFromRemainingArguments = `$true)][string[]]`$Args)

    Add-Call "ninja|`$([string]::Join('|', `$Args))"
    `$global:LASTEXITCODE = 0
}

function Assert-LastNativeExitCode {
    param([string]`$Step)

    Add-Call "assert|`$Step"
}

function Write-Host {
    param([Parameter(ValueFromRemainingArguments = `$true)][object[]]`$Message)

    Add-Call "host|`$(`$Message -join ' ')"
}

Invoke-SurakartaBuild -BuildDir 'C:\build-dir'$cleanFirstArgument -Targets @('alpha', 'beta')
"@ | Set-Content -Path (Join-Path $Sandbox 'run-build.ps1')
}

function New-ConfigureHarness {
    param(
        [string]$Sandbox,
        [string]$CacheSourceDir
    )

    $scriptsDir = Join-Path $Sandbox 'scripts'
    $buildDir = Join-Path $Sandbox 'build-vs-release'
    New-Item -ItemType Directory -Path $scriptsDir, $buildDir -Force | Out-Null

    Copy-Item -Path (Join-Path $here 'common.ps1') -Destination (Join-Path $scriptsDir 'common.ps1')
    "CMAKE_HOME_DIRECTORY:INTERNAL=$CacheSourceDir" | Set-Content -Path (Join-Path $buildDir 'CMakeCache.txt')

    @"
. (Join-Path `$PSScriptRoot 'common.ps1')

function Add-Call {
    param([string]`$Line)

    Add-Content -Path (Join-Path `$PSScriptRoot 'calls.log') -Value `$Line
}

function Import-SurakartaDevShell {
    param([string]`$BuildDir)

    Add-Call "import|`$BuildDir"
}

function Get-CMakePath {
    param([string]`$BuildDir)

    return 'fake-cmake'
}

function fake-cmake {
    param([Parameter(ValueFromRemainingArguments = `$true)][string[]]`$Args)

    Add-Call "cmake|`$([string]::Join('|', `$Args))"
    `$global:LASTEXITCODE = 0
}

function Assert-LastNativeExitCode {
    param([string]`$Step)

    Add-Call "assert|`$Step"
}

function Remove-Item {
    param(
        [string]`$LiteralPath,
        [switch]`$Recurse,
        [switch]`$Force
    )

    Add-Call "remove|`$LiteralPath|`$(`$Recurse.IsPresent)|`$(`$Force.IsPresent)"
}

Invoke-SurakartaConfigure -Configuration Release -BuildDir '$buildDir'
"@ | Set-Content -Path (Join-Path $scriptsDir 'run-configure.ps1')
}

function New-BenchmarkHarness {
    param(
        [string]$Sandbox,
        [bool]$ShouldMismatch = $false
    )

    $scriptsDir = Join-Path $Sandbox 'scripts'
    $testDataDir = Join-Path $Sandbox 'test\test_data'
    New-Item -ItemType Directory -Path $scriptsDir, $testDataDir -Force | Out-Null

    Copy-Item -Path (Join-Path $here 'benchmark-train.ps1') -Destination (Join-Path $scriptsDir 'benchmark-train.ps1')

    "middlegame" | Set-Content -Path (Join-Path $testDataDir 'game1.txt')

    @"
function Add-Call {
    param([string]`$Line)

    Add-Content -Path (Join-Path `$PSScriptRoot 'calls.log') -Value `$Line
}

function Get-SurakartaRepoRoot {
    return (Resolve-Path (Join-Path `$PSScriptRoot '..')).Path
}

function Invoke-SurakartaConfigure {
    param([string]`$Configuration, [string]`$BuildDir)

    Add-Call "configure|`$Configuration|`$BuildDir"
}

function Invoke-SurakartaBuild {
    param([string]`$BuildDir, [string[]]`$Targets)

    Add-Call "build|`$BuildDir|`$([string]::Join(',', `$Targets))"
}

function Stop-SurakartaWorkspaceProcesses {
    param([string]`$WorkspaceRoot)

    Add-Call "stop|`$WorkspaceRoot"
    return @()
}

function Assert-NoSurakartaWorkspaceProcesses {
    param([string]`$WorkspaceRoot)

    Add-Call "assert-no-processes|`$WorkspaceRoot"
}
"@ | Set-Content -Path (Join-Path $scriptsDir 'common.ps1')

    @"
`$script:NextPid = 0
`$script:TrainInvocation = 0
`$script:ShouldMismatch = [System.Convert]::ToBoolean('$ShouldMismatch')

function Get-Date {
    param([string]`$Format)

    if (`$Format) {
        return '20260424-120000'
    }

    return Microsoft.PowerShell.Utility\Get-Date
}

function Start-Process {
    param(
        [string]`$FilePath,
        [string[]]`$ArgumentList,
        [string]`$WorkingDirectory,
        [string]`$RedirectStandardOutput,
        [string]`$RedirectStandardError,
        [switch]`$NoNewWindow,
        [switch]`$PassThru
    )

    Add-Call "start|`$FilePath|`$([string]::Join(' ', `$ArgumentList))|`$WorkingDirectory"
    New-Item -ItemType Directory -Force -Path ([System.IO.Path]::GetDirectoryName(`$RedirectStandardOutput)) | Out-Null
    '' | Set-Content -Path `$RedirectStandardError

    `$command = `$ArgumentList[0]
    `$stdout = ''

    switch (`$command) {
        'bitboard-train' {
            `$outputIndex = [Array]::IndexOf(`$ArgumentList, '--output')
            `$outputPath = `$ArgumentList[`$outputIndex + 1]
            `$gamesIndex = [Array]::IndexOf(`$ArgumentList, '--games')
            `$games = [int]`$ArgumentList[`$gamesIndex + 1]
            New-Item -ItemType Directory -Force -Path ([System.IO.Path]::GetDirectoryName(`$outputPath)) | Out-Null

            if (`$games -eq 0) {
                [System.IO.File]::WriteAllText(`$outputPath, 'bootstrap-fixed')
                `$stdout = '{"games_requested":0,"games_completed":0,"checkpoint_count":0}'
            } else {
                `$script:TrainInvocation += 1
                `$candidateContent = if (`$script:ShouldMismatch -and `$script:TrainInvocation -eq 2) { 'candidate-run-2' } else { 'candidate-fixed' }
                [System.IO.File]::WriteAllText(`$outputPath, `$candidateContent)

                `$checkpointDirIndex = [Array]::IndexOf(`$ArgumentList, '--checkpoint-dir')
                if (`$checkpointDirIndex -ge 0) {
                    `$checkpointDir = `$ArgumentList[`$checkpointDirIndex + 1]
                    New-Item -ItemType Directory -Force -Path `$checkpointDir | Out-Null
                    `$checkpointContent = if (`$script:ShouldMismatch -and `$script:TrainInvocation -eq 2) { 'checkpoint-run-2' } else { 'checkpoint-fixed' }
                    [System.IO.File]::WriteAllText((Join-Path `$checkpointDir 'checkpoint-2.bin'), `$checkpointContent)
                }

                `$stdout = '{"games_requested":4,"games_completed":4,"checkpoint_count":1}'
            }
        }
        'bitboard-search' {
            `$stdout = '{"case_id":"middlegame","best_move":"(4, 4) -> (5, 1) (B)"}'
        }
        'bitboard-benchmark' {
            `$stdout = '{"case_id":"middlegame","nodes":256}'
        }
        'bitboard-eval' {
            `$candidateIndex = [Array]::IndexOf(`$ArgumentList, '--candidate')
            `$baselineIndex = [Array]::IndexOf(`$ArgumentList, '--baseline')
            `$candidatePath = `$ArgumentList[`$candidateIndex + 1]
            `$baselinePath = `$ArgumentList[`$baselineIndex + 1]
            `$candidateJsonPath = `$candidatePath.Replace('\', '\\')
            `$baselineJsonPath = `$baselinePath.Replace('\', '\\')
            `$candidateSearchSeconds = if (`$candidatePath -like '*run-2*') { '0.002000' } else { '0.001000' }
            `$baselineSearchSeconds = if (`$baselinePath -like '*run-2*') { '0.004000' } else { '0.003000' }
            if (`$ArgumentList -contains 'json') {
                `$stdout = "{""candidate_weights"":""`$candidateJsonPath"",""baseline_weights"":""`$baselineJsonPath"",""position_reports"":[{""case_id"":""middlegame"",""candidate"":{""search_seconds"":`$candidateSearchSeconds,""nodes"":128},""baseline"":{""search_seconds"":`$baselineSearchSeconds,""nodes"":64}}],""match_summary"":{""candidate_wins"":3,""baseline_wins"":2,""draws"":1}}"
            } else {
                `$stdout = "candidate: `$candidatePath`nbaseline: `$baselinePath`ncandidate_wins: 3"
            }
        }
        default {
            throw "Unexpected command: `$command"
        }
    }

    `$stdout | Set-Content -Path `$RedirectStandardOutput

    `$process = [pscustomobject]@{
        Id = (`$script:NextPid += 1)
        ExitCode = 0
    }
    `$process | Add-Member -MemberType ScriptMethod -Name Refresh -Value { }
    return `$process
}

function Wait-Process {
    param([int]`$Id)
}

. (Join-Path `$PSScriptRoot 'common.ps1')
. (Join-Path `$PSScriptRoot 'benchmark-train.ps1')
"@ | Set-Content -Path (Join-Path $scriptsDir 'benchmark-harness.ps1')
}

function New-ManualAcceptanceHarness {
    param([string]$Sandbox)

    $scriptsDir = Join-Path $Sandbox 'scripts'
    $testDir = Join-Path $Sandbox 'test'
    New-Item -ItemType Directory -Path $scriptsDir, $testDir -Force | Out-Null

    Copy-Item -Path (Join-Path $here 'manual-acceptance.ps1') -Destination (Join-Path $scriptsDir 'manual-acceptance.ps1')
    @'
# checklist template
'@ | Set-Content -Path (Join-Path $testDir 'manual_acceptance_checklist.md')

    Push-Location $Sandbox
    git init -b main | Out-Null
    git config user.name 'Pester Harness'
    git config user.email 'pester@example.com'
    'baseline' | Set-Content -Path 'README.md'
    git add README.md
    git commit -m 'init' | Out-Null
    Pop-Location

    @"
function Add-Call {
    param([string]`$Line)

    Add-Content -Path (Join-Path `$PSScriptRoot 'calls.log') -Value `$Line
}

function Get-SurakartaRepoRoot {
    return (Resolve-Path (Join-Path `$PSScriptRoot '..')).Path
}

function Invoke-SurakartaConfigure {
    param([string]`$Configuration, [string]`$BuildDir)

    Add-Call "configure|`$Configuration|`$BuildDir"
}

function Invoke-SurakartaBuild {
    param([string]`$BuildDir, [string[]]`$Targets)

    Add-Call "build|`$BuildDir|`$([string]::Join(',', `$Targets))"
}

function Stop-SurakartaWorkspaceProcesses {
    param([string]`$WorkspaceRoot)

    Add-Call "stop|`$WorkspaceRoot"
    return @()
}

function Assert-NoSurakartaWorkspaceProcesses {
    param([string]`$WorkspaceRoot)

    Add-Call "assert-no-processes|`$WorkspaceRoot"
}
"@ | Set-Content -Path (Join-Path $scriptsDir 'common.ps1')

    @"
function Get-Date {
    param([string]`$Format)

    if (`$Format) {
        return '20260424-130000'
    }

    return Microsoft.PowerShell.Utility\Get-Date
}

function Start-Process {
    param(
        [string]`$FilePath,
        [string[]]`$ArgumentList,
        [string]`$WorkingDirectory,
        [switch]`$PassThru
    )

    Add-Call "start|`$FilePath|`$([string]::Join(' ', `$ArgumentList))|`$WorkingDirectory"
    return [pscustomobject]@{ Id = 1 }
}

. (Join-Path `$PSScriptRoot 'common.ps1')
. (Join-Path `$PSScriptRoot 'manual-acceptance.ps1')
"@ | Set-Content -Path (Join-Path $scriptsDir 'manual-harness.ps1')
}

Describe 'Invoke-SurakartaBuild' {
    BeforeEach {
        $script:Sandbox = Join-Path ([System.IO.Path]::GetTempPath()) ([System.Guid]::NewGuid().ToString())
        New-Item -ItemType Directory -Path $script:Sandbox | Out-Null
    }

    AfterEach {
        if (Test-Path $script:Sandbox) {
            Remove-Item -Path $script:Sandbox -Recurse -Force
        }
    }

    It 'keeps incremental build as the default path' {
        New-BuildHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'run-build.ps1') | Out-Null

        $calls = Get-Content -Path (Join-Path $script:Sandbox 'calls.log')

        $calls.Count | Should Be 3
        $calls[0] | Should Be 'import|C:\build-dir'
        $calls[1] | Should Be 'ninja|-C|C:\build-dir|alpha|beta'
        $calls[2] | Should Be 'assert|ninja build'
    }

    It 'runs ninja clean first when CleanFirst is requested' {
        New-BuildHarness -Sandbox $script:Sandbox -CleanFirst
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'run-build.ps1') | Out-Null

        $calls = Get-Content -Path (Join-Path $script:Sandbox 'calls.log')

        $calls.Count | Should Be 6
        $calls[0] | Should Be 'import|C:\build-dir'
        $calls[1] | Should Be 'host|[INFO] Header dependency tracking is unreliable in this Windows + Ninja + localized MSVC toolchain; using correctness-first clean rebuild for workflow validation.'
        $calls[2] | Should Be 'ninja|-C|C:\build-dir|-t|clean'
        $calls[3] | Should Be 'assert|ninja clean'
        $calls[4] | Should Be 'ninja|-C|C:\build-dir|alpha|beta'
        $calls[5] | Should Be 'assert|ninja build'
    }
}

Describe 'workflow.ps1' {
    BeforeEach {
        $script:Sandbox = Join-Path ([System.IO.Path]::GetTempPath()) ([System.Guid]::NewGuid().ToString())
        New-Item -ItemType Directory -Path $script:Sandbox | Out-Null

        Copy-Item -Path (Join-Path $here 'workflow.ps1') -Destination (Join-Path $script:Sandbox 'workflow.ps1')

        @'
function Add-Call {
    param([string]$Line)

    Add-Content -Path (Join-Path $PSScriptRoot 'calls.log') -Value $Line
}

function Get-SurakartaBuildDir {
    param([string]$Configuration)

    Add-Call "get-build-dir|$Configuration"
    return "AUTO-$Configuration"
}

function Invoke-SurakartaConfigure {
    param([string]$Configuration, [string]$BuildDir)

    Add-Call "configure|$Configuration|$BuildDir"
}

function Invoke-SurakartaBuild {
    param([string]$BuildDir, [switch]$CleanFirst, [string[]]$Targets)

    Add-Call "build|$BuildDir|$($CleanFirst.IsPresent)|$([string]::Join(',', $Targets))"
}

function Invoke-SurakartaCTest {
    param([string]$BuildDir)

    Add-Call "ctest|$BuildDir"
}

function Assert-NoSurakartaWorkspaceProcesses {
    Add-Call 'assert-no-processes'
}
'@ | Set-Content -Path (Join-Path $script:Sandbox 'common.ps1')
    }

    AfterEach {
        if (Test-Path $script:Sandbox) {
            Remove-Item -Path $script:Sandbox -Recurse -Force
        }
    }

    It 'passes CleanFirst through the default workflow build' {
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'workflow.ps1') -Configuration Release -BuildDir 'C:\workflow-build' | Out-Null

        $calls = Get-Content -Path (Join-Path $script:Sandbox 'calls.log')

        $calls.Count | Should Be 4
        $calls[0] | Should Be 'configure|Release|C:\workflow-build'
        $calls[1] | Should Be 'build|C:\workflow-build|True|surakarta-benchmark,surakarta-bitboard-selftest,surakarta-dev-session-selftest,surakarta-gui'
        $calls[2] | Should Be 'ctest|C:\workflow-build'
        $calls[3] | Should Be 'assert-no-processes'
    }
}

Describe 'Invoke-SurakartaConfigure' {
    BeforeEach {
        $script:Sandbox = Join-Path ([System.IO.Path]::GetTempPath()) ([System.Guid]::NewGuid().ToString())
        New-Item -ItemType Directory -Path $script:Sandbox | Out-Null
    }

    AfterEach {
        if (Test-Path $script:Sandbox) {
            Remove-Item -Path $script:Sandbox -Recurse -Force
        }
    }

    It 'clears stale build caches before reconfiguring' {
        New-ConfigureHarness -Sandbox $script:Sandbox -CacheSourceDir 'C:/stale-root'
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\run-configure.ps1') | Out-Null

        $buildDir = Join-Path $script:Sandbox 'build-vs-release'
        $repoRoot = $script:Sandbox
        $calls = Get-Content -Path (Join-Path $script:Sandbox 'scripts\calls.log')

        $calls[0] | Should Be "remove|$buildDir|True|True"
        $calls[1] | Should Be "import|$buildDir"
        $calls[2] | Should Be "cmake|-S|$repoRoot|-B|$buildDir|-G|Ninja|-DCMAKE_BUILD_TYPE=Release|-DSURAKARTA_ENABLE_GUI=ON"
        $calls[3] | Should Be 'assert|cmake configure'
    }

    It 'keeps matching caches intact' {
        New-ConfigureHarness -Sandbox $script:Sandbox -CacheSourceDir $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\run-configure.ps1') | Out-Null

        $buildDir = Join-Path $script:Sandbox 'build-vs-release'
        $repoRoot = $script:Sandbox
        $calls = Get-Content -Path (Join-Path $script:Sandbox 'scripts\calls.log')

        $calls.Count | Should Be 3
        $calls[0] | Should Be "import|$buildDir"
        $calls[1] | Should Be "cmake|-S|$repoRoot|-B|$buildDir|-G|Ninja|-DCMAKE_BUILD_TYPE=Release|-DSURAKARTA_ENABLE_GUI=ON"
        $calls[2] | Should Be 'assert|cmake configure'
    }
}

Describe 'benchmark-train.ps1' {
    BeforeEach {
        $script:Sandbox = Join-Path ([System.IO.Path]::GetTempPath()) ([System.Guid]::NewGuid().ToString())
        New-Item -ItemType Directory -Path $script:Sandbox | Out-Null
    }

    AfterEach {
        if (Test-Path $script:Sandbox) {
            Remove-Item -Path $script:Sandbox -Recurse -Force
        }
    }

    It 'runs dual smoke passes, exercises benchmark loading, and writes reproducibility summaries' {
        New-BenchmarkHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\benchmark-harness.ps1') | Out-Null

        $calls = Get-Content -Path (Join-Path $script:Sandbox 'scripts\calls.log')
        $reproJsonPath = Join-Path $script:Sandbox 'build-local\training\20260424-120000\repro-summary.json'
        ($calls | Where-Object { $_ -like 'start|*|bitboard-train *--games 4*' }).Count | Should Be 2
        ($calls | Where-Object { $_ -like 'start|*|bitboard-benchmark --weights *' }).Count | Should Be 2
        Test-Path $reproJsonPath | Should Be $true
        Test-Path (Join-Path $script:Sandbox 'build-local\training\20260424-120000\repro-summary.txt') | Should Be $true

        $summary = Get-Content -Path $reproJsonPath -Raw | ConvertFrom-Json
        $summary.reproducible | Should Be $true
        $summary.runs.Count | Should Be 2
        $summary.runs[0].signature.eval_json | Should Match '^[A-F0-9]{64}$'
        $summary.runs[0].signature.eval_text | Should Match '^[A-F0-9]{64}$'
        $summary.runs[0].signature.eval_json | Should Be $summary.runs[1].signature.eval_json
        $summary.runs[0].signature.eval_text | Should Be $summary.runs[1].signature.eval_text
    }

    It 'fails when reproducibility hashes drift between the two smoke runs' {
        New-BenchmarkHarness -Sandbox $script:Sandbox -ShouldMismatch $true
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\benchmark-harness.ps1') | Out-Null

        $LASTEXITCODE | Should Not Be 0
    }
}

Describe 'manual-acceptance.ps1' {
    BeforeEach {
        $script:Sandbox = Join-Path ([System.IO.Path]::GetTempPath()) ([System.Guid]::NewGuid().ToString())
        New-Item -ItemType Directory -Path $script:Sandbox | Out-Null
    }

    AfterEach {
        if (Test-Path $script:Sandbox) {
            Remove-Item -Path $script:Sandbox -Recurse -Force
        }
    }

    It 'creates the manual acceptance archive scaffold and records the launch command' {
        New-ManualAcceptanceHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\manual-harness.ps1') | Out-Null

        $runRoot = Join-Path $script:Sandbox 'build-local\manual-acceptance\20260424-130000'
        $calls = Get-Content -Path (Join-Path $script:Sandbox 'scripts\calls.log')

        Test-Path (Join-Path $runRoot 'commands.txt') | Should Be $true
        Test-Path (Join-Path $runRoot 'checklist.md') | Should Be $true
        Test-Path (Join-Path $runRoot 'result.txt') | Should Be $true
        Test-Path (Join-Path $runRoot 'git-commit.txt') | Should Be $true
        Test-Path (Join-Path $runRoot 'screenshots') | Should Be $true
        ($calls | Where-Object { $_ -like 'start|*surakarta-gui.exe*' }).Count | Should Be 1
        Get-Content -Path (Join-Path $runRoot 'commands.txt') -Raw | Should Match 'surakarta-gui\.exe'
    }
}
