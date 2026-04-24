#include <algorithm>
#include <atomic>
#include <bit>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "surakarta_bitboard.h"
#include "surakarta_game.h"
#include "surakarta_rule_manager_impl.h"
#include "surakarta_rule_manager_bitboard.h"
#include "surakarta_search.h"
#include "surakarta_utils.h"

namespace {

using surakarta::bitboard::Bit;
using surakarta::bitboard::DescribeLegalTargets;
using surakarta::bitboard::MakeSquare;
using surakarta::bitboard::Move;
using surakarta::bitboard::NTupleWeights;
using surakarta::bitboard::PositionBuilder;
using surakarta::bitboard::PositionAdapter;
using surakarta::bitboard::ReconstructCapturePath;
using surakarta::bitboard::EvalOptions;
using surakarta::bitboard::TrainingOptions;
using surakarta::bitboard::TrainingSummary;
using surakarta::bitboard::SearchController;
using surakarta::bitboard::SearchLimits;
using surakarta::bitboard::Undo;

bool EqualFragment(const SurakartaMovePathFragment& lhs, const SurakartaMovePathFragment& rhs) {
    if (lhs.is_curve != rhs.is_curve) {
        return false;
    }
    if (!lhs.is_curve) {
        return lhs.info.straight.start_x == rhs.info.straight.start_x &&
               lhs.info.straight.start_y == rhs.info.straight.start_y &&
               lhs.info.straight.end_x == rhs.info.straight.end_x &&
               lhs.info.straight.end_y == rhs.info.straight.end_y;
    }
    return lhs.info.curve.center_x == rhs.info.curve.center_x &&
           lhs.info.curve.center_y == rhs.info.curve.center_y &&
           lhs.info.curve.radius == rhs.info.curve.radius &&
           lhs.info.curve.start_angle == rhs.info.curve.start_angle &&
           lhs.info.curve.end_angle == rhs.info.curve.end_angle &&
           lhs.info.curve.is_clockwise == rhs.info.curve.is_clockwise;
}

bool Expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << std::endl;
        return false;
    }
    return true;
}

bool EqualMove(Move lhs, Move rhs) {
    return lhs.from == rhs.from &&
           lhs.to == rhs.to &&
           lhs.flags == rhs.flags &&
           lhs.aux == rhs.aux;
}

std::string FormatMove(Move move) {
    if (!move.IsValid()) {
        return "n/a";
    }
    const auto from = PositionAdapter::ToLegacyPosition(move.from);
    const auto to = PositionAdapter::ToLegacyPosition(move.to);
    return "(" + std::to_string(from.x) + ", " + std::to_string(from.y) + ") -> (" +
           std::to_string(to.x) + ", " + std::to_string(to.y) + ")";
}

bool IsMovePreferredForTest(Move candidate, Move current) {
    if (!candidate.IsValid()) {
        return false;
    }
    if (!current.IsValid()) {
        return true;
    }
    if (candidate.from != current.from) {
        return candidate.from < current.from;
    }
    if (candidate.to != current.to) {
        return candidate.to < current.to;
    }
    if (candidate.flags != current.flags) {
        return candidate.flags < current.flags;
    }
    return candidate.aux < current.aux;
}

std::string QuoteCommandArg(const std::string& value) {
    auto quoted = std::string{"\""};
    for (const auto ch : value) {
        if (ch == '"') {
            quoted += "\\\"";
        } else {
            quoted += ch;
        }
    }
    quoted += "\"";
    return quoted;
}

std::filesystem::path CurrentExecutablePath() {
#ifdef _WIN32
    std::array<char, MAX_PATH> buffer{};
    const auto size = GetModuleFileNameA(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
    return std::filesystem::path(std::string(buffer.data(), size));
#else
    return std::filesystem::read_symlink("/proc/self/exe");
#endif
}

std::filesystem::path BenchmarkExecutablePath() {
    auto executable = CurrentExecutablePath();
#ifdef _WIN32
    return executable.parent_path() / "surakarta-benchmark.exe";
#else
    return executable.parent_path() / "surakarta-benchmark";
#endif
}

int RunCommandCapture(const std::string& command, std::string* output) {
#ifdef _WIN32
    const auto shell_command = "cmd /d /s /c \"" + command + " 2>&1\"";
    auto* pipe = _popen(shell_command.c_str(), "r");
#else
    auto* pipe = popen((command + " 2>&1").c_str(), "r");
#endif
    if (pipe == nullptr) {
        if (output != nullptr) {
            *output = "failed to start process";
        }
        return -1;
    }

    auto captured = std::string{};
    std::array<char, 512> buffer{};
    while (std::fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        captured += buffer.data();
    }

#ifdef _WIN32
    const auto exit_code = _pclose(pipe);
#else
    const auto exit_code = pclose(pipe);
#endif

    if (output != nullptr) {
        *output = captured;
    }
    return exit_code;
}

bool JsonHasField(const std::string& text, const std::string& field) {
    return text.find("\"" + field + "\":") != std::string::npos;
}

bool ExpectJsonReportShape(const std::string& text, const std::string& case_id, const std::string& label) {
    auto okay = true;
    okay &= Expect(!text.empty(), label + " should emit non-empty json output");
    okay &= Expect(text.front() == '{', label + " should start with a JSON object");
    okay &= Expect(text.find("\"case_id\":\"" + case_id + "\"") != std::string::npos,
                   label + " should include the explicit case id");

    const std::array<std::string, 23> fields{
        "depth",
        "threads",
        "search_seconds",
        "nodes",
        "nps",
        "tt_hits",
        "null_prunes",
        "lmr_reductions",
        "fail_highs",
        "fail_lows",
        "aspiration_retries",
        "aspiration_attempts",
        "full_window_researches",
        "root_best_updates",
        "tt_exact_cutoffs",
        "tt_bound_cutoffs",
        "beta_cutoffs",
        "stale_root_tasks",
        "aborted_root_tasks",
        "worker_idle_spins",
        "thread_spawn_count",
        "pv",
        "best_move",
    };
    for (const auto& field : fields) {
        okay &= Expect(JsonHasField(text, field), label + " missing json field: " + field);
    }
    okay &= Expect(JsonHasField(text, "score"), label + " missing json field: score");
    return okay;
}

bool ExpectEvalJsonReportShape(const std::string& text, const std::string& label) {
    auto okay = true;
    okay &= Expect(!text.empty(), label + " should emit non-empty json output");
    okay &= Expect(text.front() == '{', label + " should start with a JSON object");
    for (const auto& field : std::array<std::string, 6>{
             "candidate_weights",
             "baseline_weights",
             "depth",
             "position_reports",
             "match_summary",
             "match_games",
         }) {
        okay &= Expect(JsonHasField(text, field), label + " missing json field: " + field);
    }
    okay &= Expect(text.find("\"case_id\":\"opening\"") != std::string::npos,
                   label + " should include opening report");
    okay &= Expect(text.find("\"case_id\":\"middlegame\"") != std::string::npos,
                   label + " should include middlegame report");
    okay &= Expect(text.find("\"case_id\":\"endgame\"") != std::string::npos,
                   label + " should include endgame report");
    return okay;
}

struct TempDirGuard {
    explicit TempDirGuard(const std::string& prefix)
        : path(std::filesystem::temp_directory_path() /
               (prefix + "-" + std::to_string(GetCurrentProcessId()) + "-" +
                std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()))) {
        std::filesystem::create_directories(path);
    }

    ~TempDirGuard() {
        std::error_code ignored;
        std::filesystem::remove_all(path, ignored);
    }

    std::filesystem::path path;
};

std::string ReadFileBinary(const std::filesystem::path& file_name) {
    auto input = std::ifstream(file_name, std::ios::binary);
    auto stream = std::ostringstream{};
    stream << input.rdbuf();
    return stream.str();
}

surakarta::bitboard::Position LoadPositionFromFile(const std::string& file_name) {
    SurakartaGame game;
    if (file_name.empty()) {
        game.StartGame();
    } else {
        game.StartGame(file_name);
    }
    return PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());
}

int ReferenceAlphaBetaForRoot(const surakarta::bitboard::NTupleEvaluator& evaluator,
                              surakarta::bitboard::Position& position,
                              int depth,
                              int alpha,
                              int beta) {
    const auto side = position.SideToMove();
    const auto enemy = side == surakarta::bitboard::Color::Black
                           ? surakarta::bitboard::Color::White
                           : surakarta::bitboard::Color::Black;
    auto terminal = (std::numeric_limits<int>::min)();
    if (position.board.Count(side) == 0) {
        terminal = -200'000 + position.ply;
    } else if (position.board.Count(enemy) == 0) {
        terminal = 200'000 - position.ply;
    } else if (position.no_capture_ply >= position.max_no_capture_round) {
        const int score = surakarta::bitboard::MaterialBalance(position);
        terminal = side == surakarta::bitboard::Color::Black ? score : -score;
    }
    if (terminal != (std::numeric_limits<int>::min)()) {
        return terminal;
    }
    if (depth <= 0) {
        return evaluator.Evaluate(position);
    }

    auto move_list = surakarta::bitboard::MoveList{};
    surakarta::bitboard::GenerateMoves(position, move_list);
    if (move_list.size == 0) {
        return evaluator.Evaluate(position);
    }

    auto best = (std::numeric_limits<int>::min)();
    for (int i = 0; i < move_list.size; ++i) {
        Undo undo;
        surakarta::bitboard::MakeMove(position, move_list.moves[i], undo);
        const int score = -ReferenceAlphaBetaForRoot(evaluator, position, depth - 1, -beta, -alpha);
        surakarta::bitboard::UnmakeMove(position, move_list.moves[i], undo);
        best = std::max(best, score);
        alpha = std::max(alpha, score);
        if (alpha >= beta) {
            break;
        }
    }
    return best;
}

surakarta::bitboard::SearchResult ReferenceRootBestMove(const surakarta::bitboard::Position& root,
                                                        const surakarta::bitboard::NTupleEvaluator& evaluator,
                                                        int depth) {
    auto result = surakarta::bitboard::SearchResult{};
    result.depth = depth;
    result.score = (std::numeric_limits<int>::min)();

    auto move_list = surakarta::bitboard::MoveList{};
    surakarta::bitboard::GenerateMoves(root, move_list);
    for (int i = 0; i < move_list.size; ++i) {
        auto working = root;
        Undo undo;
        const auto move = move_list.moves[i];
        surakarta::bitboard::MakeMove(working, move, undo);
        const int score = -ReferenceAlphaBetaForRoot(evaluator,
                                                     working,
                                                     depth - 1,
                                                     (std::numeric_limits<int>::min)() / 2,
                                                     (std::numeric_limits<int>::max)() / 2);
        surakarta::bitboard::UnmakeMove(working, move, undo);

        if (score > result.score || (score == result.score && IsMovePreferredForTest(move, result.best_move))) {
            result.best_move = move;
            result.score = score;
            result.pv[0] = move;
            result.pv_length = 1;
        }
    }

    return result;
}

surakarta::bitboard::Position ApplyLegacyMoveToPosition(const surakarta::bitboard::Position& root,
                                                        const SurakartaMove& legacy_move) {
    auto position = root;
    const auto move = PositionAdapter::FromLegacyMove(legacy_move, position);
    auto undo = Undo{};
    MakeMove(position, move, undo);
    return position;
}

