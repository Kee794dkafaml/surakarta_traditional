#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "surakarta_bitboard.h"
#include "surakarta_dev_session.h"
#include "surakarta_rule_manager_bitboard.h"

namespace {

using namespace std::chrono_literals;

using surakarta::bitboard::Color;
using surakarta::bitboard::DescribeLegalTargets;
using surakarta::bitboard::MakeSquare;
using surakarta::bitboard::PositionAdapter;
using surakarta::bitboard::SearchLimits;
using surakarta::bitboard::SearchSnapshot;
using surakarta::devtools::DevelopmentSession;

bool Expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << std::endl;
        return false;
    }
    return true;
}

bool TestSelectionMatchesBitboardTargets() {
    auto session = DevelopmentSession{};
    const auto from = MakeSquare(0, 1);
    const auto expected = DescribeLegalTargets(session.Position(), from);

    auto okay = true;
    okay &= Expect(session.SelectSquare(from), "expected to select a movable opening piece");
    okay &= Expect(session.LegalTargets().size() == expected.size(), "session legal target count mismatch");
    return okay;
}

bool TestHoverPathForCaptureTarget() {
    auto session = DevelopmentSession{};
    std::string error;
    auto okay = true;
    okay &= Expect(session.LoadFromFile(std::string(BITBOARD_TEST_DATA_DIR) + "\\game1.txt", &error), error);
    okay &= Expect(session.SelectSquare(MakeSquare(2, 2)), "expected to select capture source square");
    session.SetHoveredTarget(MakeSquare(2, 0));
    okay &= Expect(!session.HoverPath().empty(), "capture hover should reconstruct a visible path");
    return okay;
}

bool TestEngineRepliesAfterHumanMove() {
    auto session = DevelopmentSession{};
    auto limits = SearchLimits{};
    limits.max_depth = 1;
    limits.threads = 1;
    session.SetSearchLimits(limits);
    session.SetHumanColor(Color::Black);

    auto okay = true;
    okay &= Expect(session.SelectSquare(MakeSquare(0, 1)), "expected to select black piece");
    okay &= Expect(session.ApplyHumanMove(MakeSquare(0, 2)), "expected to apply opening move");
    okay &= Expect(session.SearchActive(), "engine search should start after human move");

    const auto human_ply = session.Position().ply;
    okay &= Expect(session.WaitForIdle(5s), "engine reply should finish within timeout");
    okay &= Expect(session.Position().ply == human_ply + 1, "engine should apply exactly one reply");
    okay &= Expect(session.Position().SideToMove() == Color::Black, "turn should return to human");
    return okay;
}

bool TestChangingHumanSideCancelsSearch() {
    auto session = DevelopmentSession{};
    auto limits = SearchLimits{};
    limits.max_depth = 6;
    limits.threads = 2;
    session.SetSearchLimits(limits);
    session.SetHumanColor(Color::White);

    auto okay = true;
    okay &= Expect(session.SearchActive(), "engine should think immediately when human plays white");
    session.SetHumanColor(Color::Black);
    session.Update();
    okay &= Expect(!session.SearchActive(), "switching human side should cancel outstanding search");
    okay &= Expect(session.Position().ply == 0, "canceled search must not leak a stale engine move");
    return okay;
}

bool SnapshotIsCleared(const SearchSnapshot& snapshot) {
    return !snapshot.active &&
           !snapshot.stop_requested &&
           !snapshot.best_move.IsValid() &&
           snapshot.score == 0 &&
           snapshot.depth == 0 &&
           snapshot.pv_length == 0 &&
           snapshot.nodes == 0 &&
           snapshot.qnodes == 0 &&
           snapshot.tt_hits == 0 &&
           snapshot.null_prunes == 0 &&
           snapshot.lmr_reductions == 0 &&
           snapshot.fail_highs == 0 &&
           snapshot.fail_lows == 0 &&
           snapshot.aspiration_retries == 0 &&
           snapshot.aspiration_attempts == 0 &&
           snapshot.full_window_researches == 0 &&
           snapshot.root_best_updates == 0 &&
           snapshot.tt_exact_cutoffs == 0 &&
           snapshot.tt_bound_cutoffs == 0 &&
           snapshot.beta_cutoffs == 0 &&
           snapshot.stale_root_tasks == 0 &&
           snapshot.aborted_root_tasks == 0 &&
           snapshot.worker_idle_spins == 0 &&
           snapshot.thread_spawn_count == 0 &&
           snapshot.nps == 0;
}

bool SnapshotHasProgress(const SearchSnapshot& snapshot) {
    return snapshot.nodes > 0 || snapshot.depth > 0 || snapshot.best_move.IsValid();
}

