param(
    [string]$BuildDir = (Join-Path (Resolve-Path (Join-Path $PSScriptRoot '..')).Path 'build-vs-release'),
    [int]$TimeoutSeconds = 90,
    [int]$MovegenIterations = 20000
)

. (Join-Path $PSScriptRoot 'common.ps1')

$repoRoot = Get-SurakartaRepoRoot
$benchmarkExe = Join-Path $BuildDir 'bin\surakarta-benchmark.exe'
$outputRoot = Join-Path $repoRoot 'build-local\perf'
$timestamp = Get-Date -Format 'yyyyMMdd-HHmmss'
$runRoot = Join-Path $outputRoot $timestamp
$logsDir = Join-Path $runRoot 'logs'

Invoke-SurakartaConfigure -Configuration 'Release' -BuildDir $BuildDir
Invoke-SurakartaBuild -BuildDir $BuildDir -Targets @('surakarta-benchmark')

New-Item -ItemType Directory -Force -Path $logsDir | Out-Null
Stop-SurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot | Out-Null

$cases = @(
    [pscustomobject]@{ CaseId = 'opening'; File = $null },
    [pscustomobject]@{ CaseId = 'middlegame'; File = (Join-Path $repoRoot 'test\test_data\game1.txt') },
    [pscustomobject]@{ CaseId = 'endgame'; File = (Join-Path $repoRoot 'test\test_data\game6.txt') }
)
$depths = @(6, 7, 8)
$threads = @(1, 2, 4)
$results = New-Object System.Collections.Generic.List[object]
$baselineSeconds = @{}

foreach ($case in $cases) {
    foreach ($depth in $depths) {
        foreach ($threadCount in $threads) {
            $tag = '{0}-d{1}-t{2}' -f $case.CaseId, $depth, $threadCount
            $stdoutLog = Join-Path $logsDir "$tag.stdout.log"
            $stderrLog = Join-Path $logsDir "$tag.stderr.log"
            $args = @(
                'bitboard-benchmark',
                '--format', 'json',
                '--case-id', $case.CaseId,
                '--depth', "$depth",
                '--threads', "$threadCount",
                '--movegen-iters', "$MovegenIterations"
            )
            if ($case.File) {
                $args += @('--file', $case.File)
            }

            $startTime = Get-Date
            $process = Start-Process -FilePath $benchmarkExe `
                -ArgumentList $args `
                -WorkingDirectory (Join-Path $BuildDir 'bin') `
                -RedirectStandardOutput $stdoutLog `
                -RedirectStandardError $stderrLog `
                -NoNewWindow `
                -PassThru

            $timedOut = $false
            try {
                Wait-Process -Id $process.Id -Timeout $TimeoutSeconds -ErrorAction Stop
            } catch {
                $timedOut = $true
                Stop-Process -Id $process.Id -Force -ErrorAction SilentlyContinue
                Wait-Process -Id $process.Id -ErrorAction SilentlyContinue
            }

            $process.Refresh()
            $wallSeconds = [Math]::Round(((Get-Date) - $startTime).TotalSeconds, 6)
            $cleaned = @(Stop-SurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot -ExcludeProcessId @($PID))
            if ($timedOut) {
                throw "Perf case $tag timed out after $TimeoutSeconds seconds. See $stdoutLog and $stderrLog."
            }
            if ($process.ExitCode -ne 0) {
                throw "Perf case $tag failed with exit code $($process.ExitCode). See $stdoutLog and $stderrLog."
            }

            $rawJson = Get-Content -Path $stdoutLog -Raw
            $record = $rawJson | ConvertFrom-Json
            if ($threadCount -eq 1) {
                $baselineSeconds["$($case.CaseId)|$depth"] = [double]$record.search_seconds
                $speedup = 1.0
            } else {
                $baselineKey = "$($case.CaseId)|$depth"
                if (-not $baselineSeconds.ContainsKey($baselineKey)) {
                    throw "Missing 1T baseline for $baselineKey before computing speedup."
                }
                $speedup = $baselineSeconds[$baselineKey] / [double]$record.search_seconds
            }

            $results.Add([pscustomobject]@{
                case_id = $record.case_id
                depth = [int]$record.depth
                threads = [int]$record.threads
                search_seconds = [double]$record.search_seconds
                nodes = [uint64]$record.nodes
                nps = [uint64]$record.nps
                tt_hits = [uint64]$record.tt_hits
                null_prunes = [uint64]$record.null_prunes
                lmr_reductions = [uint64]$record.lmr_reductions
                fail_highs = [uint64]$record.fail_highs
                fail_lows = [uint64]$record.fail_lows
                aspiration_retries = [uint64]$record.aspiration_retries
                pv = [string]$record.pv
                best_move = [string]$record.best_move
                score = [int]$record.score
                speedup = [Math]::Round($speedup, 6)
                wall_seconds = $wallSeconds
                stdout_log = $stdoutLog
                stderr_log = $stderrLog
                cleaned_processes = $cleaned.Count
            })
        }
    }
}

$jsonPath = Join-Path $runRoot 'perf-summary.json'
$textPath = Join-Path $runRoot 'perf-summary.txt'

$results | ConvertTo-Json -Depth 5 | Set-Content -Path $jsonPath

$lines = New-Object System.Collections.Generic.List[string]
$lines.Add("Surakarta perf suite")
$lines.Add("Output: $runRoot")
$lines.Add('')

foreach ($caseId in @('opening', 'middlegame', 'endgame')) {
    foreach ($depth in $depths) {
        $lines.Add("$caseId depth $depth")
        $subset = $results | Where-Object { $_.case_id -eq $caseId -and $_.depth -eq $depth } | Sort-Object threads
        foreach ($entry in $subset) {
            $lines.Add(("  {0}T  nodes={1}  nps={2}  tt_hits={3}  speedup={4:N3}  search_seconds={5:N6}  pv={6}" -f `
                $entry.threads,
                $entry.nodes,
                $entry.nps,
                $entry.tt_hits,
                $entry.speedup,
                $entry.search_seconds,
                $entry.pv))
        }
        $lines.Add('')
    }
}

$lines | Set-Content -Path $textPath
Assert-NoSurakartaWorkspaceProcesses -WorkspaceRoot $repoRoot

Write-Host "[PASS] perf runner"
Write-Host "JSON: $jsonPath"
Write-Host "Text: $textPath"
