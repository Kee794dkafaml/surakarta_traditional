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

function Get-NormalizedFullPath {
    param([string]$Path)

    return [System.IO.Path]::GetFullPath($Path).TrimEnd('\')
}

function Test-SurakartaGitIgnoredPath {
    param(
        [string]$RepoRoot,
        [string]$Path
    )

    if (-not (Test-Path (Join-Path $RepoRoot '.git'))) {
        return $false
    }

    $relativePath = [System.IO.Path]::GetRelativePath($RepoRoot, $Path)
    if ([string]::IsNullOrWhiteSpace($relativePath) -or $relativePath.StartsWith('..')) {
        return $false
    }

    $git = Get-Command git -ErrorAction SilentlyContinue
    if ($null -eq $git) {
        return $false
    }

    & $git.Source -C $RepoRoot check-ignore -q -- $relativePath
    return $LASTEXITCODE -eq 0
}

function Clear-StaleSurakartaBuildDir {
    param(
        [string]$BuildDir,
        [string]$RepoRoot = (Get-SurakartaRepoRoot)
    )

    $cacheRoot = Get-CMakeCacheValue -BuildDir $BuildDir -Name 'CMAKE_HOME_DIRECTORY'
    if (-not $cacheRoot) {
        return $false
    }

    $normalizedRepoRoot = Get-NormalizedFullPath -Path $RepoRoot
    $normalizedBuildDir = Get-NormalizedFullPath -Path $BuildDir
    $normalizedCacheRoot = Get-NormalizedFullPath -Path $cacheRoot

    if ($normalizedCacheRoot.Equals($normalizedRepoRoot, [System.StringComparison]::OrdinalIgnoreCase)) {
        return $false
    }

    $repoPrefix = $normalizedRepoRoot + '\'
    if (-not $normalizedBuildDir.StartsWith($repoPrefix, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing to clear stale build directory outside repo root: $normalizedBuildDir"
    }

    $relativePath = [System.IO.Path]::GetRelativePath($normalizedRepoRoot, $normalizedBuildDir)
    $looksLikeGeneratedBuildDir = $relativePath -like 'build*' -or
                                  $relativePath -like '.worktrees*' -or
                                  $relativePath -like 'worktrees*'
    $ignoredByGit = Test-SurakartaGitIgnoredPath -RepoRoot $normalizedRepoRoot -Path $normalizedBuildDir

    if (-not $ignoredByGit -and -not $looksLikeGeneratedBuildDir) {
        throw "Refusing to clear non-ignored directory for stale cache recovery: $normalizedBuildDir"
    }

    Remove-Item -LiteralPath $normalizedBuildDir -Recurse -Force
    return $true
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

    $null = Clear-StaleSurakartaBuildDir -BuildDir $BuildDir
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

function Get-SurakartaGitCommit {
    param([string]$RepoRoot = (Get-SurakartaRepoRoot))

    $git = Get-Command git -ErrorAction SilentlyContinue
    if ($null -eq $git) {
        throw 'Unable to resolve git commit because git is not available.'
    }

    $commit = (& $git.Source -C $RepoRoot rev-parse HEAD 2>$null)
    if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrWhiteSpace($commit)) {
        throw "Unable to resolve git commit for $RepoRoot."
    }

    return [string]$commit.Trim()
}

function Get-SurakartaFileSha256 {
    param([string]$Path)

    if (-not (Test-Path $Path)) {
        throw "Missing expected file: $Path"
    }

    return (Get-FileHash -Path $Path -Algorithm SHA256).Hash
}

function Get-SurakartaNTupleWeightHeader {
    param([string]$Path)

    if (-not (Test-Path $Path)) {
        throw "Missing expected weight file: $Path"
    }

    $stream = [System.IO.File]::Open($Path, [System.IO.FileMode]::Open, [System.IO.FileAccess]::Read, [System.IO.FileShare]::Read)
    $reader = [System.IO.BinaryReader]::new($stream)
    try {
        if ($stream.Length -lt 20) {
            throw "Weight file is too small to contain an NTuple header: $Path"
        }

        $magic = $reader.ReadUInt32()
        $version = $reader.ReadUInt16()
        $reserved = $reader.ReadUInt16()
        $tupleSetHash = $reader.ReadUInt64()
        $weightCount = $reader.ReadUInt32()

        return [pscustomobject]@{
            magic = ('0x{0:X8}' -f $magic)
            version = [int]$version
            reserved = [int]$reserved
            tuple_set_hash = ('0x{0:X16}' -f $tupleSetHash)
            weight_count = [uint32]$weightCount
        }
    } finally {
        $reader.Dispose()
        $stream.Dispose()
    }
}

function Test-SurakartaObjectProperty {
    param(
        [object]$Value,
        [string]$Name
    )

    return $null -ne ($Value.PSObject.Properties[$Name])
}

function Assert-SurakartaWeightManifestTraceable {
    param(
        [object]$Manifest,
        [string]$Label = 'weight manifest'
    )

    foreach ($field in @('git_commit', 'checkpoint', 'binary_sha256')) {
        if (-not (Test-SurakartaObjectProperty -Value $Manifest -Name $field)) {
            throw "$Label is missing required traceability field '$field'."
        }
        if ($null -eq $Manifest.$field) {
            throw "$Label has null required traceability field '$field'."
        }
    }

    if ([string]::IsNullOrWhiteSpace([string]$Manifest.git_commit)) {
        throw "$Label has empty required traceability field 'git_commit'."
    }
    if ([string]::IsNullOrWhiteSpace([string]$Manifest.binary_sha256)) {
        throw "$Label has empty required traceability field 'binary_sha256'."
    }
}

function New-SurakartaWeightManifest {
    param(
        [string]$RepoRoot,
        [string]$WeightPath,
        [string]$ArtifactKind,
        [int]$Seed,
        [int]$Games,
        [int]$Depth,
        [double]$Alpha,
        [double]$Lambda,
        [double]$Epsilon,
        [int]$EpsilonPlies,
        [double]$TerminalReward = 1200.0,
        [double]$TdErrorClip = 0.0,
        [int]$TerminalOnlyWarmup = 0,
        [int]$NearTerminalCurriculum = 0,
        [int]$CheckpointEvery,
        [object]$Checkpoint
    )

    $fullWeightPath = [System.IO.Path]::GetFullPath($WeightPath)
    $header = Get-SurakartaNTupleWeightHeader -Path $fullWeightPath
    $checkpointValue = if ($null -ne $Checkpoint) {
        $Checkpoint
    } else {
        [ordered]@{
            is_checkpoint = $false
            name = $null
            games_completed = $null
        }
    }

    $manifest = [ordered]@{
        manifest_version = 1
        artifact_kind = $ArtifactKind
        git_commit = Get-SurakartaGitCommit -RepoRoot $RepoRoot
        weights_path = $fullWeightPath
        weights_relative_path = [System.IO.Path]::GetRelativePath($RepoRoot, $fullWeightPath)
        seed = $Seed
        games = $Games
        depth = $Depth
        alpha = $Alpha
        lambda = $Lambda
        epsilon = $Epsilon
        epsilon_plies = $EpsilonPlies
        terminal_reward = $TerminalReward
        td_error_clip = $TdErrorClip
        terminal_only_warmup = $TerminalOnlyWarmup
        near_terminal_curriculum = $NearTerminalCurriculum
        checkpoint_every = $CheckpointEvery
        checkpoint = $checkpointValue
        tuple_set_hash = $header.tuple_set_hash
        weight_count = [int]$header.weight_count
        binary_sha256 = Get-SurakartaFileSha256 -Path $fullWeightPath
    }

    Assert-SurakartaWeightManifestTraceable -Manifest ([pscustomobject]$manifest) -Label "$ArtifactKind manifest"
    return $manifest
}

function Write-SurakartaWeightManifest {
    param(
        [string]$RepoRoot,
        [string]$WeightPath,
        [string]$ArtifactKind,
        [int]$Seed,
        [int]$Games,
        [int]$Depth,
        [double]$Alpha,
        [double]$Lambda,
        [double]$Epsilon,
        [int]$EpsilonPlies,
        [double]$TerminalReward = 1200.0,
        [double]$TdErrorClip = 0.0,
        [int]$TerminalOnlyWarmup = 0,
        [int]$NearTerminalCurriculum = 0,
        [int]$CheckpointEvery,
        [object]$Checkpoint
    )

    $manifest = New-SurakartaWeightManifest -RepoRoot $RepoRoot `
        -WeightPath $WeightPath `
        -ArtifactKind $ArtifactKind `
        -Seed $Seed `
        -Games $Games `
        -Depth $Depth `
        -Alpha $Alpha `
        -Lambda $Lambda `
        -Epsilon $Epsilon `
        -EpsilonPlies $EpsilonPlies `
        -TerminalReward $TerminalReward `
        -TdErrorClip $TdErrorClip `
        -TerminalOnlyWarmup $TerminalOnlyWarmup `
        -NearTerminalCurriculum $NearTerminalCurriculum `
        -CheckpointEvery $CheckpointEvery `
        -Checkpoint $Checkpoint
    $manifestPath = "$WeightPath.manifest.json"
    $manifest | ConvertTo-Json -Depth 8 | Set-Content -Path $manifestPath
    return [pscustomobject]@{
        Path = $manifestPath
        Manifest = [pscustomobject]$manifest
    }
}

function Write-SurakartaSessionWeightManifest {
    param(
        [string]$RepoRoot,
        [string]$Path,
        [string]$Purpose,
        [object[]]$Artifacts
    )

    $manifest = [ordered]@{
        manifest_version = 1
        purpose = $Purpose
        git_commit = Get-SurakartaGitCommit -RepoRoot $RepoRoot
        artifacts = @($Artifacts | ForEach-Object { $_.Manifest })
    }
    $manifest | ConvertTo-Json -Depth 10 | Set-Content -Path $Path
    return [pscustomobject]$manifest
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
