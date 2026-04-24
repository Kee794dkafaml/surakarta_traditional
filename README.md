# Surakarta Core

This repository is a library for https://github.com/surakarta-game/surakarta-game.

Here you can find the source code for libsurakarta, a library that provides rule management and a simple AI for https://github.com/surakarta-game/surakarta-game, together with a simple CLI tool for benchmark and some unit tests for the library.

Use the following command to clone the repository with dependencies:
```bash
git clone https://github.com/surakarta-game/surakarta-core.git --recursive
```

## How to compile

You can use GCC, Clang or MSVC to build the project on Linux (or other UNIX-like OS) or Windows. Unit tests are not available for Windows.

### Linux

You can compile the project on UNIX-like OS with the following commands:
```bash
mkdir build
cd build
cmake ..
make
```

Binary outputs can be found in build/bin and build/lib:
 - build/bin/surakarta-benchmark: a simple CLI tool for benchmark
 - build/bin/surakarta-test: unit tests
 - build/lib/libsurakarta.a: the library that contains rule manager and the simple AI

 ### Windows

 You can use Visual Studio to compile this repository. VS supports CMake very well, and we have tested to build this CMake project with MSVC.

### Windows workflow

Default regression, manual rule acceptance, and training smoke are intentionally separated now. The current gate order is:

`baseline snapshot -> default regression -> manual GUI rule acceptance -> TD/self-play smoke -> Phase 4B selection work`

```powershell
pwsh ./scripts/workflow.ps1 -Configuration Debug
pwsh ./scripts/workflow.ps1 -Configuration Release
pwsh ./scripts/manual-acceptance.ps1
pwsh ./scripts/perf-runner.ps1
pwsh ./scripts/benchmark-train.ps1
```

- `workflow.ps1` configures, builds, runs the default CTest suite, and checks for lingering `surakarta-*.exe` processes under the workspace. If the selected build directory still points at a stale source tree in `CMakeCache.txt`, the script clears that generated build directory and reconfigures from the current repo root before continuing.
- The default CTest suite contains deterministic regression only: `surakarta-bitboard-selftest`, `surakarta-dev-session-selftest`, and GUI smoke tests.
- `surakarta-benchmark statistic -n 10` is no longer part of default regression. Keep it as a manual legacy command only.
- `manual-acceptance.ps1` builds `surakarta-gui`, creates an archive scaffold under `build-local/manual-acceptance/<timestamp>/`, copies the checklist template, records the current git commit, and launches the GUI for the required manual rule scenarios.
- `perf-runner.ps1` runs the fixed perf suite serially for `opening`, `middlegame` (`game1`), and `endgame` (`game6`) across depths `6/7/8` and threads `1/2/4`. Reports are written to `build-local/perf/<timestamp>/`.
- `benchmark-train.ps1` is the formal TD/self-play smoke gate. It builds `surakarta-benchmark`, runs two identical training passes with the same seed, verifies `candidate.bin != bootstrap.bin`, checks `bitboard-search` and `bitboard-benchmark` loading, writes `bitboard-eval` JSON/text reports, and compares hashes between the two runs. Artifacts are archived under `build-local/training/<timestamp>/run-1/` and `run-2/`, with `repro-summary.json` and `repro-summary.txt` stored at the timestamp root.
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

 All the dependencies are used for testing. So if you don't want to test, all the dependencies are unnecessary.
