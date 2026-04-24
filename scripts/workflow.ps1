param(
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Debug',
    [string]$BuildDir
)

. (Join-Path $PSScriptRoot 'common.ps1')

if (-not $BuildDir) {
    $BuildDir = Get-SurakartaBuildDir -Configuration $Configuration
}

Invoke-SurakartaConfigure -Configuration $Configuration -BuildDir $BuildDir
Invoke-SurakartaBuild -BuildDir $BuildDir -CleanFirst -Targets @(
    'surakarta-benchmark',
    'surakarta-bitboard-selftest',
    'surakarta-dev-session-selftest',
    'surakarta-gui'
)
Invoke-SurakartaCTest -BuildDir $BuildDir
Assert-NoSurakartaWorkspaceProcesses

Write-Host "[PASS] workflow $Configuration"