bool ValidatePrincipalVariation(const surakarta::bitboard::Position& root,
                                const surakarta::bitboard::SearchResult& result,
                                std::string* reason) {
    if (result.pv_length < 0 || result.pv_length > surakarta::bitboard::kMaxPly) {
        if (reason != nullptr) {
            *reason = "pv_length out of range: " + std::to_string(result.pv_length);
        }
        return false;
    }

    if (result.pv_length > 0) {
        const auto& pv_head = result.pv[0];
        if (pv_head.from != result.best_move.from ||
            pv_head.to != result.best_move.to ||
            pv_head.flags != result.best_move.flags) {
            if (reason != nullptr) {
                *reason = "best_move does not match pv head";
            }
            return false;
        }
    }

    auto position = root;
    for (int ply = 0; ply < result.pv_length; ++ply) {
        auto legal_moves = surakarta::bitboard::MoveList{};
        GenerateMoves(position, legal_moves);

        bool found = false;
        auto chosen_move = Move{};
        for (int i = 0; i < legal_moves.size; ++i) {
            const auto candidate = legal_moves.moves[i];
            if (candidate.from == result.pv[ply].from &&
                candidate.to == result.pv[ply].to &&
                candidate.flags == result.pv[ply].flags) {
                chosen_move = candidate;
                found = true;
                break;
            }
        }

        if (!found) {
            if (reason != nullptr) {
                *reason = "illegal pv move at ply " + std::to_string(ply);
            }
            return false;
        }

        Undo undo;
        MakeMove(position, chosen_move, undo);
    }

    return true;
}

bool TestQuietMask() {
    const auto& tables = surakarta::bitboard::GetBitboardTables();
    const auto expected = Bit(MakeSquare(1, 0)) | Bit(MakeSquare(0, 1)) | Bit(MakeSquare(1, 1));
    return Expect(tables.quiet_move_mask[MakeSquare(0, 0)] == expected, "quiet mask for corner square is wrong");
}

bool TestRuleParity() {
    auto all_ok = true;

    auto compare_targets = [&](const std::string& file_name) {
        SurakartaGame legacy_game;
        legacy_game.StartGame(file_name);
        SurakartaGame bitboard_game;
        bitboard_game.StartGame(file_name);
        auto legacy_rule = legacy_game.GetRuleManager();
        auto bitboard_rule = std::make_shared<SurakartaRuleManagerBitboard>(bitboard_game.GetBoard(), bitboard_game.GetGameInfo());
        for (int x = 0; x < BOARD_SIZE; ++x) {
            for (int y = 0; y < BOARD_SIZE; ++y) {
                const auto position = SurakartaPosition(x, y);
                const auto legacy_targets = legacy_rule->GetAllLegalTarget(position);
                const auto bitboard_targets = bitboard_rule->GetAllLegalTarget(position);
                std::sort(legacy_targets->begin(), legacy_targets->end(), [](const auto& lhs, const auto& rhs) {
                    return lhs.x == rhs.x ? lhs.y < rhs.y : lhs.x < rhs.x;
                });
                std::sort(bitboard_targets->begin(), bitboard_targets->end(), [](const auto& lhs, const auto& rhs) {
                    return lhs.x == rhs.x ? lhs.y < rhs.y : lhs.x < rhs.x;
                });
                all_ok &= Expect(*legacy_targets == *bitboard_targets, "legal targets mismatch on " + file_name);
            }
        }
    };

    const std::vector<std::pair<std::string, std::vector<SurakartaMove>>> move_cases{
        {std::string(BITBOARD_TEST_DATA_DIR) + "\\game1.txt", {
                                                          {2, 2, 2, 0, SurakartaPlayer::BLACK},
                                                          {2, 2, 2, 1, SurakartaPlayer::BLACK},
                                                          {2, 2, 2, 3, SurakartaPlayer::BLACK},
                                                          {4, 4, 5, 1, SurakartaPlayer::BLACK},
                                                          {4, 5, 5, 1, SurakartaPlayer::BLACK},
                                                          {4, 5, 4, 6, SurakartaPlayer::BLACK},
                                                          {0, 0, 0, 1, SurakartaPlayer::BLACK},
                                                          {2, 0, 1, 0, SurakartaPlayer::BLACK},
                                                          {2, 0, 1, 0, SurakartaPlayer::WHITE},
                                                      }},
        {std::string(BITBOARD_TEST_DATA_DIR) + "\\game2.txt", {
                                                          {4, 4, 3, 4, SurakartaPlayer::WHITE},
                                                          {3, 5, 5, 3, SurakartaPlayer::WHITE},
                                                      }},
        {std::string(BITBOARD_TEST_DATA_DIR) + "\\game8.txt", {
                                                          {1, 3, 1, 2, SurakartaPlayer::WHITE},
                                                          {1, 2, 1, 3, SurakartaPlayer::WHITE},
                                                      }},
        {std::string(BITBOARD_TEST_DATA_DIR) + "\\game9.txt", {
                                                          {3, 3, 2, 3, SurakartaPlayer::BLACK},
                                                          {3, 3, 3, 1, SurakartaPlayer::BLACK},
                                                      }},
    };

    for (const auto& [file_name, moves] : move_cases) {
        SurakartaGame legacy_game;
        legacy_game.StartGame(file_name);
        SurakartaGame bitboard_game;
        bitboard_game.StartGame(file_name);
        auto legacy_rule = legacy_game.GetRuleManager();
        auto bitboard_rule = std::make_shared<SurakartaRuleManagerBitboard>(bitboard_game.GetBoard(), bitboard_game.GetGameInfo());
        for (const auto& move : moves) {
            const auto legacy_reason = legacy_rule->JudgeMove(move);
            const auto bitboard_reason = bitboard_rule->JudgeMove(move);
            all_ok &= Expect(legacy_reason == bitboard_reason, "move reason mismatch on " + file_name);
        }
        compare_targets(file_name);
    }

    const std::vector<std::pair<std::string, SurakartaIllegalMoveReason>> end_cases{
        {std::string(BITBOARD_TEST_DATA_DIR) + "\\game6.txt", SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE},
        {std::string(BITBOARD_TEST_DATA_DIR) + "\\game3.txt", SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE},
        {std::string(BITBOARD_TEST_DATA_DIR) + "\\game4.txt", SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE},
        {std::string(BITBOARD_TEST_DATA_DIR) + "\\game5.txt", SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE},
        {std::string(BITBOARD_TEST_DATA_DIR) + "\\game7.txt", SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE},
    };

    for (const auto& [file_name, reason] : end_cases) {
        SurakartaGame legacy_game;
        legacy_game.StartGame(file_name);
        SurakartaGame bitboard_game;
        bitboard_game.StartGame(file_name);
        auto legacy_rule = legacy_game.GetRuleManager();
        auto bitboard_rule = std::make_shared<SurakartaRuleManagerBitboard>(bitboard_game.GetBoard(), bitboard_game.GetGameInfo());
        const auto legacy_end = legacy_rule->JudgeEnd(reason);
        const auto bitboard_end = bitboard_rule->JudgeEnd(reason);
        all_ok &= Expect(legacy_end == bitboard_end, "end reason mismatch on " + file_name);
    }

    return all_ok;
}