bool MovesEqual(const surakarta::bitboard::Move& lhs, const surakarta::bitboard::Move& rhs) {
    return lhs.from == rhs.from && lhs.to == rhs.to && lhs.flags == rhs.flags && lhs.aux == rhs.aux;
}

bool PositionsEqual(const surakarta::bitboard::Position& lhs, const surakarta::bitboard::Position& rhs) {
    return lhs.board.pieces[0] == rhs.board.pieces[0] &&
           lhs.board.pieces[1] == rhs.board.pieces[1] &&
           lhs.side_to_move == rhs.side_to_move &&
           lhs.ply == rhs.ply &&
           lhs.no_capture_ply == rhs.no_capture_ply &&
           lhs.max_no_capture_round == rhs.max_no_capture_round &&
           lhs.zobrist_key == rhs.zobrist_key &&
           lhs.eval_cache == rhs.eval_cache;
}

bool HasCurveFragment(const std::vector<SurakartaMovePathFragment>& path) {
    for (const auto& fragment : path) {
        if (fragment.is_curve) {
            return true;
        }
    }
    return false;
}

std::string ScreenshotSameFileCaptureGoldenFile() {
    return std::string(BITBOARD_TEST_DATA_DIR) + "\\screenshot_2026_05_06_same_file_capture.txt";
}

std::filesystem::path WriteTempBoardFile(const std::string& name, const std::string& contents) {
    const auto path = std::filesystem::temp_directory_path() / name;
    auto output = std::ofstream(path);
    output << contents;
    return path;
}

bool TestCoordinateConventionIsFileThenRank() {
    const auto square = MakeSquare(2, 3);
    const auto legacy = PositionAdapter::ToLegacyPosition(square);
    auto okay = true;
    okay &= Expect(legacy.x == 2 && legacy.y == 3,
                   "engine text coordinates should be printed as GUI x,y coordinates");
    okay &= Expect(PositionAdapter::FromLegacyPosition(SurakartaPosition(2, 3)) == square,
                   "GUI x,y coordinates should round-trip through the bitboard adapter");
    return okay;
}

bool TestScreenshotRejectedCandidatesAreNotSelectableCaptures() {
    auto session = DevelopmentSession{};
    std::string error;

    auto okay = true;
    okay &= Expect(session.LoadFromFile(ScreenshotSameFileCaptureGoldenFile(), &error), error);
    okay &= Expect(!session.SelectSquare(MakeSquare(2, 3)),
                   "screenshot engine-text source (2,3) should not be selectable because the GUI square is empty");
    okay &= Expect(!session.SelectSquare(MakeSquare(2, 4)),
                   "screenshot GUI-observed source (2,4) should not be selectable because the GUI square is empty");
    return okay;
}

bool TestAppliedCaptureKeepsVisibleLastMovePath() {
    auto session = DevelopmentSession{};
    std::string error;

    auto okay = true;
    okay &= Expect(session.LoadFromFile(std::string(BITBOARD_TEST_DATA_DIR) + "\\game9.txt", &error), error);
    okay &= Expect(session.SelectSquare(MakeSquare(3, 3)), "expected to select capture source square");
    okay &= Expect(session.ApplyHumanMove(MakeSquare(3, 1)), "expected to apply same-file capture");
    okay &= Expect(session.LastMove().has_value(), "applied capture should be recorded as last move");
    okay &= Expect(!session.LastMovePath().empty(), "applied capture should keep a visible path");
    okay &= Expect(HasCurveFragment(session.LastMovePath()),
                   "same-file capture path should show the loop instead of looking like a direct jump");
    return okay;
}

bool TestLiveSnapshotCarriesPrincipalVariationAndCounters() {
    auto session = DevelopmentSession{};
    auto limits = SearchLimits{};
    limits.max_depth = 8;
    limits.threads = 1;
    limits.aspiration_window = 1;
    session.SetSearchLimits(limits);
    session.SetHumanColor(Color::White);

    auto okay = true;
    okay &= Expect(session.SearchActive(), "engine should think immediately for live snapshot telemetry");

    auto saw_live_snapshot = false;
    auto saw_live_pv = false;
    const auto deadline = std::chrono::steady_clock::now() + 5s;
    while (std::chrono::steady_clock::now() < deadline) {
        session.Update();
        const auto snapshot = session.SearchSnapshot();
        if (snapshot.active && SnapshotHasProgress(snapshot)) {
            saw_live_snapshot = true;
            if (snapshot.best_move.IsValid()) {
                okay &= Expect(snapshot.pv_length > 0,
                               "live snapshot with best move should expose a non-empty principal variation");
                if (snapshot.pv_length > 0) {
                    okay &= Expect(MovesEqual(snapshot.pv[0], snapshot.best_move),
                                   "live snapshot pv head should match the current best move");
                    saw_live_pv = true;
                    break;
                }
            }
        }
        std::this_thread::sleep_for(1ms);
    }

    okay &= Expect(saw_live_snapshot, "session should publish a live search snapshot");
    okay &= Expect(saw_live_pv, "session should expose pv telemetry while search is active");
    okay &= Expect(session.WaitForIdle(5s), "live snapshot telemetry search should finish within timeout");

    const auto final_snapshot = session.SearchSnapshot();
    okay &= Expect(!final_snapshot.active, "final snapshot should go inactive after search completion");
    okay &= Expect(SnapshotHasProgress(final_snapshot),
                   "completed search telemetry should remain visible after the engine move is applied");
    okay &= Expect(final_snapshot.pv_length >= 0, "final snapshot pv length should stay in range");
    okay &= Expect(final_snapshot.fail_highs + final_snapshot.fail_lows == final_snapshot.aspiration_retries,
                   "final snapshot aspiration accounting should stay internally consistent");
    okay &= Expect(final_snapshot.aspiration_attempts >= final_snapshot.aspiration_retries,
                   "final snapshot aspiration attempts should dominate retries");
    okay &= Expect(final_snapshot.thread_spawn_count == 0,
                   "single-thread live telemetry search should not spawn worker threads");
    return okay;
}

