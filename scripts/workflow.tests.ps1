$here = Split-Path -Parent $MyInvocation.MyCommand.Path

function Initialize-HarnessGitRepo {
    param([string]$Sandbox)

    Push-Location $Sandbox
    git init -b main | Out-Null
    git config user.name 'Pester Harness'
    git config user.email 'pester@example.com'
    'baseline' | Set-Content -Path 'README.md'
    git add README.md
    git commit -m 'init' | Out-Null
    Pop-Location
}

function Write-FakeNTupleWeights {
    param(
        [string]$Path,
        [int]$Value = 0
    )

    New-Item -ItemType Directory -Force -Path ([System.IO.Path]::GetDirectoryName($Path)) | Out-Null
    $stream = [System.IO.File]::Open($Path, [System.IO.FileMode]::Create, [System.IO.FileAccess]::Write)
    $writer = [System.IO.BinaryWriter]::new($stream)
    try {
        $writer.Write([uint32]0x5455504C)
        $writer.Write([uint16]1)
        $writer.Write([uint16]0)
        $writer.Write([uint64]0x1020304050607080)
        $writer.Write([uint32]1)
        $writer.Write([int32]$Value)
    } finally {
        $writer.Dispose()
        $stream.Dispose()
    }
}

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
    $commonPath = Join-Path $here 'common.ps1'
    New-Item -ItemType Directory -Path $scriptsDir, $testDataDir -Force | Out-Null
    Initialize-HarnessGitRepo -Sandbox $Sandbox

    Copy-Item -Path (Join-Path $here 'benchmark-train.ps1') -Destination (Join-Path $scriptsDir 'benchmark-train.ps1')

    "middlegame" | Set-Content -Path (Join-Path $testDataDir 'game1.txt')

    @"
. '$commonPath'

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

function Write-FakeNTupleWeights {
    param(
        [string]`$Path,
        [int]`$Value
    )

    New-Item -ItemType Directory -Force -Path ([System.IO.Path]::GetDirectoryName(`$Path)) | Out-Null
    `$stream = [System.IO.File]::Open(`$Path, [System.IO.FileMode]::Create, [System.IO.FileAccess]::Write)
    `$writer = [System.IO.BinaryWriter]::new(`$stream)
    try {
        `$writer.Write([uint32]0x5455504C)
        `$writer.Write([uint16]1)
        `$writer.Write([uint16]0)
        `$writer.Write([uint64]0x1020304050607080)
        `$writer.Write([uint32]1)
        `$writer.Write([int32]`$Value)
    } finally {
        `$writer.Dispose()
        `$stream.Dispose()
    }
}

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
                Write-FakeNTupleWeights -Path `$outputPath -Value 11
                `$stdout = '{"games_requested":0,"games_completed":0,"checkpoint_count":0}'
            } else {
                `$script:TrainInvocation += 1
                `$candidateValue = if (`$script:ShouldMismatch -and `$script:TrainInvocation -eq 2) { 23 } else { 22 }
                Write-FakeNTupleWeights -Path `$outputPath -Value `$candidateValue

                `$checkpointDirIndex = [Array]::IndexOf(`$ArgumentList, '--checkpoint-dir')
                if (`$checkpointDirIndex -ge 0) {
                    `$checkpointDir = `$ArgumentList[`$checkpointDirIndex + 1]
                    New-Item -ItemType Directory -Force -Path `$checkpointDir | Out-Null
                    `$checkpointValue = if (`$script:ShouldMismatch -and `$script:TrainInvocation -eq 2) { 33 } else { 32 }
                    Write-FakeNTupleWeights -Path (Join-Path `$checkpointDir 'checkpoint-2.bin') -Value `$checkpointValue
                }

                `$stdout = "{""games_requested"":`$games,""games_completed"":`$games,""checkpoint_count"":1}"
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
    `$process | Add-Member -MemberType ScriptMethod -Name WaitForExit -Value { }
    `$process | Add-Member -MemberType ScriptMethod -Name Refresh -Value { }
    return `$process
}

function Wait-Process {
    param([int]`$Id)
    throw "Harness should use process.WaitForExit() instead of Wait-Process -Id"
}

. (Join-Path `$PSScriptRoot 'common.ps1')
. (Join-Path `$PSScriptRoot 'benchmark-train.ps1')
"@ | Set-Content -Path (Join-Path $scriptsDir 'benchmark-harness.ps1')
}

function New-SelectionHarness {
    param(
        [string]$Sandbox,
        [switch]$RepeatabilityArtifactProfiles
    )

    $scriptsDir = Join-Path $Sandbox 'scripts'
    $testDataDir = Join-Path $Sandbox 'test\test_data'
    $commonPath = Join-Path $here 'common.ps1'
    New-Item -ItemType Directory -Path $scriptsDir, $testDataDir -Force | Out-Null
    Initialize-HarnessGitRepo -Sandbox $Sandbox

    Copy-Item -Path (Join-Path $here 'benchmark-select.ps1') -Destination (Join-Path $scriptsDir 'benchmark-select.ps1')

    'opening' | Set-Content -Path (Join-Path $testDataDir 'game1.txt')
    'endgame' | Set-Content -Path (Join-Path $testDataDir 'game6.txt')

    @"
. '$commonPath'

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

    $repeatabilityArtifactProfilesLiteral = if ($RepeatabilityArtifactProfiles.IsPresent) { '$true' } else { '$false' }
    @"
`$script:NextPid = 0
`$script:TrainInvocation = 0
`$script:RepeatabilityArtifactProfiles = $repeatabilityArtifactProfilesLiteral

function Write-FakeNTupleWeights {
    param(
        [string]`$Path,
        [int]`$Value
    )

    New-Item -ItemType Directory -Force -Path ([System.IO.Path]::GetDirectoryName(`$Path)) | Out-Null
    `$stream = [System.IO.File]::Open(`$Path, [System.IO.FileMode]::Create, [System.IO.FileAccess]::Write)
    `$writer = [System.IO.BinaryWriter]::new(`$stream)
    try {
        `$writer.Write([uint32]0x5455504C)
        `$writer.Write([uint16]1)
        `$writer.Write([uint16]0)
        `$writer.Write([uint64]0x1020304050607080)
        `$writer.Write([uint32]1)
        `$writer.Write([int32]`$Value)
    } finally {
        `$writer.Dispose()
        `$stream.Dispose()
    }
}

function Get-Date {
    param([string]`$Format)

    if (`$Format) {
        return '20260424-140000'
    }

    return Microsoft.PowerShell.Utility\Get-Date
}

function Wait-Process {
    param([int]`$Id)
    throw "Harness should use process.WaitForExit() instead of Wait-Process -Id"
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
            `$seedIndex = [Array]::IndexOf(`$ArgumentList, '--seed')
            `$gamesIndex = [Array]::IndexOf(`$ArgumentList, '--games')
            `$games = [int]`$ArgumentList[`$gamesIndex + 1]
            `$seed = if (`$seedIndex -ge 0) { [int]`$ArgumentList[`$seedIndex + 1] } else { 0 }

            New-Item -ItemType Directory -Force -Path ([System.IO.Path]::GetDirectoryName(`$outputPath)) | Out-Null

            if (`$games -eq 0) {
                Write-FakeNTupleWeights -Path `$outputPath -Value 11
                `$stdout = '{"games_requested":0,"games_completed":0,"checkpoint_count":0}'
            } else {
                `$script:TrainInvocation += 1
                `$content = switch (`$seed) {
                    20260423 { 21 }
                    20260424 { 22 }
                    20260425 { 23 }
                    default { 24 }
                }
                Write-FakeNTupleWeights -Path `$outputPath -Value `$content

                `$checkpointDirIndex = [Array]::IndexOf(`$ArgumentList, '--checkpoint-dir')
                if (`$checkpointDirIndex -ge 0) {
                    `$checkpointDir = `$ArgumentList[`$checkpointDirIndex + 1]
                    New-Item -ItemType Directory -Force -Path `$checkpointDir | Out-Null
                    Write-FakeNTupleWeights -Path (Join-Path `$checkpointDir ("checkpoint-{0}.bin" -f `$seed)) -Value (100 + `$seed % 1000)
                }

                `$stdout = "{""games_requested"":`$games,""games_completed"":`$games,""checkpoint_count"":1}"
            }
        }
        'bitboard-eval' {
            `$candidateIndex = [Array]::IndexOf(`$ArgumentList, '--candidate')
            `$baselineIndex = [Array]::IndexOf(`$ArgumentList, '--baseline')
            `$candidatePath = `$ArgumentList[`$candidateIndex + 1]
            `$baselinePath = `$ArgumentList[`$baselineIndex + 1]
            `$candidateJsonPath = `$candidatePath.Replace('\', '\\')
            `$baselineJsonPath = `$baselinePath.Replace('\', '\\')

            `$profile = if (`$script:RepeatabilityArtifactProfiles -and `$candidatePath -like '*seed-20260423*') {
                'repeatability-timing-sensitive'
            } elseif (`$script:RepeatabilityArtifactProfiles -and `$candidatePath -like '*seed-20260425*') {
                'repeatability-stable-tree-inflation'
            } elseif (`$candidatePath -like '*seed-20260423*') {
                'pass'
            } elseif (`$candidatePath -like '*seed-20260424*') {
                'watch'
            } elseif (`$candidatePath -like '*seed-20260426*') {
                'micro-timing'
            } elseif (`$candidatePath -like '*seed-20260427*') {
                'strength-pass-perf-regress'
            } elseif (`$candidatePath -like '*seed-20260428*') {
                'strength-watch-perf-pass'
            } elseif (`$candidatePath -like '*seed-20260429*') {
                'score-regression-perf-pass'
            } elseif (`$candidatePath -like '*seed-20260430*') {
                'search-seconds-perf-regress'
            } elseif (`$candidatePath -like '*seed-20260431*') {
                'node-perf-regress'
            } elseif (`$candidatePath -like '*seed-20260432*') {
                'qnodes-perf-regress'
            } else {
                'regress'
            }

            switch (`$profile) {
                'pass' {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.010; nodes = 96; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 35 }; baseline = @{ search_seconds = 0.012; nodes = 128; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.014; nodes = 112; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 27 }; baseline = @{ search_seconds = 0.017; nodes = 144; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.008; nodes = 80; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 11 }; baseline = @{ search_seconds = 0.009; nodes = 96; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 3; baseline_wins = 2; draws = 1 }
                }
                'watch' {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.013; nodes = 110; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 }; baseline = @{ search_seconds = 0.012; nodes = 100; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.018; nodes = 132; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 }; baseline = @{ search_seconds = 0.016; nodes = 120; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.011; nodes = 88; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 }; baseline = @{ search_seconds = 0.010; nodes = 80; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 2; baseline_wins = 2; draws = 2 }
                }
                'micro-timing' {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.010; nodes = 120; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 }; baseline = @{ search_seconds = 0.010; nodes = 120; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.011; nodes = 140; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 }; baseline = @{ search_seconds = 0.011; nodes = 140; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.000026; nodes = 36; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 }; baseline = @{ search_seconds = 0.000009; nodes = 36; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 3; baseline_wins = 3; draws = 0 }
                }
                'strength-pass-perf-regress' {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.014; nodes = 1400; qnodes = 1600; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 35 }; baseline = @{ search_seconds = 0.010; nodes = 1000; qnodes = 1000; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.008; nodes = 800; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 27 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.007; nodes = 700; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 11 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 4; baseline_wins = 2; draws = 0 }
                }
                'search-seconds-perf-regress' {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.014; nodes = 1000; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 35 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.008; nodes = 800; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 27 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.007; nodes = 700; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 11 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 4; baseline_wins = 2; draws = 0 }
                }
                'node-perf-regress' {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.010; nodes = 1400; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 35 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.008; nodes = 800; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 27 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.007; nodes = 700; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 11 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 4; baseline_wins = 2; draws = 0 }
                }
                'qnodes-perf-regress' {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.010; nodes = 1000; qnodes = 1400; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 35 }; baseline = @{ search_seconds = 0.010; nodes = 1000; qnodes = 1000; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.008; nodes = 800; qnodes = 800; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 27 }; baseline = @{ search_seconds = 0.010; nodes = 1000; qnodes = 1000; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.007; nodes = 700; qnodes = 700; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 11 }; baseline = @{ search_seconds = 0.010; nodes = 1000; qnodes = 1000; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 4; baseline_wins = 2; draws = 0 }
                }
                'repeatability-timing-sensitive' {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.001469059; nodes = 1653; best_move = '(0, 1) -> (1, 2) (B)'; pv = '(0, 1) -> (1, 2) (B)'; score = 35 }; baseline = @{ search_seconds = 0.001000000; nodes = 1472; best_move = '(0, 1) -> (1, 2) (B)'; pv = '(0, 1) -> (1, 2) (B)'; score = 30 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.008; nodes = 800; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 27 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.007; nodes = 700; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 11 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 4; baseline_wins = 2; draws = 0 }
                }
                'repeatability-stable-tree-inflation' {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.002159236; nodes = 1907; best_move = '(1, 1) -> (2, 2) (B)'; pv = '(1, 1) -> (2, 2) (B)'; score = 35 }; baseline = @{ search_seconds = 0.001000000; nodes = 1472; best_move = '(1, 1) -> (2, 2) (B)'; pv = '(1, 1) -> (2, 2) (B)'; score = 30 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.008; nodes = 800; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 27 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.007; nodes = 700; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 11 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 4; baseline_wins = 2; draws = 0 }
                }
                'strength-watch-perf-pass' {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.008; nodes = 800; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.009; nodes = 900; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.007; nodes = 700; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 3; baseline_wins = 3; draws = 0 }
                }
                'score-regression-perf-pass' {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.008; nodes = 800; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 25 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 30 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.009; nodes = 900; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 27 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.007; nodes = 700; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 11 }; baseline = @{ search_seconds = 0.010; nodes = 1000; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 4; baseline_wins = 2; draws = 0 }
                }
                default {
                    `$positionReports = @(
                        @{ case_id = 'opening'; candidate = @{ search_seconds = 0.015; nodes = 130; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 25 }; baseline = @{ search_seconds = 0.012; nodes = 100; best_move = '(0, 0) -> (1, 0)'; pv = '(0, 0) -> (1, 0)'; score = 28 } },
                        @{ case_id = 'middlegame'; candidate = @{ search_seconds = 0.021; nodes = 156; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 20 }; baseline = @{ search_seconds = 0.016; nodes = 120; best_move = '(4, 4) -> (5, 1)'; pv = '(4, 4) -> (5, 1)'; score = 24 } },
                        @{ case_id = 'endgame'; candidate = @{ search_seconds = 0.014; nodes = 104; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 7 }; baseline = @{ search_seconds = 0.010; nodes = 80; best_move = '(6, 6) -> (6, 5)'; pv = '(6, 6) -> (6, 5)'; score = 9 } }
                    )
                    `$matchSummary = @{ candidate_wins = 1; baseline_wins = 3; draws = 2 }
                }
            }

            if (`$ArgumentList -contains 'json') {
                `$positionJson = (`$positionReports | ConvertTo-Json -Depth 8 -Compress)
                `$matchGamesJson = @(
                    @{ case_id = 'opening'; candidate_color = 'black'; plies = 14; outcome = if (`$profile -eq 'pass') { 'candidate_win' } elseif (`$profile -eq 'watch') { 'draw' } else { 'baseline_win' }; final_reason = 'terminal' },
                    @{ case_id = 'opening'; candidate_color = 'white'; plies = 16; outcome = if (`$profile -eq 'pass') { 'candidate_win' } elseif (`$profile -eq 'watch') { 'baseline_win' } else { 'baseline_win' }; final_reason = 'terminal' },
                    @{ case_id = 'middlegame'; candidate_color = 'black'; plies = 18; outcome = if (`$profile -eq 'pass') { 'candidate_win' } elseif (`$profile -eq 'watch') { 'draw' } else { 'baseline_win' }; final_reason = 'terminal' },
                    @{ case_id = 'middlegame'; candidate_color = 'white'; plies = 20; outcome = if (`$profile -eq 'pass') { 'baseline_win' } elseif (`$profile -eq 'watch') { 'draw' } else { 'baseline_win' }; final_reason = 'terminal' },
                    @{ case_id = 'endgame'; candidate_color = 'black'; plies = 12; outcome = if (`$profile -eq 'pass') { 'candidate_win' } elseif (`$profile -eq 'watch') { 'draw' } else { 'baseline_win' }; final_reason = 'terminal' },
                    @{ case_id = 'endgame'; candidate_color = 'white'; plies = 10; outcome = if (`$profile -eq 'pass') { 'candidate_win' } elseif (`$profile -eq 'watch') { 'draw' } else { 'baseline_win' }; final_reason = 'terminal' }
                ) | ConvertTo-Json -Depth 8 -Compress
                `$stdout = "{""candidate_weights"":""`$candidateJsonPath"",""baseline_weights"":""`$baselineJsonPath"",""depth"":4,""position_reports"":`$positionJson,""match_summary"":{""candidate_wins"":`$(`$matchSummary.candidate_wins),""baseline_wins"":`$(`$matchSummary.baseline_wins),""draws"":`$(`$matchSummary.draws)},""match_games"":`$matchGamesJson}"
            } else {
                `$stdout = "candidate: `$candidatePath`nbaseline: `$baselinePath`ncandidate_wins: `$(`$matchSummary.candidate_wins)`nbaseline_wins: `$(`$matchSummary.baseline_wins)`ndraws: `$(`$matchSummary.draws)"
            }
        }
        'bitboard-search' {
            `$stdout = '{"case_id":"middlegame","best_move":"(4, 4) -> (5, 1) (B)"}'
        }
        'bitboard-benchmark' {
            `$stdout = '{"case_id":"middlegame","nodes":256}'
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
    `$process | Add-Member -MemberType ScriptMethod -Name WaitForExit -Value { }
    `$process | Add-Member -MemberType ScriptMethod -Name Refresh -Value { }
    return `$process
}

. (Join-Path `$PSScriptRoot 'common.ps1')
. (Join-Path `$PSScriptRoot 'benchmark-select.ps1') @args
"@ | Set-Content -Path (Join-Path $scriptsDir 'selection-harness.ps1')
}

function New-PerfHarness {
    param(
        [string]$Sandbox,
        [switch]$Regress
    )

    $scriptsDir = Join-Path $Sandbox 'scripts'
    $testDataDir = Join-Path $Sandbox 'test\test_data'
    $commonPath = Join-Path $here 'common.ps1'
    New-Item -ItemType Directory -Path $scriptsDir, $testDataDir -Force | Out-Null
    Initialize-HarnessGitRepo -Sandbox $Sandbox

    Copy-Item -Path (Join-Path $here 'perf-runner.ps1') -Destination (Join-Path $scriptsDir 'perf-runner.ps1')

    'middlegame' | Set-Content -Path (Join-Path $testDataDir 'game1.txt')
    'endgame' | Set-Content -Path (Join-Path $testDataDir 'game6.txt')

    $baseline = @()
    foreach ($caseId in @('opening', 'middlegame', 'endgame')) {
        foreach ($depth in @(6, 7, 8)) {
            foreach ($threads in @(1, 2, 4)) {
                $nodes = if ($caseId -eq 'endgame' -and $depth -eq 6) { 36 } else { 1000 + ($depth * 100) + $threads }
                $baseline += [ordered]@{
                    case_id = $caseId
                    depth = $depth
                    threads = $threads
                    search_seconds = if ($nodes -lt 500) { 0.000010 } else { 0.010000 + ($depth / 1000.0) + ($threads / 10000.0) }
                    nodes = $nodes
                    best_move = "$caseId-d$depth-best"
                    score = $depth * 10
                }
            }
        }
    }
    [ordered]@{ results = $baseline } | ConvertTo-Json -Depth 6 | Set-Content -Path (Join-Path $testDataDir 'perf-baseline.json')

    @"
. '$commonPath'

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
    param([string]`$WorkspaceRoot, [int[]]`$ExcludeProcessId = @())

    Add-Call "stop|`$WorkspaceRoot"
    return @()
}

function Assert-NoSurakartaWorkspaceProcesses {
    param([string]`$WorkspaceRoot)

    Add-Call "assert-no-processes|`$WorkspaceRoot"
}
"@ | Set-Content -Path (Join-Path $scriptsDir 'common.ps1')

    $regressLiteral = if ($Regress.IsPresent) { '$true' } else { '$false' }
    @"
`$script:NextPid = 0
`$script:Regress = $regressLiteral

function Get-Date {
    param([string]`$Format)

    if (`$Format) {
        return '20260424-150000'
    }

    return Microsoft.PowerShell.Utility\Get-Date
}

function Wait-Process {
    param(
        [int]`$Id,
        [int]`$Timeout,
        [string]`$ErrorAction
    )
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

    `$caseId = `$ArgumentList[[Array]::IndexOf(`$ArgumentList, '--case-id') + 1]
    `$depth = [int]`$ArgumentList[[Array]::IndexOf(`$ArgumentList, '--depth') + 1]
    `$threads = [int]`$ArgumentList[[Array]::IndexOf(`$ArgumentList, '--threads') + 1]
    `$nodes = if (`$caseId -eq 'endgame' -and `$depth -eq 6) { 36 } else { 1000 + (`$depth * 100) + `$threads }
    `$seconds = if (`$nodes -lt 500) { if (`$script:Regress) { 0.000030 } else { 0.000030 } } else { 0.010000 + (`$depth / 1000.0) + (`$threads / 10000.0) }
    if (`$script:Regress -and `$caseId -eq 'opening' -and `$depth -eq 6 -and `$threads -eq 1) {
        `$nodes = 2200
    }
    `$bestMove = if (`$script:Regress -and `$caseId -eq 'middlegame' -and `$depth -eq 7 -and `$threads -eq 1) {
        'drifted-best'
    } else {
        "`$caseId-d`$depth-best"
    }
    `$score = `$depth * 10
    `$nps = if (`$seconds -gt 0) { [uint64](`$nodes / `$seconds) } else { 0 }
    `$stdout = [ordered]@{
        case_id = `$caseId
        depth = `$depth
        threads = `$threads
        search_seconds = `$seconds
        nodes = `$nodes
        nps = `$nps
        tt_hits = 0
        null_prunes = 0
        lmr_reductions = 0
        fail_highs = 0
        fail_lows = 0
        aspiration_retries = 0
        pv = "`$bestMove"
        best_move = "`$bestMove"
        score = `$score
    } | ConvertTo-Json -Compress
    `$stdout | Set-Content -Path `$RedirectStandardOutput

    `$process = [pscustomobject]@{
        Id = (`$script:NextPid += 1)
        ExitCode = 0
    }
    `$process | Add-Member -MemberType ScriptMethod -Name Refresh -Value { }
    return `$process
}

. (Join-Path `$PSScriptRoot 'common.ps1')
. (Join-Path `$PSScriptRoot 'perf-runner.ps1') @args
"@ | Set-Content -Path (Join-Path $scriptsDir 'perf-harness.ps1')
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
        ($calls | Where-Object { $_ -like 'start|*|bitboard-train *--games 4*--terminal-reward 1200*--td-error-clip 0*' }).Count | Should Be 2
        ($calls | Where-Object { $_ -like 'start|*|bitboard-benchmark --weights *' }).Count | Should Be 2
        Test-Path $reproJsonPath | Should Be $true
        Test-Path (Join-Path $script:Sandbox 'build-local\training\20260424-120000\repro-summary.txt') | Should Be $true

        $summary = Get-Content -Path $reproJsonPath -Raw | ConvertFrom-Json
        $summary.reproducible | Should Be $true
        $summary.runs.Count | Should Be 2
        $summary.runs[0].signature.eval_json | Should Match '^[A-F0-9]{64}$'
        $summary.runs[0].signature.eval_text | Should Match '^[A-F0-9]{64}$'
        $summary.runs[0].signature.bootstrap | Should Be $summary.runs[1].signature.bootstrap
        $summary.runs[0].signature.candidate | Should Be $summary.runs[1].signature.candidate
    }

    It 'writes traceable sidecar manifests for weights and checkpoints' {
        New-BenchmarkHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\benchmark-harness.ps1') | Out-Null

        $sessionRoot = Join-Path $script:Sandbox 'build-local\training\20260424-120000'
        $candidateManifestPath = Join-Path $sessionRoot 'run-1\weights\candidate.bin.manifest.json'
        $checkpointManifestPath = Join-Path $sessionRoot 'run-1\checkpoints\checkpoint-2.bin.manifest.json'
        $sessionManifestPath = Join-Path $sessionRoot 'weights-manifest.json'

        Test-Path $candidateManifestPath | Should Be $true
        Test-Path $checkpointManifestPath | Should Be $true
        Test-Path $sessionManifestPath | Should Be $true

        $manifest = Get-Content -Path $candidateManifestPath -Raw | ConvertFrom-Json
        $manifest.git_commit | Should Match '^[0-9a-f]{40}$'
        $manifest.seed | Should Be 20260423
        $manifest.games | Should Be 4
        $manifest.depth | Should Be 4
        $manifest.alpha | Should Be 0.005
        $manifest.lambda | Should Be 0.5
        $manifest.epsilon | Should Be 0.02
        $manifest.epsilon_plies | Should Be 6
        $manifest.checkpoint_every | Should Be 2
        $manifest.terminal_reward | Should Be 1200
        $manifest.td_error_clip | Should Be 0
        $manifest.terminal_only_warmup | Should Be 0
        $manifest.near_terminal_curriculum | Should Be 0
        $manifest.checkpoint.is_checkpoint | Should Be $false
        $manifest.tuple_set_hash | Should Be '0x1020304050607080'
        $manifest.weight_count | Should Be 1
        $manifest.binary_sha256 | Should Match '^[A-F0-9]{64}$'

        $checkpointManifest = Get-Content -Path $checkpointManifestPath -Raw | ConvertFrom-Json
        $checkpointManifest.checkpoint_every | Should Be 2
        $checkpointManifest.terminal_reward | Should Be 1200
        $checkpointManifest.td_error_clip | Should Be 0
        $checkpointManifest.checkpoint.is_checkpoint | Should Be $true
        $checkpointManifest.checkpoint.name | Should Be 'checkpoint-2.bin'

        $sessionManifest = Get-Content -Path $sessionManifestPath -Raw | ConvertFrom-Json
        $sessionManifest.artifacts.Count | Should Be 6
        $sessionManifest.artifacts[0].git_commit | Should Match '^[0-9a-f]{40}$'
    }

    It 'fails when reproducibility hashes drift between the two smoke runs' {
        New-BenchmarkHarness -Sandbox $script:Sandbox -ShouldMismatch $true
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\benchmark-harness.ps1') | Out-Null

        $LASTEXITCODE | Should Not Be 0
    }
}

Describe 'weights manifest helpers' {
    It 'rejects manifests without required traceability fields' {
        . (Join-Path $here 'common.ps1')

        $missingGitCommit = [pscustomobject]@{
            checkpoint = [pscustomobject]@{ is_checkpoint = $false }
            binary_sha256 = 'ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789'
        }
        $threw = $false
        try {
            Assert-SurakartaWeightManifestTraceable -Manifest $missingGitCommit -Label 'candidate manifest'
        } catch {
            $threw = $true
            $_.Exception.Message | Should Match 'git_commit'
        }
        $threw | Should Be $true

        $missingCheckpoint = [pscustomobject]@{
            git_commit = '0123456789abcdef0123456789abcdef01234567'
            binary_sha256 = 'ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789'
        }
        $threw = $false
        try {
            Assert-SurakartaWeightManifestTraceable -Manifest $missingCheckpoint -Label 'candidate manifest'
        } catch {
            $threw = $true
            $_.Exception.Message | Should Match 'checkpoint'
        }
        $threw | Should Be $true

        $missingHash = [pscustomobject]@{
            git_commit = '0123456789abcdef0123456789abcdef01234567'
            checkpoint = [pscustomobject]@{ is_checkpoint = $false }
        }
        $threw = $false
        try {
            Assert-SurakartaWeightManifestTraceable -Manifest $missingHash -Label 'candidate manifest'
        } catch {
            $threw = $true
            $_.Exception.Message | Should Match 'binary_sha256'
        }
        $threw | Should Be $true
    }
}

Describe 'benchmark-select.ps1' {
    BeforeEach {
        $script:Sandbox = Join-Path ([System.IO.Path]::GetTempPath()) ([System.Guid]::NewGuid().ToString())
        New-Item -ItemType Directory -Path $script:Sandbox | Out-Null
    }

    AfterEach {
        if (Test-Path $script:Sandbox) {
            Remove-Item -Path $script:Sandbox -Recurse -Force
        }
    }

    It 'runs multi-seed selection, writes per-seed artifacts, and emits a scorecard' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') | Out-Null

        $calls = Get-Content -Path (Join-Path $script:Sandbox 'scripts\calls.log')
        $sessionRoot = Join-Path $script:Sandbox 'build-local\training\20260424-140000'
        $summaryJsonPath = Join-Path $sessionRoot 'selection-summary.json'
        $summaryTextPath = Join-Path $sessionRoot 'selection-summary.txt'

        ($calls | Where-Object { $_ -like 'start|*|bitboard-train *--games 0*' }).Count | Should Be 3
        ($calls | Where-Object { $_ -like 'start|*|bitboard-train *--games 32*' }).Count | Should Be 3
        ($calls | Where-Object { $_ -like 'start|*|bitboard-eval *--format json*' }).Count | Should Be 6
        ($calls | Where-Object { $_ -like 'start|*|bitboard-eval *--format text*' }).Count | Should Be 6
        Test-Path $summaryJsonPath | Should Be $true
        Test-Path $summaryTextPath | Should Be $true
        Test-Path (Join-Path $sessionRoot 'selection\seed-20260423\candidate.bin') | Should Be $true
        Test-Path (Join-Path $sessionRoot 'selection\seed-20260424\candidate.bin') | Should Be $true
        Test-Path (Join-Path $sessionRoot 'selection\seed-20260425\candidate.bin') | Should Be $true
        Test-Path (Join-Path $sessionRoot 'selection\seed-20260423\checkpoint-evals\checkpoint-20260423.eval-summary.json') | Should Be $true

        $summary = Get-Content -Path $summaryJsonPath -Raw | ConvertFrom-Json
        $summary.overall_rating | Should Be 'regress'
        $summary.scorecard.pass | Should Be 1
        $summary.scorecard.watch | Should Be 1
        $summary.scorecard.regress | Should Be 1
        $summary.per_seed.Count | Should Be 3
        $summary.per_seed[0].rating | Should Be 'pass'
        $summary.per_seed[1].rating | Should Be 'watch'
        $summary.per_seed[2].rating | Should Be 'regress'
        $summary.per_seed[0].checkpoints.Count | Should Be 1
        $summary.per_seed[0].checkpoints[0].rating | Should Be 'pass'
        $summary.per_seed[0].checkpoints[0].training_summary.games_completed | Should Be 20260423
        $summary.per_seed[0].checkpoints[0].eval_json_path | Should Match 'checkpoint-evals'
        $summary.per_seed[0].evaluation.position_reports[0].candidate.best_move | Should Match '\(0, 0\) -> \(1, 0\)'
        $summary.per_seed[1].evaluation.position_reports[1].baseline.score | Should Be 24
        $summary.per_seed[2].evaluation.match_summary.baseline_wins | Should Be 3
        $summary.strength_rating | Should Be 'regress'
        $summary.perf_rating | Should Be 'regress'
        $summary.blocker_reason | Should Match 'seed 20260424'
        $summary.blocker_reason | Should Match 'seed 20260425'
        Get-Content -Path $summaryTextPath -Raw | Should Match 'overall_rating: regress'
        Get-Content -Path $summaryTextPath -Raw | Should Match 'strength_rating: regress'
        Get-Content -Path $summaryTextPath -Raw | Should Match 'perf_rating: regress'
        Get-Content -Path $summaryTextPath -Raw | Should Match 'blocker_reason: seed 20260424'
    }

    It 'writes per-seed traceable manifests and a session manifest' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') `
            -Alpha 0.005 -Lambda 0.5 -Epsilon 0.05 -EpsilonPlies 4 `
            -TerminalReward 500 -TdErrorClip 250 -TerminalOnlyWarmup 1 -NearTerminalCurriculum 1 | Out-Null

        $sessionRoot = Join-Path $script:Sandbox 'build-local\training\20260424-140000'
        $candidateManifestPath = Join-Path $sessionRoot 'selection\seed-20260423\candidate.bin.manifest.json'
        $checkpointManifestPath = Join-Path $sessionRoot 'selection\seed-20260423\checkpoints\checkpoint-20260423.bin.manifest.json'
        $sessionManifestPath = Join-Path $sessionRoot 'weights-manifest.json'

        Test-Path $candidateManifestPath | Should Be $true
        Test-Path $checkpointManifestPath | Should Be $true
        Test-Path $sessionManifestPath | Should Be $true

        $manifest = Get-Content -Path $candidateManifestPath -Raw | ConvertFrom-Json
        $manifest.git_commit | Should Match '^[0-9a-f]{40}$'
        $manifest.seed | Should Be 20260423
        $manifest.games | Should Be 32
        $manifest.depth | Should Be 4
        $manifest.alpha | Should Be 0.005
        $manifest.lambda | Should Be 0.5
        $manifest.epsilon | Should Be 0.05
        $manifest.epsilon_plies | Should Be 4
        $manifest.checkpoint_every | Should Be 2
        $manifest.terminal_reward | Should Be 500
        $manifest.td_error_clip | Should Be 250
        $manifest.terminal_only_warmup | Should Be 1
        $manifest.near_terminal_curriculum | Should Be 1
        $manifest.checkpoint.is_checkpoint | Should Be $false
        $manifest.binary_sha256 | Should Match '^[A-F0-9]{64}$'

        $checkpointManifest = Get-Content -Path $checkpointManifestPath -Raw | ConvertFrom-Json
        $checkpointManifest.checkpoint_every | Should Be 2
        $checkpointManifest.terminal_reward | Should Be 500
        $checkpointManifest.td_error_clip | Should Be 250
        $checkpointManifest.checkpoint.is_checkpoint | Should Be $true

        $sessionManifest = Get-Content -Path $sessionManifestPath -Raw | ConvertFrom-Json
        $sessionManifest.purpose | Should Be 'selection'
        $sessionManifest.artifacts.Count | Should Be 9
    }

    It 'passes training hyperparameters through to each candidate run' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') `
            -Alpha 0.005 -Lambda 0.5 -Epsilon 0.05 -EpsilonPlies 4 `
            -TerminalReward 500 -TdErrorClip 250 -TerminalOnlyWarmup 1 -NearTerminalCurriculum 1 | Out-Null

        $calls = Get-Content -Path (Join-Path $script:Sandbox 'scripts\calls.log')
        $candidateTrainCalls = @($calls | Where-Object { $_ -like 'start|*|bitboard-train *--games 32*' })

        $candidateTrainCalls.Count | Should Be 3
        foreach ($call in $candidateTrainCalls) {
            $call | Should Match '--alpha 0.005'
            $call | Should Match '--lambda 0.5'
            $call | Should Match '--epsilon 0.05'
            $call | Should Match '--epsilon-plies 4'
            $call | Should Match '--terminal-reward 500'
            $call | Should Match '--td-error-clip 250'
            $call | Should Match '--terminal-only-warmup 1'
            $call | Should Match '--near-terminal-curriculum 1'
        }
    }

    It 'passes an explicit GamesPerSeed 64 through to candidate runs and manifests' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') `
            -GamesPerSeed 64 -Seeds 20260423 | Out-Null

        $calls = Get-Content -Path (Join-Path $script:Sandbox 'scripts\calls.log')
        ($calls | Where-Object { $_ -like 'start|*|bitboard-train *--games 64*' }).Count | Should Be 1

        $sessionRoot = Join-Path $script:Sandbox 'build-local\training\20260424-140000'
        $manifest = Get-Content -Path (Join-Path $sessionRoot 'selection\seed-20260423\candidate.bin.manifest.json') -Raw | ConvertFrom-Json
        $manifest.games | Should Be 64
        $summary = Get-Content -Path (Join-Path $sessionRoot 'selection-summary.json') -Raw | ConvertFrom-Json
        $summary.games_per_seed | Should Be 64
    }

    It 'does not regress a seed only because tiny fixed positions have noisy timing ratios' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') `
            -Seeds 20260426 | Out-Null

        $sessionRoot = Join-Path $script:Sandbox 'build-local\training\20260424-140000'
        $summaryJsonPath = Join-Path $sessionRoot 'selection-summary.json'
        $summary = Get-Content -Path $summaryJsonPath -Raw | ConvertFrom-Json

        $summary.overall_rating | Should Be 'watch'
        $summary.scorecard.regress | Should Be 0
        $summary.per_seed[0].scorecard.position_metrics[2].case_id | Should Be 'endgame'
        $summary.per_seed[0].scorecard.position_metrics[2].node_ratio | Should Be 1.0
        $summary.per_seed[0].scorecard.position_metrics[2].performance_ratio | Should Be 1.0
    }

    It 'keeps overall regress when strength passes but performance regresses' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') `
            -Seeds 20260427 | Out-Null

        $summary = Get-Content -Path (Join-Path $script:Sandbox 'build-local\training\20260424-140000\selection-summary.json') -Raw | ConvertFrom-Json

        $summary.strength_rating | Should Be 'pass'
        $summary.perf_rating | Should Be 'regress'
        $summary.overall_rating | Should Be 'regress'
        $summary.per_seed[0].rating | Should Be 'regress'

        $details = @($summary.blocker_details)
        $details.Count | Should Be 3
        ($details | Where-Object { $_.seed -eq 20260427 -and $_.category -eq 'opening' -and $_.metric -eq 'search_seconds_ratio' }).Count | Should Be 1
        ($details | Where-Object { $_.seed -eq 20260427 -and $_.category -eq 'opening' -and $_.metric -eq 'node_ratio' }).Count | Should Be 1
        ($details | Where-Object { $_.seed -eq 20260427 -and $_.category -eq 'opening' -and $_.metric -eq 'qnodes_ratio' }).Count | Should Be 1

        $nodeDetail = $details | Where-Object { $_.metric -eq 'node_ratio' } | Select-Object -First 1
        $nodeDetail.baseline_value | Should Be 1000
        $nodeDetail.candidate_value | Should Be 1400
        $nodeDetail.ratio | Should Be 1.4
        $nodeDetail.threshold | Should Be 1.25
        $nodeDetail.trigger | Should Be 'exceeds_threshold'
        $nodeDetail.repeatability | Should Be 'unknown'
        $nodeDetail.repeatability_status | Should Be 'unknown'
        ($null -eq $nodeDetail.repeatability_runs) | Should Be $true
        ($null -eq $nodeDetail.repeatability_exceeded_count) | Should Be $true
        $summary.per_seed[0].blocker_reason | Should Match 'performance ratio'
        @($summary.per_seed[0].blocker_details).Count | Should Be 3
        @($summary.per_seed[0].scorecard.blocker_details).Count | Should Be 3

        $summaryText = Get-Content -Path (Join-Path $script:Sandbox 'build-local\training\20260424-140000\selection-summary.txt') -Raw
        $summaryText | Should Match 'Perf blocker details'
        $summaryText | Should Match 'seed \| category \| metric \| ratio \| threshold \| repeatability \| verdict'
        $summaryText | Should Match '20260427 \| opening \| node_ratio \| 1\.400000 \| 1\.250000 \| unknown \| regress'
    }

    It 'integrates matching repeatability evidence as report-only blocker diagnostics' {
        New-SelectionHarness -Sandbox $script:Sandbox -RepeatabilityArtifactProfiles
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') | Out-Null

        $summary = Get-Content -Path (Join-Path $script:Sandbox 'build-local\training\20260424-140000\selection-summary.json') -Raw | ConvertFrom-Json
        $details = @($summary.blocker_details)

        $summary.strength_rating | Should Be 'watch'
        $summary.perf_rating | Should Be 'regress'
        $summary.overall_rating | Should Be 'regress'
        $summary.scorecard.regress | Should Be 2
        $summary.scorecard.watch | Should Be 1
        ($summary.per_seed | Where-Object { $_.seed -eq 20260423 } | Select-Object -First 1).rating | Should Be 'regress'
        ($summary.per_seed | Where-Object { $_.seed -eq 20260425 } | Select-Object -First 1).rating | Should Be 'regress'
        $details.Count | Should Be 3
        ($details | Where-Object { $_.metric -eq 'qnodes_ratio' }).Count | Should Be 0

        $timingSensitive = $details | Where-Object {
            $_.seed -eq 20260423 -and $_.category -eq 'opening' -and $_.metric -eq 'search_seconds_ratio'
        } | Select-Object -First 1
        $timingSensitive.repeatability | Should Be 'timing_sensitive'
        $timingSensitive.repeatability_status | Should Be 'timing_sensitive'
        $timingSensitive.repeatability_runs | Should Be 10
        $timingSensitive.repeatability_exceeded_count | Should Be 2
        $timingSensitive.repeatability_notes | Should Match 'time ratio exceeded 2/10'

        $stableTimed = $details | Where-Object {
            $_.seed -eq 20260425 -and $_.category -eq 'opening' -and $_.metric -eq 'search_seconds_ratio'
        } | Select-Object -First 1
        $stableTimed.repeatability | Should Be 'stable_tree_inflation_with_timing_noise'
        $stableTimed.repeatability_status | Should Be 'stable_tree_inflation_with_timing_noise'
        $stableTimed.repeatability_runs | Should Be 10
        $stableTimed.repeatability_exceeded_count | Should Be 7
        $stableTimed.repeatability_notes | Should Match 'stable tree/qtree inflation'

        $stableTree = $details | Where-Object {
            $_.seed -eq 20260425 -and $_.category -eq 'opening' -and $_.metric -eq 'node_ratio'
        } | Select-Object -First 1
        $stableTree.repeatability | Should Be 'stable_tree_inflation'
        $stableTree.repeatability_status | Should Be 'stable_tree_inflation'
        $stableTree.repeatability_runs | Should Be 10
        $stableTree.repeatability_exceeded_count | Should Be 10
        $stableTree.repeatability_notes | Should Match 'nodes and qnodes stable'

        $summaryText = Get-Content -Path (Join-Path $script:Sandbox 'build-local\training\20260424-140000\selection-summary.txt') -Raw
        $summaryText | Should Match 'seed \| category \| metric \| ratio \| threshold \| repeatability \| verdict'
        $summaryText | Should Match '20260423 \| opening \| search_seconds_ratio \| 1\.469059 \| 1\.250000 \| timing_sensitive \| regress'
        $summaryText | Should Match '20260425 \| opening \| search_seconds_ratio \| 2\.159236 \| 1\.250000 \| stable_tree_inflation_with_timing_noise \| regress'
        $summaryText | Should Match '20260425 \| opening \| node_ratio \| 1\.295516 \| 1\.250000 \| stable_tree_inflation \| regress'
    }

    It 'validates repeatability artifact formatting with deterministic fixture inputs' {
        New-SelectionHarness -Sandbox $script:Sandbox -RepeatabilityArtifactProfiles
        $pwsh = (Get-Process -Id $PID).Path
        $harnessPath = Join-Path $script:Sandbox 'scripts\selection-harness.ps1'
        $harnessCommand = "& '$harnessPath' -Seeds 20260423,20260425,20260430"
        & $pwsh -NoProfile -ExecutionPolicy Bypass -Command $harnessCommand | Out-Null

        $sessionRoot = Join-Path $script:Sandbox 'build-local\training\20260424-140000'
        $summary = Get-Content -Path (Join-Path $sessionRoot 'selection-summary.json') -Raw | ConvertFrom-Json
        $details = @($summary.blocker_details)

        $summary.strength_rating | Should Be 'pass'
        $summary.perf_rating | Should Be 'regress'
        $summary.overall_rating | Should Be 'regress'
        $summary.scorecard.regress | Should Be 3
        $summary.scorecard.pass | Should Be 0
        ($summary.per_seed | Where-Object { $_.seed -eq 20260423 } | Select-Object -First 1).rating | Should Be 'regress'
        ($summary.per_seed | Where-Object { $_.seed -eq 20260425 } | Select-Object -First 1).rating | Should Be 'regress'
        ($summary.per_seed | Where-Object { $_.seed -eq 20260430 } | Select-Object -First 1).rating | Should Be 'regress'
        $details.Count | Should Be 4

        $timingSensitive = $details | Where-Object {
            $_.seed -eq 20260423 -and $_.category -eq 'opening' -and $_.metric -eq 'search_seconds_ratio'
        } | Select-Object -First 1
        $timingSensitive.ratio | Should Be 1.469059
        $timingSensitive.repeatability | Should Be 'timing_sensitive'
        $timingSensitive.repeatability_status | Should Be 'timing_sensitive'
        $timingSensitive.repeatability_runs | Should Be 10
        $timingSensitive.repeatability_exceeded_count | Should Be 2
        $timingSensitive.repeatability_notes | Should Match 'time ratio exceeded 2/10'

        $stableTimed = $details | Where-Object {
            $_.seed -eq 20260425 -and $_.category -eq 'opening' -and $_.metric -eq 'search_seconds_ratio'
        } | Select-Object -First 1
        $stableTimed.ratio | Should Be 2.159236
        $stableTimed.repeatability | Should Be 'stable_tree_inflation_with_timing_noise'
        $stableTimed.repeatability_status | Should Be 'stable_tree_inflation_with_timing_noise'
        $stableTimed.repeatability_runs | Should Be 10
        $stableTimed.repeatability_exceeded_count | Should Be 7
        $stableTimed.repeatability_notes | Should Match 'stable tree/qtree inflation'

        $stableTree = $details | Where-Object {
            $_.seed -eq 20260425 -and $_.category -eq 'opening' -and $_.metric -eq 'node_ratio'
        } | Select-Object -First 1
        $stableTree.ratio | Should Be 1.295516
        $stableTree.repeatability | Should Be 'stable_tree_inflation'
        $stableTree.repeatability_status | Should Be 'stable_tree_inflation'
        $stableTree.repeatability_runs | Should Be 10
        $stableTree.repeatability_exceeded_count | Should Be 10
        $stableTree.repeatability_notes | Should Match 'nodes and qnodes stable'

        $unknown = $details | Where-Object {
            $_.seed -eq 20260430 -and $_.category -eq 'opening' -and $_.metric -eq 'search_seconds_ratio'
        } | Select-Object -First 1
        $unknown.ratio | Should Be 1.4
        $unknown.repeatability | Should Be 'unknown'
        $unknown.repeatability_status | Should Be 'unknown'
        ($null -eq $unknown.repeatability_runs) | Should Be $true
        ($null -eq $unknown.repeatability_exceeded_count) | Should Be $true
        ($null -eq $unknown.repeatability_notes) | Should Be $true

        $summaryText = Get-Content -Path (Join-Path $sessionRoot 'selection-summary.txt') -Raw
        $summaryText | Should Match 'seed \| category \| metric \| ratio \| threshold \| repeatability \| verdict'
        $summaryText | Should Match '20260423 \| opening \| search_seconds_ratio \| 1\.469059 \| 1\.250000 \| timing_sensitive \| regress'
        $summaryText | Should Match '20260425 \| opening \| search_seconds_ratio \| 2\.159236 \| 1\.250000 \| stable_tree_inflation_with_timing_noise \| regress'
        $summaryText | Should Match '20260425 \| opening \| node_ratio \| 1\.295516 \| 1\.250000 \| stable_tree_inflation \| regress'
        $summaryText | Should Match '20260430 \| opening \| search_seconds_ratio \| 1\.400000 \| 1\.250000 \| unknown \| regress'
    }

    It 'keeps overall watch when strength watches and performance passes' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') `
            -Seeds 20260428 | Out-Null

        $summary = Get-Content -Path (Join-Path $script:Sandbox 'build-local\training\20260424-140000\selection-summary.json') -Raw | ConvertFrom-Json

        $summary.strength_rating | Should Be 'watch'
        $summary.perf_rating | Should Be 'pass'
        $summary.overall_rating | Should Be 'watch'
    }

    It 'rates fixed score regression as strength regress' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') `
            -Seeds 20260429 | Out-Null

        $summary = Get-Content -Path (Join-Path $script:Sandbox 'build-local\training\20260424-140000\selection-summary.json') -Raw | ConvertFrom-Json

        $summary.strength_rating | Should Be 'regress'
        $summary.perf_rating | Should Be 'pass'
        $summary.per_seed[0].scorecard.score_regressions | Should Be 1
    }

    It 'rates over-threshold performance ratios as perf regress' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') `
            -Seeds 20260427 | Out-Null

        $summary = Get-Content -Path (Join-Path $script:Sandbox 'build-local\training\20260424-140000\selection-summary.json') -Raw | ConvertFrom-Json

        $summary.perf_rating | Should Be 'regress'
        $summary.per_seed[0].scorecard.max_performance_ratio | Should BeGreaterThan 1.25
        $summary.blocker_reason | Should Match 'performance ratio'
    }

    It 'records search_seconds_ratio over threshold as a perf blocker' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') `
            -Seeds 20260430 | Out-Null

        $summary = Get-Content -Path (Join-Path $script:Sandbox 'build-local\training\20260424-140000\selection-summary.json') -Raw | ConvertFrom-Json
        $details = @($summary.blocker_details)

        $summary.perf_rating | Should Be 'regress'
        ($details | Where-Object { $_.metric -eq 'search_seconds_ratio' }).Count | Should Be 1
        ($details | Where-Object { $_.metric -eq 'node_ratio' }).Count | Should Be 0
        $details[0].seed | Should Be 20260430
        $details[0].category | Should Be 'opening'
        $details[0].ratio | Should Be 1.4
    }

    It 'records node_ratio over threshold as a perf blocker' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') `
            -Seeds 20260431 | Out-Null

        $summary = Get-Content -Path (Join-Path $script:Sandbox 'build-local\training\20260424-140000\selection-summary.json') -Raw | ConvertFrom-Json
        $details = @($summary.blocker_details)

        $summary.perf_rating | Should Be 'regress'
        ($details | Where-Object { $_.metric -eq 'node_ratio' }).Count | Should Be 1
        ($details | Where-Object { $_.metric -eq 'search_seconds_ratio' }).Count | Should Be 0
        $details[0].seed | Should Be 20260431
        $details[0].category | Should Be 'opening'
        $details[0].ratio | Should Be 1.4
    }

    It 'records qnodes_ratio over threshold as a perf blocker when search time and nodes pass' {
        New-SelectionHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\selection-harness.ps1') `
            -Seeds 20260432 | Out-Null

        $summary = Get-Content -Path (Join-Path $script:Sandbox 'build-local\training\20260424-140000\selection-summary.json') -Raw | ConvertFrom-Json
        $details = @($summary.blocker_details)

        $summary.perf_rating | Should Be 'regress'
        $summary.per_seed[0].scorecard.max_performance_ratio | Should Be 1.4
        ($details | Where-Object { $_.metric -eq 'qnodes_ratio' }).Count | Should Be 1
        ($details | Where-Object { $_.metric -eq 'search_seconds_ratio' }).Count | Should Be 0
        ($details | Where-Object { $_.metric -eq 'node_ratio' }).Count | Should Be 0
        $details[0].seed | Should Be 20260432
        $details[0].category | Should Be 'opening'
        $details[0].ratio | Should Be 1.4
    }
}