bool TestEvaluateBitboardTerminalParity() {
    auto all_ok = true;

    const auto initial = LoadPositionFromFile("");
    all_ok &= Expect(EvaluateBitboardTerminal(initial) ==
                         std::pair(SurakartaEndReason::NONE, SurakartaPlayer::NONE),
                     "initial position should not be terminal");

    const auto pre_capture = LoadPositionFromFile(std::string(BITBOARD_TEST_DATA_DIR) + "\\game6.txt");
    all_ok &= Expect(EvaluateBitboardTerminal(pre_capture) ==
                         std::pair(SurakartaEndReason::NONE, SurakartaPlayer::NONE),
                     "pre-capture threshold position should not be terminal yet");

    const auto post_capture = ApplyLegacyMoveToPosition(pre_capture, SurakartaMove(5, 1, 1, 1, SurakartaPlayer::BLACK));
    all_ok &= Expect(EvaluateBitboardTerminal(post_capture) ==
                         std::pair(SurakartaEndReason::CHECKMATE, SurakartaPlayer::BLACK),
                     "capturing the last enemy piece should be terminal");

    auto forced_stalemate = LoadPositionFromFile(std::string(BITBOARD_TEST_DATA_DIR) + "\\game7.txt");
    forced_stalemate.no_capture_ply = static_cast<std::uint16_t>(forced_stalemate.max_no_capture_round + 1);
    all_ok &= Expect(EvaluateBitboardTerminal(forced_stalemate) ==
                         std::pair(SurakartaEndReason::STALEMATE, SurakartaPlayer::BLACK),
                     "exceeding the no-capture limit should declare stalemate for the material leader");

    return all_ok;
}

bool TestSearchParity() {
    SurakartaGame game;
    game.StartGame();
    const auto position = PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());

    SearchController controller;
    SearchLimits limits;
    limits.max_depth = 3;
    const auto pvs_result = controller.Search(position, limits);
    const auto reference = controller.SearchReference(position, 3);
    auto okay = true;
    okay &= Expect(pvs_result.best_move.IsValid(), "PVS did not return a legal best move");
    okay &= Expect(pvs_result.score == reference.score, "PVS score mismatches reference alpha-beta");
    return okay;
}

bool TestSharedNodeBudget() {
    SurakartaGame game;
    game.StartGame();
    const auto position = PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());

    SearchController controller;
    SearchLimits limits;
    limits.max_depth = 5;
    limits.threads = 4;
    limits.node_limit = 1;

    const auto result = controller.Search(position, limits);
    auto okay = true;
    okay &= Expect(result.best_move.IsValid(), "multi-thread search did not return a legal best move");
    okay &= Expect(result.stats.nodes <= limits.node_limit,
                   "node budget should be shared across all workers");
    return okay;
}

bool TestSharedStopRequest() {
    using namespace std::chrono_literals;

    SurakartaGame game;
    game.StartGame();
    const auto position = PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());

    SearchController controller;
    SearchLimits limits;
    limits.max_depth = 10;
    limits.threads = 4;

    std::atomic<bool> completed = false;
    auto result = surakarta::bitboard::SearchResult{};
    auto worker = std::thread([&]() {
        result = controller.Search(position, limits);
        completed.store(true, std::memory_order_release);
    });

    const auto activation_deadline = std::chrono::steady_clock::now() + 1s;
    while (std::chrono::steady_clock::now() < activation_deadline) {
        const auto snapshot = controller.Snapshot();
        if (snapshot.active && snapshot.nodes > 0) {
            break;
        }
        std::this_thread::sleep_for(5ms);
    }

    controller.RequestStop();

    const auto stop_deadline = std::chrono::steady_clock::now() + 2s;
    while (!completed.load(std::memory_order_acquire) &&
           std::chrono::steady_clock::now() < stop_deadline) {
        std::this_thread::sleep_for(5ms);
    }

    auto okay = true;
    okay &= Expect(completed.load(std::memory_order_acquire), "stop request should converge all search workers");
    worker.join();
    okay &= Expect(!controller.Snapshot().active, "snapshot should go inactive after stop");
    okay &= Expect(result.stats.nodes > 0, "stopped search should still report aggregate nodes");
    return okay;
}