bool TestSnapshotClearsAfterSearchCancellation() {
    auto session = DevelopmentSession{};
    auto limits = SearchLimits{};
    limits.max_depth = 8;
    limits.threads = 1;
    session.SetSearchLimits(limits);
    session.SetHumanColor(Color::White);

    auto okay = true;
    okay &= Expect(session.SearchActive(), "engine should start thinking when human side switches to white");

    auto saw_live_snapshot = false;
    auto completed_before_live_sample = false;
    auto last_observed_snapshot = SearchSnapshot{};
    const auto deadline = std::chrono::steady_clock::now() + 5s;
    while (std::chrono::steady_clock::now() < deadline) {
        session.Update();
        const auto snapshot = session.SearchSnapshot();
        last_observed_snapshot = snapshot;
        if (snapshot.active && SnapshotHasProgress(snapshot)) {
            saw_live_snapshot = true;
            break;
        }
        if (!session.SearchActive()) {
            completed_before_live_sample = true;
            break;
        }
        std::this_thread::sleep_for(1ms);
    }
    okay &= Expect(saw_live_snapshot || completed_before_live_sample,
                   "search should either publish a live snapshot or complete before sampling");

    if (completed_before_live_sample) {
        okay &= Expect(!session.SearchActive(), "naturally completed search should be idle before restart");
        okay &= Expect(!session.IsEngineTurn(), "naturally completed search should end on a human turn");
        okay &= Expect(session.LastMove().has_value(), "naturally completed search should leave behind an engine move");
        okay &= Expect(!last_observed_snapshot.active,
                       "completed snapshot sampled before cancellation should already be inactive");
        okay &= Expect(SnapshotHasProgress(last_observed_snapshot),
                       "completed snapshot sampled before cancellation should preserve telemetry");
        if (session.LastMove().has_value() && last_observed_snapshot.best_move.IsValid()) {
            okay &= Expect(MovesEqual(last_observed_snapshot.best_move, *session.LastMove()),
                           "completed snapshot best move should match the engine move that was applied");
        }

        const auto ply_before_restart = session.Position().ply;
        session.SetHumanColor(Color::Black);

        auto restart_observed = session.SearchActive();
        const auto restart_deadline = std::chrono::steady_clock::now() + 250ms;
        while (!restart_observed && std::chrono::steady_clock::now() < restart_deadline) {
            const auto restart_snapshot = session.SearchSnapshot();
            restart_observed = restart_snapshot.active || SnapshotHasProgress(restart_snapshot);
            if (!restart_observed) {
                std::this_thread::sleep_for(1ms);
            }
        }
        okay &= Expect(restart_observed, "switching to black should start a replacement search before cancellation");

        session.SetHumanColor(Color::White);
        session.Update();

        const auto cleared_snapshot = session.SearchSnapshot();
        okay &= Expect(!session.SearchActive(), "canceling the replacement search should leave the session idle");
        okay &= Expect(!session.IsEngineTurn(),
                       "canceling the replacement search should leave the session on a human turn");
        okay &= Expect(session.Position().ply == ply_before_restart,
                       "canceling the replacement search must not apply another engine move");
        okay &= Expect(SnapshotIsCleared(cleared_snapshot),
                       "canceling the replacement search should clear the restarted snapshot");
        return okay;
    }

    session.SetHumanColor(Color::Black);
    session.Update();

    const auto cleared_snapshot = session.SearchSnapshot();
    okay &= Expect(!session.SearchActive(), "canceling the live search should leave the session idle");
    okay &= Expect(!session.IsEngineTurn(), "canceling the live search should leave the session on a human turn");
    okay &= Expect(session.Position().ply == 0, "canceling the live search must not leak a stale engine move");
    okay &= Expect(SnapshotIsCleared(cleared_snapshot),
                   "canceling and switching back to human turn should clear stale search snapshot");
    return okay;
}

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

