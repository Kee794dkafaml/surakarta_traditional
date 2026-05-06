# Human-Turn Undo Design

## Context

The Dear ImGui validation tool runs human-vs-engine games through `surakarta::devtools::DevelopmentSession`. The session owns the current bitboard `Position`, human side, asynchronous engine search state, selected square, legal targets, hover path, last move, and last move path.

Engine replies are applied automatically when a completed search is joined. Today the session has no GUI-facing history, even though the bitboard layer has low-level `Undo` support for search internals.

## Goal

Add a GUI undo feature where clicking once returns the game to the previous position where it was the human player's turn.

The button is only enabled when all of these are true:

- The engine is idle.
- The current side to move is the human side.
- A previous human-turn snapshot exists.

In the normal human-vs-engine flow, one click after the engine replies will undo the human move and the engine reply together, restoring the board to the state before the human's previous move.

## Non-Goals

- Do not add arbitrary move browsing or redo.
- Do not expose undo in the legacy `SurakartaGame` API.
- Do not allow undo while the engine search is running.
- Do not preserve undo history across reset, loaded positions, or human-side changes.

## Recommended Approach

Store human-turn snapshots in `DevelopmentSession`.

Add a private `HumanTurnSnapshot` struct containing the session state needed to restore a human-turn position:

- `bitboard::Position position`
- `SessionStatus status`
- `std::optional<bitboard::Move> last_move`
- `std::vector<SurakartaMovePathFragment> last_move_path`

Add `std::vector<HumanTurnSnapshot> undo_stack_`.

Before `ApplyHumanMove()` commits a legal human move, push the current snapshot. The engine reply should not push another snapshot. This makes the stack represent "positions where the human was allowed to move" instead of raw ply history.

Add public APIs:

- `bool CanUndo() const`
- `bool UndoHumanTurn()`

`CanUndo()` returns true only when the session is not searching, the current side to move is the human side, and `undo_stack_` is not empty.

`UndoHumanTurn()` restores the most recent snapshot, clears selection and hover state, resets search artifacts for the current generation, and leaves the session idle on the restored human turn. If called while undo is not allowed, it returns false and does not mutate the session.

`Reset()`, `LoadFromFile()`, and `SetHumanColor()` clear `undo_stack_` so history cannot cross game identity or side identity boundaries.

## GUI Behavior

Add an `Undo Move` button in the sidebar near `Load Position` and `Reset Position`.

- The button is disabled when `CanUndo()` is false.
- On success, `sidebar.status_message` becomes `Undid to previous human turn.`
- On failure, no board state changes.

The existing board rendering, last-move path rendering, and search telemetry should reflect the restored snapshot immediately after undo.

## Error Handling

Undo is deliberately conservative:

- While search is active, undo is disabled to avoid racing a background engine result.
- If the current turn is not the human turn, undo is disabled.
- If no snapshot exists, undo is disabled.
- Restoring a snapshot clears current selection, hover target, hover path, and pending search artifacts.

## Testing

Extend `surakarta-dev-session-selftest` with focused regression tests:

- Human black makes a move, engine replies, session returns to black, then `UndoHumanTurn()` restores the exact pre-human-move `Position`.
- Undo is not allowed while the engine is actively searching.
- `Reset()` clears undo history.
- Loading a position clears undo history.
- Changing the human side clears undo history.

Use existing deterministic low-depth search limits so the tests stay fast under the current CTest workflow.