bool TestSearchMultithreadEquivalence() {
    auto all_ok = true;

    const std::vector<std::pair<std::string, std::string>> cases{
        {"opening", ""},
        {"middlegame", std::string(BITBOARD_TEST_DATA_DIR) + "\\game1.txt"},
        {"endgame", std::string(BITBOARD_TEST_DATA_DIR) + "\\game6.txt"},
    };

    for (const auto& [label, file_name] : cases) {
        const auto position = LoadPositionFromFile(file_name);

        auto single_limits = SearchLimits{};
        single_limits.max_depth = 6;
        single_limits.threads = 1;

        auto multi_limits = single_limits;
        multi_limits.threads = 4;

        SearchController single_controller;
        SearchController multi_controller;
        const auto single = single_controller.Search(position, single_limits);
        const auto multi = multi_controller.Search(position, multi_limits);

        all_ok &= Expect(single.best_move.IsValid(), "single-thread search should return a legal best move for " + label);
        all_ok &= Expect(multi.best_move.IsValid(), "multi-thread search should return a legal best move for " + label);
        all_ok &= Expect(single.best_move.from == multi.best_move.from &&
                             single.best_move.to == multi.best_move.to &&
                             single.best_move.flags == multi.best_move.flags &&
                             single.best_move.aux == multi.best_move.aux,
                         "best move mismatch on " + label + ": 1T=" + FormatMove(single.best_move) +
                             ", 4T=" + FormatMove(multi.best_move));
        all_ok &= Expect(single.score == multi.score,
                         "score mismatch on " + label + ": 1T=" + std::to_string(single.score) +
                             ", 4T=" + std::to_string(multi.score));
    }

    return all_ok;
}

bool TestPrincipalVariationBounds() {
    SurakartaGame game;
    game.StartGame();
    const auto position = PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());

    SearchController controller;
    SearchLimits limits;
    limits.max_depth = 4;
    limits.threads = 4;

    auto okay = true;
    for (int iteration = 0; iteration < 4; ++iteration) {
        const auto result = controller.Search(position, limits);
        std::string reason;
        okay &= Expect(result.best_move.IsValid(), "search should keep returning a legal best move");
        okay &= Expect(ValidatePrincipalVariation(position, result, &reason),
                       "principal variation should stay bounded and legal: " + reason);
    }
    return okay;
}

bool TestAspirationStatsAndSnapshotPv() {
    const auto position = LoadPositionFromFile(std::string(BITBOARD_TEST_DATA_DIR) + "\\game1.txt");

    SearchController controller;
    SearchLimits limits;
    limits.max_depth = 6;
    limits.threads = 4;
    limits.aspiration_window = 1;

    const auto result = controller.Search(position, limits);
    const auto snapshot = controller.Snapshot();

    auto okay = true;
    std::string reason;
    okay &= Expect(result.best_move.IsValid(), "search should return a legal best move for aspiration stats test");
    okay &= Expect(ValidatePrincipalVariation(position, result, &reason),
                   "search result pv should stay legal for aspiration stats test: " + reason);
    okay &= Expect(result.pv_length > 0, "search result should publish a non-empty principal variation");
    okay &= Expect(result.stats.aspiration_retries > 0,
                   "narrow aspiration window should trigger at least one aspiration retry");
    okay &= Expect(result.stats.aspiration_retries == result.stats.fail_highs + result.stats.fail_lows,
                   "aspiration retry accounting should match fail-high/fail-low counts");
    okay &= Expect(result.stats.aspiration_attempts >= static_cast<std::uint64_t>(limits.max_depth),
                   "aspiration attempts should count the initial search attempt for each depth");
    okay &= Expect(result.stats.full_window_researches > 0,
                   "mate-like aspiration instability should promote at least one full-window re-search");
    okay &= Expect(result.stats.aspiration_retries < 64,
                   "phase 3B aspiration strategy should avoid linear retry explosions");
    okay &= Expect(result.stats.root_best_updates > 0,
                   "root search should report at least one best-update event");
    okay &= Expect(result.stats.beta_cutoffs > 0,
                   "search should report beta cutoffs after 3B telemetry expansion");
    okay &= Expect(result.stats.thread_spawn_count == 3,
                   "persistent worker pool should spawn threads once per search lifecycle");
    okay &= Expect(result.stats.stale_root_tasks + result.stats.aborted_root_tasks > 0,
                   "multithreaded fail-high search should report stale or aborted root work");
    okay &= Expect(snapshot.pv_length == result.pv_length,
                   "final snapshot should preserve the completed search pv length");
    okay &= Expect(EqualMove(snapshot.best_move, result.best_move),
                   "final snapshot best move should match completed search result");
    okay &= Expect(snapshot.fail_highs == result.stats.fail_highs,
                   "final snapshot fail-high count should match result stats");
    okay &= Expect(snapshot.fail_lows == result.stats.fail_lows,
                   "final snapshot fail-low count should match result stats");
    okay &= Expect(snapshot.aspiration_retries == result.stats.aspiration_retries,
                   "final snapshot aspiration retry count should match result stats");
    okay &= Expect(snapshot.aspiration_attempts == result.stats.aspiration_attempts,
                   "final snapshot aspiration attempt count should match result stats");
    okay &= Expect(snapshot.full_window_researches == result.stats.full_window_researches,
                   "final snapshot full-window re-search count should match result stats");
    okay &= Expect(snapshot.root_best_updates == result.stats.root_best_updates,
                   "final snapshot root best update count should match result stats");
    okay &= Expect(snapshot.tt_exact_cutoffs == result.stats.tt_exact_cutoffs,
                   "final snapshot tt exact cutoff count should match result stats");
    okay &= Expect(snapshot.tt_bound_cutoffs == result.stats.tt_bound_cutoffs,
                   "final snapshot tt bound cutoff count should match result stats");
    okay &= Expect(snapshot.beta_cutoffs == result.stats.beta_cutoffs,
                   "final snapshot beta cutoff count should match result stats");
    okay &= Expect(snapshot.stale_root_tasks == result.stats.stale_root_tasks,
                   "final snapshot stale root task count should match result stats");
    okay &= Expect(snapshot.aborted_root_tasks == result.stats.aborted_root_tasks,
                   "final snapshot aborted root task count should match result stats");
    okay &= Expect(snapshot.worker_idle_spins == result.stats.worker_idle_spins,
                   "final snapshot worker idle count should match result stats");
    okay &= Expect(snapshot.thread_spawn_count == result.stats.thread_spawn_count,
                   "final snapshot thread spawn count should match result stats");
    if (snapshot.pv_length > 0) {
        okay &= Expect(EqualMove(snapshot.pv[0], snapshot.best_move),
                       "final snapshot pv head should match snapshot best move");
    }
    return okay;
}