Describe 'perf-runner.ps1' {
    BeforeEach {
        $script:Sandbox = Join-Path ([System.IO.Path]::GetTempPath()) ([System.Guid]::NewGuid().ToString())
        New-Item -ItemType Directory -Path $script:Sandbox | Out-Null
    }

    AfterEach {
        if (Test-Path $script:Sandbox) {
            Remove-Item -Path $script:Sandbox -Recurse -Force
        }
    }

    It 'runs the full baseline matrix and passes tiny-position timing noise when nodes are stable' {
        New-PerfHarness -Sandbox $script:Sandbox
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\perf-harness.ps1') | Out-Null

        $sessionRoot = Join-Path $script:Sandbox 'build-local\perf\20260424-150000'
        $gateJsonPath = Join-Path $sessionRoot 'perf-gate.json'
        $calls = Get-Content -Path (Join-Path $script:Sandbox 'scripts\calls.log')

        ($calls | Where-Object { $_ -like 'start|*|bitboard-benchmark *' }).Count | Should Be 27
        Test-Path $gateJsonPath | Should Be $true

        $gate = Get-Content -Path $gateJsonPath -Raw | ConvertFrom-Json
        $gate.status | Should Be 'pass'
        $gate.failures.Count | Should Be 0
        $gate.results.Count | Should Be 27
        ($gate.results | Where-Object { $_.case_id -eq 'endgame' -and $_.depth -eq 6 -and $_.performance_basis -eq 'node_ratio_tiny_position' }).Count | Should Be 3
    }

    It 'fails when baseline best move drifts or performance regresses' {
        New-PerfHarness -Sandbox $script:Sandbox -Regress
        $pwsh = (Get-Process -Id $PID).Path
        & $pwsh -NoProfile -ExecutionPolicy Bypass -File (Join-Path $script:Sandbox 'scripts\perf-harness.ps1') | Out-Null

        $LASTEXITCODE | Should Not Be 0
        $gate = Get-Content -Path (Join-Path $script:Sandbox 'build-local\perf\20260424-150000\perf-gate.json') -Raw | ConvertFrom-Json
        $gate.status | Should Be 'fail'
        ($gate.failures | Where-Object { $_.reason -eq 'best_move_drift' }).Count | Should Be 1
        ($gate.failures | Where-Object { $_.reason -eq 'performance_regression' }).Count | Should BeGreaterThan 0
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