bool TestTerminalStatusUsesSharedRuleEvaluation() {
    auto session = DevelopmentSession{};
    std::string error;
    auto okay = true;
    okay &= Expect(session.LoadFromFile(std::string(BITBOARD_TEST_DATA_DIR) + "\\game6.txt", &error), error);
    session.SetHumanColor(Color::Black);
    okay &= Expect(session.SelectSquare(MakeSquare(5, 1)), "expected to select the forced terminal capture source");
    okay &= Expect(session.ApplyHumanMove(MakeSquare(1, 1)), "expected to apply the terminal capture move");

    const auto expected = EvaluateBitboardTerminal(session.Position());
    const auto status = session.Status();
    okay &= Expect(status.terminal, "terminal capture should mark the session as terminal");
    okay &= Expect(status.end_reason == expected.first, "session end reason should match shared terminal evaluation");
    okay &= Expect(status.winner == expected.second, "session winner should match shared terminal evaluation");
    return okay;
}

bool TestTerminalStatusUsesNationalNoCaptureSemantics() {
    const auto stalemate_path = WriteTempBoardFile(
        "surakarta-national-stalemate-status.txt",
        "B B W . . . \n"
        ". . . . . . \n"
        ". . . . . . \n"
        ". . . . . . \n"
        ". . . . . . \n"
        ". . . . . . \n"
        "current_player: B\n"
        "num_round: 1\n"
        "last_captured_round: 0\n"
        "end_reason: NONE\n"
        "winner: .\n"
        "max_no_capture_round: 40\n");
    const auto legacy_counter_path = WriteTempBoardFile(
        "surakarta-national-counter-not-terminal.txt",
        ". . . . . . \n"
        "B . . . . . \n"
        ". . . . . . \n"
        ". . . . . . \n"
        "W . . . . . \n"
        ". . . . . . \n"
        "current_player: B\n"
        "num_round: 50\n"
        "last_captured_round: 0\n"
        "end_reason: NONE\n"
        "winner: .\n"
        "max_no_capture_round: 1\n");

    auto okay = true;
    {
        auto session = DevelopmentSession{};
        std::string error;
        okay &= Expect(session.LoadFromFile(stalemate_path.string(), &error), error);
        const auto status = session.Status();
        okay &= Expect(status.terminal, "national no-capture stalemate should mark the session as terminal");
        okay &= Expect(status.end_reason == SurakartaEndReason::STALEMATE,
                       "national no-capture stalemate should report STALEMATE");
        okay &= Expect(status.winner == SurakartaPlayer::BLACK,
                       "national no-capture stalemate should award the material leader");
    }
    {
        auto session = DevelopmentSession{};
        std::string error;
        okay &= Expect(session.LoadFromFile(legacy_counter_path.string(), &error), error);
        const auto status = session.Status();
        okay &= Expect(!status.terminal,
                       "legacy no-capture counter alone should not mark a capture-available position terminal");
        okay &= Expect(status.end_reason == SurakartaEndReason::NONE,
                       "capture-available legacy no-capture fixture should keep NONE end reason");
        okay &= Expect(status.winner == SurakartaPlayer::NONE,
                       "capture-available legacy no-capture fixture should keep NONE winner");
    }

    std::error_code ignored;
    std::filesystem::remove(stalemate_path, ignored);
    std::filesystem::remove(legacy_counter_path, ignored);
    return okay;
}

}  // namespace

int main() {
    bool okay = true;
    okay &= TestCoordinateConventionIsFileThenRank();
    okay &= TestSelectionMatchesBitboardTargets();
    okay &= TestHoverPathForCaptureTarget();
    okay &= TestScreenshotRejectedCandidatesAreNotSelectableCaptures();
    okay &= TestEngineRepliesAfterHumanMove();
    okay &= TestUndoRestoresPreviousHumanTurnAfterEngineReply();
    okay &= TestUndoIsBlockedDuringEngineSearch();
    okay &= TestUndoHistoryClearsAcrossSessionIdentityChanges();
    okay &= TestChangingHumanSideCancelsSearch();
    okay &= TestAppliedCaptureKeepsVisibleLastMovePath();
    okay &= TestLiveSnapshotCarriesPrincipalVariationAndCounters();
    okay &= TestSnapshotClearsAfterSearchCancellation();
    okay &= TestTerminalStatusUsesSharedRuleEvaluation();
    okay &= TestTerminalStatusUsesNationalNoCaptureSemantics();
    if (okay) {
        std::cout << "[PASS] surakarta-dev-session-selftest" << std::endl;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