bool TestMiddlegameRootBestMatchesReference() {
    const auto position = LoadPositionFromFile(std::string(BITBOARD_TEST_DATA_DIR) + "\\game1.txt");

    SearchController controller;
    SearchLimits limits;
    limits.max_depth = 6;
    limits.threads = 1;
    limits.aspiration_window = 1'000'000;

    const auto result = controller.Search(position, limits);

    SearchController reference_controller;
    const auto reference = ReferenceRootBestMove(position, reference_controller.Evaluator(), limits.max_depth);

    auto okay = true;
    okay &= Expect(result.score == reference.score,
                   "middlegame depth-6 score should match reference root search: search=" +
                       std::to_string(result.score) + ", reference=" + std::to_string(reference.score));
    okay &= Expect(EqualMove(result.best_move, reference.best_move),
                   "middlegame depth-6 best move should match reference root search: search=" +
                       FormatMove(result.best_move) + ", reference=" + FormatMove(reference.best_move));
    return okay;
}

bool TestBenchmarkJsonOutput() {
    const auto benchmark = BenchmarkExecutablePath();
    const auto file_name = std::string(BITBOARD_TEST_DATA_DIR) + "\\game1.txt";
    const auto base_args = std::string{" --file "} + QuoteCommandArg(file_name) +
                           " --depth 6 --threads 1 --format json --case-id middlegame";

    auto search_output = std::string{};
    const auto search_command = QuoteCommandArg(benchmark.string()) + " bitboard-search" + base_args;
    const auto search_exit = RunCommandCapture(search_command, &search_output);

    auto benchmark_output = std::string{};
    const auto benchmark_command = QuoteCommandArg(benchmark.string()) +
                                   " bitboard-benchmark --movegen-iters 1000" + base_args;
    const auto benchmark_exit = RunCommandCapture(benchmark_command, &benchmark_output);

    auto okay = true;
    okay &= Expect(std::filesystem::exists(benchmark), "benchmark executable should exist next to selftest");
    okay &= Expect(search_exit == 0, "bitboard-search --format json should exit successfully");
    okay &= Expect(benchmark_exit == 0, "bitboard-benchmark --format json should exit successfully");
    okay &= ExpectJsonReportShape(search_output, "middlegame", "bitboard-search json output");
    okay &= ExpectJsonReportShape(benchmark_output, "middlegame", "bitboard-benchmark json output");
    return okay;
}

bool TestWeightRoundTrip() {
    SearchController controller;
    const auto exported = controller.Evaluator().ExportWeights();

    TempDirGuard temp_dir("surakarta-weight-roundtrip");
    const auto weight_path = temp_dir.path / "roundtrip.bin";

    auto okay = true;
    okay &= Expect(exported.SaveBinary(weight_path.string()), "bootstrap weights should save to disk");

    NTupleWeights loaded;
    okay &= Expect(loaded.LoadBinary(weight_path.string()), "saved weights should load from disk");
    okay &= Expect(loaded.header.magic == exported.header.magic, "weight round-trip should preserve header magic");
    okay &= Expect(loaded.header.version == exported.header.version, "weight round-trip should preserve header version");
    okay &= Expect(loaded.header.tuple_set_hash == exported.header.tuple_set_hash,
                   "weight round-trip should preserve tuple hash");
    okay &= Expect(loaded.header.weight_count == exported.header.weight_count,
                   "weight round-trip should preserve weight count");
    okay &= Expect(loaded.values.size() == exported.values.size(),
                   "weight round-trip should preserve vector size");

    SearchController reloaded;
    okay &= Expect(reloaded.Evaluator().ApplyWeights(loaded), "loaded weights should apply to evaluator");
    const auto reexported = reloaded.Evaluator().ExportWeights();
    okay &= Expect(reexported.header.tuple_set_hash == exported.header.tuple_set_hash,
                   "re-exported weights should keep tuple hash");
    okay &= Expect(reexported.values.size() == exported.values.size(),
                   "re-exported weights should keep vector size");
    for (std::size_t i = 0; i < exported.values.size(); ++i) {
        okay &= Expect(static_cast<int>(std::llround(exported.values[i])) == static_cast<int>(std::llround(reexported.values[i])),
                       "weight round-trip should preserve quantized weight at index " + std::to_string(i));
    }
    return okay;
}

