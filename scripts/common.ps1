Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Get-SurakartaRepoRoot {
    return (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
}

function Assert-LastNativeExitCode {
    param([string]$Step)

    if ($LASTEXITCODE -ne 0) {
        throw "$Step failed with exit code $LASTEXITCODE."
    }
}

function Get-SurakartaBuildDir {
    param(
        [ValidateSet('Debug', 'Release')]
        [string]$Configuration
    )

    $root = Get-SurakartaRepoRoot
    if ($Configuration -eq 'Debug') {
        return (Join-Path $root 'build-vs')
    }
    return (Join-Path $root 'build-vs-release')
}

function Get-CMakeCacheValue {
    param(
        [string]$BuildDir,
        [string]$Name
    )

    $cacheFile = Join-Path $BuildDir 'CMakeCache.txt'
    if (-not (Test-Path $cacheFile)) {
        return $null
    }

    $match = Select-String -Path $cacheFile -Pattern "^${Name}:(?:FILEPATH|INTERNAL|STRING|UNINITIALIZED|BOOL)=(.*)$" | Select-Object -First 1
    if ($null -eq $match) {
        return $null
    }

    return $match.Matches[0].Groups[1].Value
}

function Get-DefaultToolPath {
    param(
        [ValidateSet('cmake', 'ctest', 'ninja', 'launchdevshell')]
        [string]$Tool
    )

    $candidates = @()
    if ($env:VSINSTALLDIR) {
        switch ($Tool) {
            'cmake' { $candidates += (Join-Path $env:VSINSTALLDIR 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe') }
            'ctest' { $candidates += (Join-Path $env:VSINSTALLDIR 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe') }
            'ninja' { $candidates += (Join-Path $env:VSINSTALLDIR 'Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe') }
            'launchdevshell' { $candidates += (Join-Path $env:VSINSTALLDIR 'Common7\Tools\Launch-VsDevShell.ps1') }
        }
    }

    switch ($Tool) {
        'cmake' { $candidates += 'D:\Dev\App\VS2022\VS2022\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' }
        'ctest' { $candidates += 'D:\Dev\App\VS2022\VS2022\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\ctest.exe' }
        'ninja' { $candidates += 'D:\Dev\App\VS2022\VS2022\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe' }
        'launchdevshell' { $candidates += 'D:\Dev\App\VS2022\VS2022\Common7\Tools\Launch-VsDevShell.ps1' }
    }

    foreach ($candidate in $candidates) {
        if ($candidate -and (Test-Path $candidate)) {
            return $candidate
        }
    }

    throw "Unable to locate tool '$Tool'."
}

function Get-CMakePath {
    param([string]$BuildDir)

    $fromCache = Get-CMakeCacheValue -BuildDir $BuildDir -Name 'CMAKE_COMMAND'
    if ($fromCache -and (Test-Path $fromCache)) {
        return $fromCache
    }
    return Get-DefaultToolPath -Tool 'cmake'
}

function Get-CTestPath {
    param([string]$BuildDir)

    $fromCache = Get-CMakeCacheValue -BuildDir $BuildDir -Name 'CMAKE_CTEST_COMMAND'
    if ($fromCache -and (Test-Path $fromCache)) {
        return $fromCache
    }
    return Get-DefaultToolPath -Tool 'ctest'
}

function Get-NinjaPath {
    param([string]$BuildDir)

    $fromCache = Get-CMakeCacheValue -BuildDir $BuildDir -Name 'CMAKE_MAKE_PROGRAM'
    if ($fromCache -and (Test-Path $fromCache)) {
        return $fromCache
    }
    return Get-DefaultToolPath -Tool 'ninja'
}

function Get-LaunchVsDevShellPath {
    param([string]$BuildDir)

    $cmakePath = Get-CMakePath -BuildDir $BuildDir
    if ($cmakePath -match '^(.*?)[/\\]Common7[/\\]IDE[/\\]CommonExtensions[/\\]Microsoft[/\\]CMake[/\\]CMake[/\\]bin[/\\]cmake\.exe$') {
        $candidate = Join-Path $Matches[1] 'Tools\Launch-VsDevShell.ps1'
        if (Test-Path $candidate) {
            return $candidate
        }
    }

    return Get-DefaultToolPath -Tool 'launchdevshell'
}

function Import-SurakartaDevShell {
    param([string]$BuildDir)

    . (Get-LaunchVsDevShellPath -BuildDir $BuildDir) -Arch amd64 -HostArch amd64 | Out-Null
}

function Invoke-SurakartaConfigure {
    param(
        [ValidateSet('Debug', 'Release')]
        [string]$Configuration,
        [string]$BuildDir = (Get-SurakartaBuildDir -Configuration $Configuration)
    )

    Import-SurakartaDevShell -BuildDir $BuildDir
    $cmake = Get-CMakePath -BuildDir $BuildDir
    $root = Get-SurakartaRepoRoot
    & $cmake -S $root -B $BuildDir -G Ninja "-DCMAKE_BUILD_TYPE=$Configuration" -DSURAKARTA_ENABLE_GUI=ON
    Assert-LastNativeExitCode -Step "cmake configure"
}

function Invoke-SurakartaBuild {
    param(
        [string]$BuildDir,
        [switch]$CleanFirst,
        [string[]]$Targets
    )

    Import-SurakartaDevShell -BuildDir $BuildDir
    $ninja = Get-NinjaPath -BuildDir $BuildDir

    if ($CleanFirst) {
        # CMAKE_CXX_CL_SHOWINCLUDES_PREFIX=注意: 包含文件: still leaves `ninja -t deps` at `#deps 0` here.
        Write-Host "[INFO] Header dependency tracking is unreliable in this Windows + Ninja + localized MSVC toolchain; using correctness-first clean rebuild for workflow validation."
        & $ninja -C $BuildDir -t clean
        Assert-LastNativeExitCode -Step "ninja clean"
    }

    & $ninja -C $BuildDir @Targets
    Assert-LastNativeExitCode -Step "ninja build"
}

function Invoke-SurakartaCTest {
    param([string]$BuildDir)

    $ctest = Get-CTestPath -BuildDir $BuildDir
    & $ctest --test-dir $BuildDir --output-on-failure
    Assert-LastNativeExitCode -Step "ctest"
}

function Get-SurakartaWorkspaceProcesses {
    param([string]$WorkspaceRoot = (Get-SurakartaRepoRoot))

    $normalizedRoot = [System.IO.Path]::GetFullPath($WorkspaceRoot).TrimEnd('\')
    return @(Get-CimInstance Win32_Process | Where-Object {
        $_.Name -like 'surakarta-*.exe' -and
        $_.ExecutablePath -and
        ([System.IO.Path]::GetFullPath($_.ExecutablePath)).StartsWith($normalizedRoot, [System.StringComparison]::OrdinalIgnoreCase)
    } | Select-Object @{
        Name = 'ProcessId'
        Expression = { $_.ProcessId }
    }, Name, ExecutablePath, CommandLine)
}

function Stop-SurakartaWorkspaceProcesses {
    param(
        [string]$WorkspaceRoot = (Get-SurakartaRepoRoot),
        [int[]]$ExcludeProcessId = @()
    )

    $processes = @(Get-SurakartaWorkspaceProcesses -WorkspaceRoot $WorkspaceRoot | Where-Object {
        $ExcludeProcessId -notcontains $_.ProcessId
    })

    foreach ($process in $processes) {
        Stop-Process -Id $process.ProcessId -Force -ErrorAction SilentlyContinue
    }

    return $processes
}

function Assert-NoSurakartaWorkspaceProcesses {
    param([string]$WorkspaceRoot = (Get-SurakartaRepoRoot))

    $leftovers = @(Get-SurakartaWorkspaceProcesses -WorkspaceRoot $WorkspaceRoot)
    if ($leftovers.Count -gt 0) {
        $summary = $leftovers | ForEach-Object {
            "{0}:{1}" -f $_.ProcessId, $_.ExecutablePath
        }
        throw "Found lingering Surakarta workspace processes: $($summary -join '; ')"
    }
}
