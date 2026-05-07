# Human-Turn Undo Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a GUI undo button that returns a human-vs-engine game to the previous human-turn position.

**Architecture:** Store human-turn snapshots inside `surakarta::devtools::DevelopmentSession`. Push one snapshot before each successful human move, restore the last snapshot through a conservative `UndoHumanTurn()` API, and expose the API to the ImGui sidebar as a disabled-when-unavailable button.

**Tech Stack:** C++20, CMake, Dear ImGui, existing `surakarta-dev-session-selftest` binary.

---

### Task 1: Add Failing DevSession Undo Tests

**Files:**
- Modify: `src/surakarta_dev_session_selftest.cpp`

- [ ] **Step 1: Add a position equality helper and undo tests**

Add this helper after `MovesEqual`:

```cpp
bool PositionsEqual(const surakarta::bitboard::Position& lhs, const surakarta::bitboard::Position& rhs) {
    return lhs.board.pieces == rhs.board.pieces &&
           lhs.side_to_move == rhs.side_to_move &&
           lhs.ply == rhs.ply &&
           lhs.quiet_plies == rhs.quiet_plies &&
           lhs.zobrist_key == rhs.zobrist_key;
}
```

Add these test functions before `TestTerminalStatusUsesSharedRuleEvaluation`:

```cpp
bool TestUndoRestoresPreviousHumanTurnAfterEngineReply() {
    auto session = DevelopmentSession{};
    auto limits = SearchLimits{};
    limits.max_depth = 1;
    limits.threads = 1;
    session.SetSearchLimits(limits);
    session.SetHumanColor(Color::Black);

    const auto before_human_move = session.Position();

    auto okay = true;
    okay &= Expect(session.SelectSquare(MakeSquare(0, 1)), "expected to select black piece before undo test");
    okay &= Expect(session.ApplyHumanMove(MakeSquare(0, 2)), "expected to apply opening move before undo test");
    okay &= Expect(session.WaitForIdle(5s), "engine reply should finish before undo");
    okay &= Expect(session.Position().SideToMove() == Color::Black, "undo test should return to human turn");
    okay &= Expect(session.CanUndo(), "undo should be available on the next human turn");
    okay &= Expect(session.UndoHumanTurn(), "undo should restore the previous human turn");
    okay &= Expect(PositionsEqual(session.Position(), before_human_move),
                   "undo should restore the exact pre-human-move position");
    okay &= Expect(session.Position().SideToMove() == Color::Black, "undo should leave black to move");
    okay &= Expect(!session.SearchActive(), "undo should leave the engine idle");
    okay &= Expect(!session.CanUndo(), "single undo should consume the only snapshot");
    return okay;
}

bool TestUndoIsBlockedDuringEngineSearch() {
    auto session = DevelopmentSession{};
    auto limits = SearchLimits{};
    limits.max_depth = 8;
    limits.threads = 1;
    session.SetSearchLimits(limits);
    session.SetHumanColor(Color::Black);

    auto okay = true;
    okay &= Expect(session.SelectSquare(MakeSquare(0, 1)), "expected to select black piece before active-search undo test");
    okay &= Expect(session.ApplyHumanMove(MakeSquare(0, 2)), "expected to start engine search before active-search undo test");
    okay &= Expect(session.SearchActive(), "engine should be active before active-search undo test");
    okay &= Expect(!session.CanUndo(), "undo should be disabled while engine is searching");
    okay &= Expect(!session.UndoHumanTurn(), "undo call should fail while engine is searching");
    okay &= Expect(session.Position().SideToMove() == Color::White,
                   "failed undo during search should not change side to move");
    okay &= Expect(session.WaitForIdle(5s), "engine search should still finish after blocked undo");
    return okay;
}

bool TestUndoHistoryClearsAcrossSessionIdentityChanges() {
    auto session = DevelopmentSession{};
    auto limits = SearchLimits{};
    limits.max_depth = 1;
    limits.threads = 1;
    session.SetSearchLimits(limits);
    session.SetHumanColor(Color::Black);

    auto okay = true;
    okay &= Expect(session.SelectSquare(MakeSquare(0, 1)), "expected to select black piece before reset undo clear test");
    okay &= Expect(session.ApplyHumanMove(MakeSquare(0, 2)), "expected to apply opening move before reset undo clear test");
    okay &= Expect(session.WaitForIdle(5s), "engine reply should finish before reset undo clear test");
    okay &= Expect(session.CanUndo(), "undo should be available before reset");
    session.Reset();
    okay &= Expect(!session.CanUndo(), "reset should clear undo history");

    okay &= Expect(session.SelectSquare(MakeSquare(0, 1)), "expected to select black piece before load undo clear test");
    okay &= Expect(session.ApplyHumanMove(MakeSquare(0, 2)), "expected to apply opening move before load undo clear test");
    okay &= Expect(session.WaitForIdle(5s), "engine reply should finish before load undo clear test");
    okay &= Expect(session.CanUndo(), "undo should be available before load");
    std::string error;
    okay &= Expect(session.LoadFromFile(std::string(BITBOARD_TEST_DATA_DIR) + "\\game1.txt", &error), error);
    okay &= Expect(!session.CanUndo(), "loading a position should clear undo history");

    session.Reset();
    okay &= Expect(session.SelectSquare(MakeSquare(0, 1)), "expected to select black piece before side undo clear test");
    okay &= Expect(session.ApplyHumanMove(MakeSquare(0, 2)), "expected to apply opening move before side undo clear test");
    okay &= Expect(session.WaitForIdle(5s), "engine reply should finish before side undo clear test");
    okay &= Expect(session.CanUndo(), "undo should be available before human side change");
    session.SetHumanColor(Color::White);
    okay &= Expect(!session.CanUndo(), "changing human side should clear undo history");
    return okay;
}
```