bool TestTrainingReproducibility() {
    TempDirGuard temp_dir("surakarta-training-repro");
    const auto first_output = temp_dir.path / "candidate-a.bin";
    const auto second_output = temp_dir.path / "candidate-b.bin";
    const auto first_checkpoints = temp_dir.path / "checkpoints-a";
    const auto second_checkpoints = temp_dir.path / "checkpoints-b";

    auto options = TrainingOptions{};
    options.output_weights_path = first_output.string();
    options.games = 2;
    options.seed = 1337;
    options.checkpoint_every = 1;
    options.checkpoint_dir = first_checkpoints.string();

    auto first_summary = TrainingSummary{};
    auto error_message = std::string{};
    auto okay = true;
    okay &= Expect(surakarta::bitboard::RunBitboardTraining(options, &first_summary, &error_message),
                   "first deterministic training run should succeed: " + error_message);

    options.output_weights_path = second_output.string();
    options.checkpoint_dir = second_checkpoints.string();
    auto second_summary = TrainingSummary{};
    error_message.clear();
    okay &= Expect(surakarta::bitboard::RunBitboardTraining(options, &second_summary, &error_message),
                   "second deterministic training run should succeed: " + error_message);

    okay &= Expect(std::filesystem::exists(first_output), "first training run should write candidate weights");
    okay &= Expect(std::filesystem::exists(second_output), "second training run should write candidate weights");
    okay &= Expect(ReadFileBinary(first_output) == ReadFileBinary(second_output),
                   "deterministic training should reproduce identical candidate bytes");
    okay &= Expect(first_summary.games_completed == second_summary.games_completed,
                   "deterministic training should reproduce completed game count");
    okay &= Expect(first_summary.black_wins == second_summary.black_wins,
                   "deterministic training should reproduce black win count");
    okay &= Expect(first_summary.white_wins == second_summary.white_wins,
                   "deterministic training should reproduce white win count");
    okay &= Expect(first_summary.draws == second_summary.draws,
                   "deterministic training should reproduce draw count");
    okay &= Expect(first_summary.checkpoint_count == second_summary.checkpoint_count,
                   "deterministic training should reproduce checkpoint count");
    okay &= Expect(first_summary.update_count == second_summary.update_count,
                   "deterministic training should reproduce update count");
    okay &= Expect(first_summary.positions_evaluated == second_summary.positions_evaluated,
                   "deterministic training should reproduce evaluated position count");
    okay &= Expect(std::abs(first_summary.average_abs_td_error - second_summary.average_abs_td_error) < 1e-9,
                   "deterministic training should reproduce average td error");
    okay &= Expect(std::abs(first_summary.max_abs_td_error - second_summary.max_abs_td_error) < 1e-9,
                   "deterministic training should reproduce max td error");
    okay &= Expect(std::abs(first_summary.average_abs_weight_delta - second_summary.average_abs_weight_delta) < 1e-9,
                   "deterministic training should reproduce average weight delta");
    okay &= Expect(std::abs(first_summary.max_abs_weight_delta - second_summary.max_abs_weight_delta) < 1e-9,
                   "deterministic training should reproduce max weight delta");
    return okay;
}

bool TestTrainedWeightsLoadInCli() {
    TempDirGuard temp_dir("surakarta-trained-cli");
    const auto candidate_path = temp_dir.path / "candidate.bin";

    auto options = TrainingOptions{};
    options.output_weights_path = candidate_path.string();
    options.games = 1;
    options.seed = 20260423;

    auto summary = TrainingSummary{};
    auto error_message = std::string{};
    auto okay = true;
    okay &= Expect(surakarta::bitboard::RunBitboardTraining(options, &summary, &error_message),
                   "training smoke should produce candidate weights: " + error_message);
    okay &= Expect(std::filesystem::exists(candidate_path), "training smoke should write candidate weights");

    const auto benchmark = BenchmarkExecutablePath();
    const auto middlegame = std::string(BITBOARD_TEST_DATA_DIR) + "\\game1.txt";
    const auto base_args = std::string{" --weights "} + QuoteCommandArg(candidate_path.string()) +
                           " --format json --depth 4 --threads 1 --case-id middlegame --file " +
                           QuoteCommandArg(middlegame);

    auto search_output = std::string{};
    const auto search_exit = RunCommandCapture(QuoteCommandArg(benchmark.string()) + " bitboard-search" + base_args,
                                               &search_output);
    auto benchmark_output = std::string{};
    const auto benchmark_exit =
        RunCommandCapture(QuoteCommandArg(benchmark.string()) + " bitboard-benchmark --movegen-iters 256" + base_args,
                          &benchmark_output);

    okay &= Expect(search_exit == 0, "trained weights should load in bitboard-search");
    okay &= Expect(benchmark_exit == 0, "trained weights should load in bitboard-benchmark");
    okay &= ExpectJsonReportShape(search_output, "middlegame", "trained bitboard-search json output");
    okay &= ExpectJsonReportShape(benchmark_output, "middlegame", "trained bitboard-benchmark json output");
    return okay;
}

bool TestEvalJsonOutput() {
    TempDirGuard temp_dir("surakarta-eval-json");
    const auto candidate_path = temp_dir.path / "candidate.bin";

    auto options = TrainingOptions{};
    options.output_weights_path = candidate_path.string();
    options.games = 0;

    auto summary = TrainingSummary{};
    auto error_message = std::string{};
    auto okay = true;
    okay &= Expect(surakarta::bitboard::RunBitboardTraining(options, &summary, &error_message),
                   "bootstrap export should succeed before eval smoke: " + error_message);

    const auto benchmark = BenchmarkExecutablePath();
    auto eval_output = std::string{};
    const auto eval_command = QuoteCommandArg(benchmark.string()) + " bitboard-eval --candidate " +
                              QuoteCommandArg(candidate_path.string()) + " --depth 4 --format json";
    const auto eval_exit = RunCommandCapture(eval_command, &eval_output);

    okay &= Expect(eval_exit == 0, "bitboard-eval --format json should exit successfully");
    okay &= ExpectEvalJsonReportShape(eval_output, "bitboard-eval json output");
    return okay;
}

