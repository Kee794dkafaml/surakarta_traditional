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