Add the tests to `main()` after `TestEngineRepliesAfterHumanMove()`:

```cpp
    okay &= TestUndoRestoresPreviousHumanTurnAfterEngineReply();
    okay &= TestUndoIsBlockedDuringEngineSearch();
    okay &= TestUndoHistoryClearsAcrossSessionIdentityChanges();
```

- [ ] **Step 2: Run the focused selftest and confirm RED**

Run:

```powershell
cmake --build build --target surakarta-dev-session-selftest --config Debug
.\build\bin\Debug\surakarta-dev-session-selftest.exe
```

Expected: compile fails because `DevelopmentSession` has no `CanUndo()` or `UndoHumanTurn()` members.

### Task 2: Implement DevSession Human-Turn Snapshot Undo

**Files:**
- Modify: `include/surakarta_dev_session.h`
- Modify: `src/surakarta_dev_session.cpp`
- Test: `src/surakarta_dev_session_selftest.cpp`

- [ ] **Step 1: Add public API and private snapshot storage**

In `include/surakarta_dev_session.h`, add public methods after `ClearSelection()`:

```cpp
    bool CanUndo() const;
    bool UndoHumanTurn();
```

Add private snapshot helpers before `CompletedSearch`:

```cpp
    struct HumanTurnSnapshot {
        bitboard::Position position{};
        SessionStatus status{};
        std::optional<bitboard::Move> last_move;
        std::vector<SurakartaMovePathFragment> last_move_path;
    };
```

Add private methods near `ApplyMove`:

```cpp
    void PushHumanTurnSnapshot();
    void ClearUndoHistory();
```

Add storage near `last_move_path_`:

```cpp
    std::vector<HumanTurnSnapshot> undo_stack_;
```

- [ ] **Step 2: Implement minimal undo behavior**

In `src/surakarta_dev_session.cpp`, clear history at identity boundaries:

```cpp
void DevelopmentSession::SetHumanColor(bitboard::Color color) {
    if (human_color_ == color) {
        return;
    }
    CancelSearch();
    human_color_ = color;
    ClearUndoHistory();
    RebuildSelection();
    RebuildHoverPath();
    StartEngineSearchIfNeeded();
}
```

In `LoadPosition`, after `loaded_file_ = file_name;`:

```cpp
    ClearUndoHistory();
```

Add methods after `ClearSelection()`:

```cpp
bool DevelopmentSession::CanUndo() const {
    return !SearchActive() && position_.SideToMove() == human_color_ && !undo_stack_.empty();
}

bool DevelopmentSession::UndoHumanTurn() {
    Update();
    if (!CanUndo()) {
        return false;
    }

    const auto generation = AdvanceGeneration();
    const auto snapshot = undo_stack_.back();
    undo_stack_.pop_back();
    position_ = snapshot.position;
    status_ = snapshot.status;
    last_move_ = snapshot.last_move;
    last_move_path_ = snapshot.last_move_path;
    ClearSelection();
    ResetSearchArtifacts(generation);
    RefreshDerivedState();
    return true;
}
```

Add helpers near `ApplyMove`:

```cpp
void DevelopmentSession::PushHumanTurnSnapshot() {
    undo_stack_.push_back(HumanTurnSnapshot{position_, status_, last_move_, last_move_path_});
}

void DevelopmentSession::ClearUndoHistory() {
    undo_stack_.clear();
}
```

In `ApplyHumanMove()`, before `ApplyMove(move);`:

```cpp
    PushHumanTurnSnapshot();
```

- [ ] **Step 3: Run the focused selftest and confirm GREEN**

Run:

```powershell
cmake --build build --target surakarta-dev-session-selftest --config Debug
.\build\bin\Debug\surakarta-dev-session-selftest.exe
```

Expected: `[PASS] surakarta-dev-session-selftest`.

### Task 3: Add GUI Undo Button

**Files:**
- Modify: `src/surakarta_gui_main.cpp`

- [ ] **Step 1: Add disabled `Undo Move` button near load/reset controls**

In `RenderSidebar`, after the `Reset Position` button block:

```cpp
    ImGui::SameLine();
    const bool can_undo = session.CanUndo();
    if (!can_undo) {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button("Undo Move")) {
        if (session.UndoHumanTurn()) {
            sidebar.status_message = "Undid to previous human turn.";
        }
    }
    if (!can_undo) {
        ImGui::EndDisabled();
    }
```

- [ ] **Step 2: Build GUI smoke target**

Run:

```powershell
cmake --build build --target surakarta-gui --config Debug
.\build\bin\Debug\surakarta-gui.exe --smoke-test-sidebar-labels
```

Expected: build succeeds and smoke prints `[PASS] surakarta-gui sidebar label smoke`.

### Task 4: Final Verification

**Files:**
- Verify only.

- [ ] **Step 1: Run focused tests**

Run:

```powershell
.\build\bin\Debug\surakarta-dev-session-selftest.exe
.\build\bin\Debug\surakarta-gui.exe --smoke-test-sidebar-labels
```

Expected: both pass.

- [ ] **Step 2: Check diff scope**

Run:

```powershell
git diff -- include/surakarta_dev_session.h src/surakarta_dev_session.cpp src/surakarta_dev_session_selftest.cpp src/surakarta_gui_main.cpp docs/superpowers/plans/2026-05-06-human-turn-undo.md
```

Expected: diff only covers undo tests, undo session API/storage, GUI button, and this plan.