bool TestStaticMaskParity() {
    const auto generated = surakarta::bitboard::BitboardGenerator().Build();
    const auto& tables = surakarta::bitboard::GetBitboardTables();
    auto okay = true;
    okay &= Expect(generated.quiet_move_mask == tables.quiet_move_mask, "quiet masks drifted from generator output");
    okay &= Expect(generated.capture_target_mask == tables.capture_target_mask, "capture target masks drifted from generator output");
    okay &= Expect(generated.capture_variant_count == tables.capture_variant_count, "capture variant counts drifted from generator output");
    okay &= Expect(generated.capture_clear_mask == tables.capture_clear_mask, "capture clear masks drifted from generator output");
    okay &= Expect(generated.capture_start_dir == tables.capture_start_dir, "capture start dirs drifted from generator output");
    return okay;
}

bool TestDebugTargetDescriptionsAndPaths() {
    const auto file_name = std::string(BITBOARD_TEST_DATA_DIR) + "\\game1.txt";

    SurakartaGame legacy_game;
    legacy_game.StartGame(file_name);
    const auto legacy_board = legacy_game.GetBoard();
    const auto legacy_game_info = legacy_game.GetGameInfo();

    const auto position = PositionBuilder::FromLegacy(*legacy_board, *legacy_game_info);
    const auto from_pos = SurakartaPosition(2, 2);
    const auto from = PositionAdapter::FromLegacyPosition(from_pos);
    const auto mask = surakarta::bitboard::GetLegalTargetMask(position, from);
    const auto targets = DescribeLegalTargets(position, from);

    auto okay = true;
    okay &= Expect(static_cast<int>(targets.size()) == std::popcount(mask),
                   "described targets should match legal target mask popcount");

    bool found_capture = false;
    for (const auto& target : targets) {
        okay &= Expect((mask & Bit(target.to)) != 0, "described target must exist in legal target mask");
        if (target.to == MakeSquare(2, 0)) {
            found_capture = true;
            okay &= Expect(target.IsCapture(), "capture target should be marked as capture");
            okay &= Expect(target.variant_count > 0, "capture target should expose at least one path variant");

            const auto path = ReconstructCapturePath(position, from, target.to, target.primary_variant);
            okay &= Expect(!path.empty(), "reconstructed capture path should not be empty");

            const auto trace_lists = SurakartaInitPositionListsUtil(legacy_board).InitPositionList();
            const auto start_direction = SurakartaMovablityUtil(legacy_board).GetDirectionCapture(
                *(*legacy_board)[from_pos.x][from_pos.y],
                *(*legacy_board)[2][0]);
            const auto trace = SurakartaTraceGenerateUtil(legacy_board, trace_lists.black_list, trace_lists.white_list)
                                   .GenerateCaptured(from_pos, start_direction);
            okay &= Expect(path.size() == trace.path.size(), "reconstructed capture path length mismatches legacy trace");
            const auto compare_count = std::min<std::size_t>(path.size(), trace.path.size());
            for (std::size_t i = 0; i < compare_count; ++i) {
                okay &= Expect(EqualFragment(path[i], trace.path[i]), "reconstructed capture fragment mismatches legacy trace");
            }
        }
    }

    okay &= Expect(found_capture, "expected capture target was not described");
    return okay;
}

}  // namespace

int main() {
    static_assert(sizeof(Move) == 4, "Move must stay POD-sized");
    static_assert(alignof(surakarta::bitboard::Position) == 64, "Position must be cache-line aligned");

    bool ok = true;
    std::cerr << "[TEST] TestQuietMask" << std::endl;
    ok &= TestQuietMask();
    std::cerr << "[TEST] TestRuleParity" << std::endl;
    ok &= TestRuleParity();
    std::cerr << "[TEST] TestEvaluateBitboardTerminalParity" << std::endl;
    ok &= TestEvaluateBitboardTerminalParity();
    std::cerr << "[TEST] TestSearchParity" << std::endl;
    ok &= TestSearchParity();
    std::cerr << "[TEST] TestSharedNodeBudget" << std::endl;
    ok &= TestSharedNodeBudget();
    std::cerr << "[TEST] TestSharedStopRequest" << std::endl;
    ok &= TestSharedStopRequest();
    std::cerr << "[TEST] TestSearchMultithreadEquivalence" << std::endl;
    ok &= TestSearchMultithreadEquivalence();
    std::cerr << "[TEST] TestPrincipalVariationBounds" << std::endl;
    ok &= TestPrincipalVariationBounds();
    std::cerr << "[TEST] TestAspirationStatsAndSnapshotPv" << std::endl;
    ok &= TestAspirationStatsAndSnapshotPv();
    std::cerr << "[TEST] TestMiddlegameRootBestMatchesReference" << std::endl;
    ok &= TestMiddlegameRootBestMatchesReference();
    std::cerr << "[TEST] TestBenchmarkJsonOutput" << std::endl;
    ok &= TestBenchmarkJsonOutput();
    std::cerr << "[TEST] TestWeightRoundTrip" << std::endl;
    ok &= TestWeightRoundTrip();
    std::cerr << "[TEST] TestTrainingReproducibility" << std::endl;
    ok &= TestTrainingReproducibility();
    std::cerr << "[TEST] TestTrainedWeightsLoadInCli" << std::endl;
    ok &= TestTrainedWeightsLoadInCli();
    std::cerr << "[TEST] TestEvalJsonOutput" << std::endl;
    ok &= TestEvalJsonOutput();
    std::cerr << "[TEST] TestStaticMaskParity" << std::endl;
    ok &= TestStaticMaskParity();
    std::cerr << "[TEST] TestDebugTargetDescriptionsAndPaths" << std::endl;
    ok &= TestDebugTargetDescriptionsAndPaths();
    if (ok) {
        std::cout << "[PASS] surakarta-bitboard-selftest" << std::endl;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
