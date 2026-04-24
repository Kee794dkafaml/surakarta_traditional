# Manual Acceptance Checklist

Archive this filled checklist under `build-local/manual-acceptance/<timestamp>/checklist.md`.

## Capture Rules

- Record `Expected`, `Actual`, and `Conclusion` for every scenario.
- Save required screenshots into `screenshots/`.
- Use `surakarta-gui` as the formal manual acceptance surface.
- `game7.txt` is reference-only. Do not use it as the formal `no_capture_limit` gate.

## Scenario 1: Opening Quiet Move

- Setup: launch `surakarta-gui` on the default opening position with `--human black`.
- Steps:
  1. Select `(0,1)`.
  2. Move to `(0,2)`.
- Expected: move is accepted, board updates, engine can continue responding.
- Actual:
- Conclusion:
- Screenshot: `opening-move.png`

## Scenario 2: `game1.txt` Legal And Illegal Targets

- Setup: load `test/test_data/game1.txt`.
- Steps:
  1. Select `(2,2)`.
  2. Inspect the highlighted targets.
- Expected:
  - `(2,0)` is shown as a legal capture target.
  - `(2,3)` is shown as a legal non-capture target.
  - `(2,1)` is not highlighted and cannot be executed.
  - Out-of-board targets are not selectable or highlighted.
- Actual:
- Conclusion:
- Screenshot: `game1-targets.png`

## Scenario 3: `game1.txt` Capture Gate

- Setup: keep `game1.txt` loaded.
- Steps:
  1. Verify `(4,4) -> (5,1)` executes as a legal capture.
  2. Reload `game1.txt`.
  3. Verify `(4,5) -> (5,1)` is rejected or cannot be executed.
- Expected: the legal capture succeeds; the illegal capture does not.
- Actual:
- Conclusion:

## Scenario 4: `game2.txt` White Capture Paths

- Setup: load `test/test_data/game2.txt`, set the human side to white if needed.
- Steps:
  1. Execute `(4,4) -> (3,4)`.
  2. Reload `game2.txt`.
  3. Execute `(3,5) -> (5,3)`.
- Expected: both white capture moves are accepted.
- Actual:
- Conclusion:

## Scenario 5: `game6.txt` Terminal Capture

- Setup: load `test/test_data/game6.txt`.
- Steps:
  1. Execute `(5,1) -> (1,1)`.
- Expected: GUI shows `CHECKMATE`.
- Actual:
- Conclusion:
- Screenshot: `game6-checkmate.png`

## Scenario 6: `game6.txt` `no_capture_limit` Terminal Gate

- Setup: reload `test/test_data/game6.txt`.
- Steps:
  1. Execute the legal non-capture move `(5,2) -> (4,2)`.
- Expected: GUI shows `STALEMATE`.
- Actual:
- Conclusion:
- Screenshot: `game6-stalemate.png`

## Reference: `game7.txt`

- Purpose: reference-only near-threshold position for `no_capture_limit`.
- Note: because `JudgeEnd()` runs before `num_round_++`, `game7.txt` is not the formal one-move terminal gate.
- Actual observations:
