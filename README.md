# Surakarta Core

This repository is a library for https://github.com/surakarta-game/surakarta-game.

Here you can find the source code for libsurakarta, a library that provides rule management and a simple AI for https://github.com/surakarta-game/surakarta-game, together with a simple CLI tool for benchmark and some unit tests for the library.

Use the following command to clone the repository:
```bash
git clone https://github.com/surakarta-game/surakarta-core.git
```

## How to compile

This repository is maintained around the Windows/MSVC toolchain. Visual Studio supports this CMake project well, and the current workflow has been exercised with MSVC.

Binary outputs can be found in `build/bin` and `build/lib`:
 - `build/bin/surakarta-benchmark.exe`: a simple CLI tool for benchmark
 - `build/bin/surakarta-bitboard-selftest.exe`: deterministic bitboard regression selftest
 - `build/bin/surakarta-dev-session-selftest.exe`: development session regression selftest
 - `build/bin/surakarta-gui.exe`: Dear ImGui validation tool
 - `build/bin/surakarta.dll`: the library that contains rule manager and the simple AI

### Windows workflow

Default regression, manual rule acceptance, and training smoke are intentionally separated now. The current gate order is:

`baseline snapshot -> default regression -> manual GUI rule acceptance -> TD/self-play smoke -> Phase 4B selection work`

```powershell
pwsh ./scripts/workflow.ps1 -Configuration Debug
pwsh ./scripts/workflow.ps1 -Configuration Release
pwsh ./scripts/manual-acceptance.ps1
pwsh ./scripts/perf-runner.ps1
pwsh ./scripts/benchmark-train.ps1
pwsh ./scripts/benchmark-select.ps1
```

- `workflow.ps1` configures, builds, runs the default CTest suite, and checks for lingering `surakarta-*.exe` processes under the workspace. If the selected build directory still points at a stale source tree in `CMakeCache.txt`, the script clears that generated build directory and reconfigures from the current repo root before continuing.
- The default CTest suite contains deterministic regression only: `surakarta-bitboard-selftest`, `surakarta-dev-session-selftest`, and GUI smoke tests.
- `surakarta-benchmark statistic -n 10` is no longer part of default regression. Keep it as a manual legacy command only.
- `manual-acceptance.ps1` builds `surakarta-gui`, creates an archive scaffold under `build-local/manual-acceptance/<timestamp>/`, copies the checklist template, records the current git commit, and launches the GUI for the required manual rule scenarios.
- `perf-runner.ps1` runs the fixed perf suite serially for `opening`, `middlegame` (`game1`), and `endgame` (`game6`) across depths `6/7/8` and threads `1/2/4`. It reads the repository baseline from `test/test_data/perf-baseline.json`, writes `perf-summary.json` / `perf-gate.json`, and fails on best-move drift, score drift, or perf regression beyond the gate threshold.
- `benchmark-train.ps1` is the formal TD/self-play smoke gate. It builds `surakarta-benchmark`, runs two identical training passes with the same seed, verifies `candidate.bin != bootstrap.bin`, checks `bitboard-search` and `bitboard-benchmark` loading, writes `bitboard-eval` JSON/text reports, and compares hashes between the two runs. Artifacts are archived under `build-local/training/<timestamp>/run-1/` and `run-2/`, with per-artifact `*.manifest.json` sidecars and `weights-manifest.json` stored at the session root.
- `benchmark-select.ps1` is the multi-seed observation gate. It runs several training seeds, stores per-seed `bootstrap.bin`, `candidate.bin`, and `bitboard-eval` reports under `build-local/training/<timestamp>/selection/seed-<seed>/`, and emits `selection-summary.json` / `selection-summary.txt` plus per-seed and session `weights-manifest.json` files for pass/watch/regress scorecards.
- Until the baseline snapshot, manual rule acceptance, and TD/self-play smoke gate all pass on fresh artifacts, Phase 4B checkpoint ranking and extended benchmark selection stay frozen.

### Benchmark CLI

`surakarta-benchmark` keeps text output by default and now also supports structured JSON output:

```powershell
./build-vs-release/bin/surakarta-benchmark.exe bitboard-search --format json --case-id opening
./build-vs-release/bin/surakarta-benchmark.exe bitboard-benchmark --format json --case-id middlegame --file ./test/test_data/game1.txt
./build-vs-release/bin/surakarta-benchmark.exe bitboard-train --output ./build-local/training/bootstrap.bin --games 0 --format json
./build-vs-release/bin/surakarta-benchmark.exe bitboard-eval --candidate ./build-local/training/bootstrap.bin --depth 4 --format json
```

JSON output includes:

- `case_id`
- `depth`
- `threads`
- `search_seconds`
- `nodes`
- `nps`
- `tt_hits`
- `null_prunes`
- `lmr_reductions`
- `fail_highs`
- `fail_lows`
- `aspiration_retries`
- `pv`
- `best_move`
- `score`

`bitboard-train` emits a structured training summary, while `bitboard-eval` emits a structured report with:

- `candidate_weights`
- `baseline_weights`
- `depth`
- `position_reports`
- `match_summary`
- `match_games`

 ## Dependencies

 The current Windows workflow does not require extra git submodules. When `SURAKARTA_ENABLE_GUI` is left enabled (the default), the first configure will download `glfw` and `imgui` via `FetchContent`. If you only need the core library and benchmark targets, you can disable the GUI target during configure.
