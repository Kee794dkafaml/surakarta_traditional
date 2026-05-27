#include <algorithm>
#include <atomic>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <set>
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

bool AlmostEqual(double lhs, double rhs, double epsilon = 1e-9) {
    return std::abs(lhs - rhs) <= epsilon;
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

void ClearLegacyBoard(const std::shared_ptr<SurakartaBoard>& board) {
    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            (*board)[x][y] = std::make_shared<SurakartaPiece>(x, y, PieceColor::NONE);
        }
    }
}

void PutLegacyPiece(const std::shared_ptr<SurakartaBoard>& board, int x, int y, PieceColor color) {
    (*board)[x][y] = std::make_shared<SurakartaPiece>(x, y, color);
}

struct PiecePlacement {
    int x;
    int y;
    PieceColor color;
};

struct RuleMoveExpectation {
    SurakartaMove move;
    SurakartaIllegalMoveReason expected;
    std::string label;
};

struct RuleGoldenPosition {
    std::string label;
    PieceColor current_player;
    std::vector<PiecePlacement> pieces;
    std::vector<RuleMoveExpectation> moves;
};

std::string MoveKey(int from_x, int from_y, int to_x, int to_y, bool capture) {
    return std::to_string(from_x) + "," + std::to_string(from_y) + (capture ? "x" : "-") +
           std::to_string(to_x) + "," + std::to_string(to_y);
}

std::string MoveKey(const Move& move) {
    const auto from = PositionAdapter::ToLegacyPosition(move.from);
    const auto to = PositionAdapter::ToLegacyPosition(move.to);
    return MoveKey(from.x, from.y, to.x, to.y, move.IsCapture());
}

std::string MoveKey(const SurakartaMove& move, bool capture) {
    return MoveKey(move.from.x, move.from.y, move.to.x, move.to.y, capture);
}

std::string SquareLabel(surakarta::bitboard::Square square) {
    return "(" + std::to_string(surakarta::bitboard::SquareX(square)) + "," +
           std::to_string(surakarta::bitboard::SquareY(square)) + ")";
}

SurakartaGame MakeRuleGoldenGame(const RuleGoldenPosition& golden) {
    SurakartaGame game;
    game.StartGame();
    ClearLegacyBoard(game.GetBoard());
    for (const auto& piece : golden.pieces) {
        PutLegacyPiece(game.GetBoard(), piece.x, piece.y, piece.color);
    }

    auto game_info = game.GetGameInfo();
    game_info->current_player_ = golden.current_player;
    game_info->num_round_ = 7;
    game_info->last_captured_round_ = 3;
    game_info->end_reason_ = SurakartaEndReason::NONE;
    game_info->winner_ = PieceColor::NONE;
    game_info->max_no_capture_round_ = MAX_NO_CAPTURE_ROUND;
    return game;
}

std::set<std::string> BuildLegacyMoveKeys(SurakartaGame& game, bool captures_only) {
    auto keys = std::set<std::string>{};
    const auto board = game.GetBoard();
    const auto game_info = game.GetGameInfo();
    auto rule = game.GetRuleManager();

    for (int x = 0; x < BOARD_SIZE; ++x) {
        for (int y = 0; y < BOARD_SIZE; ++y) {
            if ((*board)[x][y]->GetColor() != game_info->current_player_) {
                continue;
            }
            const auto targets = rule->GetAllLegalTarget(SurakartaPosition(x, y));
            for (const auto& target : *targets) {
                const auto target_color = (*board)[target.x][target.y]->GetColor();
                const bool capture = target_color != PieceColor::NONE && target_color != game_info->current_player_;
                if (!captures_only || capture) {
                    keys.insert(MoveKey(x, y, target.x, target.y, capture));
                }
            }
        }
    }
    return keys;
}

std::set<std::string> BuildBitboardMoveKeys(const surakarta::bitboard::Position& position, bool captures_only) {
    auto moves = surakarta::bitboard::MoveList{};
    surakarta::bitboard::GenerateMoves(position, moves, captures_only);

    auto keys = std::set<std::string>{};
    for (int i = 0; i < moves.size; ++i) {
        keys.insert(MoveKey(moves.moves[i]));
    }
    return keys;
}

SurakartaGame MakeVariantOverflowCaptureGame(bool block_remaining_variant) {
    SurakartaGame game;
    game.StartGame();
    ClearLegacyBoard(game.GetBoard());
    PutLegacyPiece(game.GetBoard(), 2, 1, PieceColor::BLACK);
    PutLegacyPiece(game.GetBoard(), 2, 2, PieceColor::BLACK);
    PutLegacyPiece(game.GetBoard(), 2, 3, PieceColor::BLACK);
    PutLegacyPiece(game.GetBoard(), 3, 3, PieceColor::WHITE);
    if (block_remaining_variant) {
        PutLegacyPiece(game.GetBoard(), 3, 2, PieceColor::BLACK);
    }

    auto game_info = game.GetGameInfo();
    game_info->current_player_ = PieceColor::BLACK;
    game_info->num_round_ = 9;
    game_info->last_captured_round_ = 4;
    game_info->end_reason_ = SurakartaEndReason::NONE;
    game_info->winner_ = PieceColor::NONE;
    game_info->max_no_capture_round_ = MAX_NO_CAPTURE_ROUND;
    return game;
}

SurakartaGame MakeQuietMoveGoldenGame() {
    SurakartaGame game;
    game.StartGame();
    ClearLegacyBoard(game.GetBoard());
    PutLegacyPiece(game.GetBoard(), 2, 2, PieceColor::BLACK);
    auto game_info = game.GetGameInfo();
    game_info->current_player_ = PieceColor::BLACK;
    game_info->num_round_ = 1;
    game_info->last_captured_round_ = 0;
    game_info->end_reason_ = SurakartaEndReason::NONE;
    game_info->winner_ = PieceColor::NONE;
    game_info->max_no_capture_round_ = MAX_NO_CAPTURE_ROUND;
    return game;
}

bool MoveListContains(const surakarta::bitboard::MoveList& moves,
                      surakarta::bitboard::Square from,
                      surakarta::bitboard::Square to,
                      bool capture) {
    for (int i = 0; i < moves.size; ++i) {
        const auto move = moves.moves[i];
        if (move.from == from && move.to == to && move.IsCapture() == capture) {
            return true;
        }
    }
    return false;
}

surakarta::bitboard::Bitboard MaskFromSquares(const std::vector<std::pair<int, int>>& squares) {
    auto mask = surakarta::bitboard::Bitboard{0};
    for (const auto& [x, y] : squares) {
        mask |= Bit(MakeSquare(x, y));
    }
    return mask;
}

bool HasCurveFragment(const std::vector<SurakartaMovePathFragment>& path) {
    for (const auto& fragment : path) {
        if (fragment.is_curve) {
            return true;
        }
    }
    return false;
}

std::string FormatPathForTest(const std::vector<SurakartaMovePathFragment>& path) {
    auto stream = std::ostringstream{};
    for (std::size_t i = 0; i < path.size(); ++i) {
        if (i > 0) {
            stream << " | ";
        }
        const auto& fragment = path[i];
        if (fragment.is_curve) {
            stream << "arc(center=(" << fragment.info.curve.center_x << "," << fragment.info.curve.center_y
                   << "), radius=" << fragment.info.curve.radius
                   << ", angles=" << fragment.info.curve.start_angle << "->" << fragment.info.curve.end_angle
                   << ", clockwise=" << (fragment.info.curve.is_clockwise ? "true" : "false") << ")";
        } else {
            stream << "(" << fragment.info.straight.start_x << "," << fragment.info.straight.start_y << ")->("
                   << fragment.info.straight.end_x << "," << fragment.info.straight.end_y << ")";
        }
    }
    return stream.str();
}

bool ValidateCapturePathInvariant(const surakarta::bitboard::Position& position, Move move, const std::string& label) {
    if (!move.IsCapture()) {
        return true;
    }

    const auto path = ReconstructCapturePath(position, move.from, move.to, move.aux);
    auto okay = true;
    okay &= Expect(!path.empty(), label + " capture must reconstruct a non-empty path");
    okay &= Expect(HasCurveFragment(path),
                   label + " capture path must include a loop/arc segment, path=" + FormatPathForTest(path));
    if (!path.empty()) {
        const auto from = PositionAdapter::ToLegacyPosition(move.from);
        const auto to = PositionAdapter::ToLegacyPosition(move.to);
        okay &= Expect(path.front().From() == from,
                       label + " capture path must start at the move source, path=" + FormatPathForTest(path));
        okay &= Expect(path.back().To() == to,
                       label + " capture path must end at the captured target, path=" + FormatPathForTest(path));
    }
    return okay;
}

bool SamePosition(const surakarta::bitboard::Position& lhs, const surakarta::bitboard::Position& rhs) {
    return lhs.board.pieces[0] == rhs.board.pieces[0] &&
           lhs.board.pieces[1] == rhs.board.pieces[1] &&
           lhs.zobrist_key == rhs.zobrist_key &&
           lhs.eval_cache == rhs.eval_cache &&
           lhs.no_capture_ply == rhs.no_capture_ply &&
           lhs.ply == rhs.ply &&
           lhs.max_no_capture_round == rhs.max_no_capture_round &&
           lhs.side_to_move == rhs.side_to_move;
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
    okay &= Expect(text.find("\"case_id\":\"capture-heavy\"") != std::string::npos,
                   label + " should include capture-heavy report");
    okay &= Expect(text.find("\"case_id\":\"endgame\"") != std::string::npos,
                   label + " should include endgame report");
    okay &= Expect(text.find("\"case_id\":\"no-capture-critical\"") != std::string::npos,
                   label + " should include no-capture-critical report");
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

bool TestRuleCorrectnessGoldenParity() {
    const std::vector<RuleGoldenPosition> golden_positions{
        {"center quiet and no-target parity",
         PieceColor::BLACK,
         {{2, 2, PieceColor::BLACK}},
         {{SurakartaMove(2, 2, 1, 1, SurakartaPlayer::BLACK),
           SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE,
           "center quiet move"},
          {SurakartaMove(2, 2, 4, 4, SurakartaPlayer::BLACK),
           SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE,
           "center far empty no-target move"}}},
        {"corner quiet parity",
         PieceColor::BLACK,
         {{0, 0, PieceColor::BLACK}},
         {{SurakartaMove(0, 0, 1, 1, SurakartaPlayer::BLACK),
           SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE,
           "corner quiet move"}}},
        {"outer-ring edge capture parity",
         PieceColor::BLACK,
         {{5, 1, PieceColor::BLACK}, {5, 2, PieceColor::BLACK}, {1, 1, PieceColor::WHITE}},
         {{SurakartaMove(5, 1, 1, 1, SurakartaPlayer::BLACK),
           SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE,
           "edge outer-ring capture"}}},
        {"inner-ring multi-path P0 parity",
         PieceColor::BLACK,
         {{2, 1, PieceColor::BLACK}, {2, 2, PieceColor::BLACK}, {2, 3, PieceColor::BLACK}, {3, 3, PieceColor::WHITE}},
         {{SurakartaMove(2, 2, 3, 3, SurakartaPlayer::BLACK),
           SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE,
           "user P0 multi-path capture"}}},
        {"own-blocked capture parity",
         PieceColor::BLACK,
         {{2, 1, PieceColor::BLACK},
          {2, 2, PieceColor::BLACK},
          {2, 3, PieceColor::BLACK},
          {3, 2, PieceColor::BLACK},
          {3, 3, PieceColor::WHITE}},
         {{SurakartaMove(2, 2, 3, 3, SurakartaPlayer::BLACK),
           SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE,
           "own piece blocks every capture variant"}}},
        {"enemy-target capture parity",
         PieceColor::BLACK,
         {{3, 3, PieceColor::BLACK}, {3, 1, PieceColor::WHITE}, {2, 3, PieceColor::WHITE}},
         {{SurakartaMove(3, 3, 3, 1, SurakartaPlayer::BLACK),
           SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE,
           "vertical enemy-target capture"},
          {SurakartaMove(3, 3, 2, 3, SurakartaPlayer::BLACK),
           SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE,
           "horizontal enemy-target capture"}}},
    };

    auto all_ok = true;
    for (const auto& golden : golden_positions) {
        auto legacy_game = MakeRuleGoldenGame(golden);
        auto bitboard_game = MakeRuleGoldenGame(golden);
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
                all_ok &= Expect(*legacy_targets == *bitboard_targets,
                                 golden.label + " legal targets mismatch at (" + std::to_string(x) + "," +
                                     std::to_string(y) + ")");
            }
        }

        const auto position = PositionBuilder::FromLegacy(*bitboard_game.GetBoard(), *bitboard_game.GetGameInfo());
        const auto legacy_moves = BuildLegacyMoveKeys(legacy_game, false);
        const auto bitboard_moves = BuildBitboardMoveKeys(position, false);
        all_ok &= Expect(legacy_moves == bitboard_moves,
                         golden.label + " generated move set mismatch, traditional=" +
                             std::to_string(legacy_moves.size()) + " bitboard=" + std::to_string(bitboard_moves.size()));

        const auto legacy_captures = BuildLegacyMoveKeys(legacy_game, true);
        const auto bitboard_captures = BuildBitboardMoveKeys(position, true);
        all_ok &= Expect(legacy_captures == bitboard_captures,
                         golden.label + " capture-only move set mismatch, traditional=" +
                             std::to_string(legacy_captures.size()) + " bitboard=" + std::to_string(bitboard_captures.size()));

        for (const auto& expectation : golden.moves) {
            const auto legacy_reason = legacy_rule->JudgeMove(expectation.move);
            const auto bitboard_reason = bitboard_rule->JudgeMove(expectation.move);
            all_ok &= Expect(legacy_reason == expectation.expected,
                             golden.label + " traditional " + expectation.label + " expected " +
                                 SurakartaToString(expectation.expected) + ", actual: " +
                                 SurakartaToString(legacy_reason));
            all_ok &= Expect(bitboard_reason == legacy_reason,
                             golden.label + " bitboard/traditional reason mismatch for " + expectation.label +
                                 ", traditional: " + SurakartaToString(legacy_reason) +
                                 ", bitboard: " + SurakartaToString(bitboard_reason));

            if (expectation.expected == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
                all_ok &= Expect(bitboard_captures.contains(MoveKey(expectation.move, true)),
                                 golden.label + " capture-only movegen missing " + expectation.label);
            } else if (expectation.expected == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
                all_ok &= Expect(bitboard_moves.contains(MoveKey(expectation.move, false)),
                                 golden.label + " quiet movegen missing " + expectation.label);
            } else {
                all_ok &= Expect(!bitboard_moves.contains(MoveKey(expectation.move, false)) &&
                                     !bitboard_moves.contains(MoveKey(expectation.move, true)),
                                 golden.label + " generated illegal move for " + expectation.label);
            }
        }
    }

    return all_ok;
}

bool TestCaptureVariantOverflowGolden() {
    auto game = MakeVariantOverflowCaptureGame(false);
    const auto move = SurakartaMove(2, 2, 3, 3, SurakartaPlayer::BLACK);
    auto legacy_rule = game.GetRuleManager();
    auto bitboard_rule = std::make_shared<SurakartaRuleManagerBitboard>(game.GetBoard(), game.GetGameInfo());

    const auto legacy_reason = legacy_rule->JudgeMove(move);
    const auto bitboard_reason = bitboard_rule->JudgeMove(move);
    const auto position = PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());
    const auto from = MakeSquare(2, 2);
    const auto to = MakeSquare(3, 3);

    auto captures = surakarta::bitboard::MoveList{};
    surakarta::bitboard::GenerateMoves(position, captures, true);

    bool described_capture = false;
    const auto targets = DescribeLegalTargets(position, surakarta::bitboard::Color::Black, from);
    for (const auto& target : targets) {
        if (target.to == to && target.IsCapture()) {
            described_capture = true;
            break;
        }
    }

    auto okay = true;
    okay &= Expect(legacy_reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE,
                   "traditional movegen should accept golden capture (2,2)->(3,3)");
    okay &= Expect(surakarta::bitboard::GetBitboardTables().capture_variant_count[from][to] == 8,
                   "golden capture (2,2)->(3,3) should preserve all 8 path variants");
    okay &= Expect(bitboard_reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE,
                   "bitboard movegen should accept golden capture (2,2)->(3,3), actual: " +
                       SurakartaToString(bitboard_reason));
    okay &= Expect(MoveListContains(captures, from, to, true),
                   "capture-only bitboard movegen should include golden capture (2,2)->(3,3)");
    okay &= Expect(described_capture,
                   "DescribeLegalTargets should include golden capture (2,2)->(3,3)");
    return okay;
}

bool TestIllegalCaptureVariantBlocked() {
    auto game = MakeVariantOverflowCaptureGame(true);
    const auto move = SurakartaMove(2, 2, 3, 3, SurakartaPlayer::BLACK);
    auto legacy_rule = game.GetRuleManager();
    auto bitboard_rule = std::make_shared<SurakartaRuleManagerBitboard>(game.GetBoard(), game.GetGameInfo());
    const auto legacy_reason = legacy_rule->JudgeMove(move);
    const auto bitboard_reason = bitboard_rule->JudgeMove(move);

    const auto position = PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());
    auto captures = surakarta::bitboard::MoveList{};
    surakarta::bitboard::GenerateMoves(position, captures, true);

    auto okay = true;
    okay &= Expect(legacy_reason == SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE,
                   "traditional movegen should reject blocked golden capture");
    okay &= Expect(bitboard_reason == SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE,
                   "bitboard movegen should reject blocked golden capture");
    okay &= Expect(!MoveListContains(captures, MakeSquare(2, 2), MakeSquare(3, 3), true),
                   "capture-only bitboard movegen should not include blocked capture");
    return okay;
}

bool TestQuietMoveGoldenStillWorks() {
    auto game = MakeQuietMoveGoldenGame();
    const auto position = PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());
    auto moves = surakarta::bitboard::MoveList{};
    surakarta::bitboard::GenerateMoves(position, moves, false);

    auto okay = true;
    okay &= Expect(MoveListContains(moves, MakeSquare(2, 2), MakeSquare(1, 1), false),
                   "normal quiet move (2,2)->(1,1) should still be generated");
    okay &= Expect(MoveListContains(moves, MakeSquare(2, 2), MakeSquare(3, 3), false),
                   "normal quiet move (2,2)->(3,3) should still be generated");
    return okay;
}

bool TestCaptureMakeUnmakeAndNoCaptureReset() {
    auto game = MakeVariantOverflowCaptureGame(false);
    auto position = PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());
    position.no_capture_ply = 11;
    position.zobrist_key = surakarta::bitboard::ComputeZobrist(position);
    const auto before = position;

    auto captures = surakarta::bitboard::MoveList{};
    surakarta::bitboard::GenerateMoves(position, captures, true);
    const auto from = MakeSquare(2, 2);
    const auto to = MakeSquare(3, 3);

    auto capture = Move{};
    bool found = false;
    for (int i = 0; i < captures.size; ++i) {
        if (captures.moves[i].from == from && captures.moves[i].to == to && captures.moves[i].IsCapture()) {
            capture = captures.moves[i];
            found = true;
            break;
        }
    }

    auto okay = true;
    okay &= Expect(found, "make/unmake golden capture test requires generated capture");
    if (!found) {
        return okay;
    }

    auto undo = Undo{};
    surakarta::bitboard::MakeMove(position, capture, undo);
    okay &= Expect(position.board.ColorAt(from) == surakarta::bitboard::Color::None,
                   "capture should clear source square");
    okay &= Expect(position.board.ColorAt(to) == surakarta::bitboard::Color::Black,
                   "capture should move black piece onto target square");
    okay &= Expect(position.board.Count(surakarta::bitboard::Color::White) == 0,
                   "capture should remove target white piece");
    okay &= Expect(position.no_capture_ply == 0,
                   "capture should reset no-capture counter");
    surakarta::bitboard::UnmakeMove(position, capture, undo);
    okay &= Expect(SamePosition(position, before),
                   "capture make/unmake should restore the exact bitboard position");
    return okay;
}

bool TestSearchDepthOneSeesGoldenCapture() {
    auto game = MakeVariantOverflowCaptureGame(false);
    const auto position = PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());
    auto qsearch_source = surakarta::bitboard::MoveList{};
    surakarta::bitboard::GenerateMoves(position, qsearch_source, true);

    SearchController controller;
    SearchLimits limits;
    limits.max_depth = 1;
    limits.threads = 1;
    const auto result = controller.Search(position, limits);

    const auto from = MakeSquare(2, 2);
    const auto to = MakeSquare(3, 3);
    auto okay = true;
    okay &= Expect(MoveListContains(qsearch_source, from, to, true),
                   "qsearch capture-only source should include golden capture");
    okay &= Expect(result.best_move.from == from && result.best_move.to == to && result.best_move.IsCapture(),
                   "depth-1 search should choose the immediate checkmate capture, actual: " +
                       FormatMove(result.best_move));
    return okay;
}

std::string ScreenshotSameFileCaptureGoldenFile() {
    return std::string(BITBOARD_TEST_DATA_DIR) + "\\screenshot_2026_05_06_same_file_capture.txt";
}

bool TestScreenshotPositionRegressionAndCandidateCaptures() {
    SurakartaGame legacy_game;
    legacy_game.StartGame(ScreenshotSameFileCaptureGoldenFile());
    auto bitboard_game = legacy_game;
    const auto position = PositionBuilder::FromLegacy(*legacy_game.GetBoard(), *legacy_game.GetGameInfo());

    const auto expected_black = MaskFromSquares({
        {0, 0}, {1, 0}, {2, 0}, {3, 0}, {5, 0},
        {0, 1}, {1, 1}, {3, 1}, {4, 1}, {5, 1},
        {1, 2},
    });
    const auto expected_white = MaskFromSquares({
        {2, 1}, {3, 3},
        {0, 4}, {1, 4}, {3, 4}, {4, 4}, {5, 4},
        {0, 5}, {1, 5}, {3, 5}, {4, 5}, {5, 5},
    });

    auto okay = true;
    okay &= Expect(position.board.Pieces(surakarta::bitboard::Color::Black) == expected_black,
                   "screenshot golden black-piece coordinates must match the captured GUI board");
    okay &= Expect(position.board.Pieces(surakarta::bitboard::Color::White) == expected_white,
                   "screenshot golden white-piece coordinates must match the captured GUI board");
    okay &= Expect(position.SideToMove() == surakarta::bitboard::Color::Black,
                   "screenshot golden side to move must be black");
    okay &= Expect(position.zobrist_key == 0x705F017D88EF1EACull,
                   "screenshot golden zobrist must match the GUI sidebar value");

    auto legacy_rule = legacy_game.GetRuleManager();
    auto bitboard_rule =
        std::make_shared<SurakartaRuleManagerBitboard>(bitboard_game.GetBoard(), bitboard_game.GetGameInfo());
    const auto full_moves = BuildBitboardMoveKeys(position, false);
    const auto capture_moves = BuildBitboardMoveKeys(position, true);
    okay &= Expect(BuildLegacyMoveKeys(legacy_game, false) == full_moves,
                   "screenshot golden full movegen must match traditional movegen");
    okay &= Expect(BuildLegacyMoveKeys(legacy_game, true) == capture_moves,
                   "screenshot golden capture-only movegen must match traditional movegen");

    const auto check_rejected_candidate = [&](const SurakartaMove& candidate, const std::string& label) {
        auto candidate_okay = true;
        const auto legacy_reason = legacy_rule->JudgeMove(candidate);
        const auto bitboard_reason = bitboard_rule->JudgeMove(candidate);
        const auto from = PositionAdapter::FromLegacyPosition(candidate.from);
        const auto to = PositionAdapter::FromLegacyPosition(candidate.to);
        candidate_okay &= Expect(!IsLegalMoveReason(legacy_reason),
                                 label + " traditional rules must reject the no-loop/screenshot candidate, actual: " +
                                     SurakartaToString(legacy_reason));
        candidate_okay &= Expect(bitboard_reason == legacy_reason,
                                 label + " bitboard reason must match traditional reason, traditional: " +
                                     SurakartaToString(legacy_reason) + ", bitboard: " +
                                     SurakartaToString(bitboard_reason));
        candidate_okay &= Expect(!full_moves.contains(MoveKey(candidate, true)),
                                 label + " full movegen must not contain the rejected capture candidate");
        candidate_okay &= Expect(!capture_moves.contains(MoveKey(candidate, true)),
                                 label + " capture-only/qsearch source must not contain the rejected capture candidate");
        candidate_okay &= Expect(ReconstructCapturePath(position, from, to, 0).empty(),
                                 label + " rejected candidate must not have a capture path");
        return candidate_okay;
    };

    okay &= check_rejected_candidate(SurakartaMove(2, 3, 2, 1, SurakartaPlayer::BLACK),
                                     "engine text (2,3)->(2,1)x");
    okay &= check_rejected_candidate(SurakartaMove(2, 4, 2, 1, SurakartaPlayer::BLACK),
                                     "GUI-observed (2,4)->(2,1)x");

    return okay;
}

bool TestScreenshotNoStraightLineCaptureInvariant() {
    const auto position = LoadPositionFromFile(ScreenshotSameFileCaptureGoldenFile());

    auto full_moves = surakarta::bitboard::MoveList{};
    surakarta::bitboard::GenerateMoves(position, full_moves, false);
    auto capture_moves = surakarta::bitboard::MoveList{};
    surakarta::bitboard::GenerateMoves(position, capture_moves, true);

    auto full_capture_keys = std::set<std::string>{};
    auto capture_only_keys = std::set<std::string>{};
    auto okay = true;
    for (int i = 0; i < full_moves.size; ++i) {
        const auto move = full_moves.moves[i];
        if (move.IsCapture()) {
            full_capture_keys.insert(MoveKey(move));
            okay &= ValidateCapturePathInvariant(position, move, "screenshot full movegen " + MoveKey(move));
        }
    }
    for (int i = 0; i < capture_moves.size; ++i) {
        const auto move = capture_moves.moves[i];
        capture_only_keys.insert(MoveKey(move));
        okay &= Expect(move.IsCapture(), "screenshot capture-only movegen must emit captures only");
        okay &= ValidateCapturePathInvariant(position, move, "screenshot capture-only movegen " + MoveKey(move));
    }
    okay &= Expect(full_capture_keys == capture_only_keys,
                   "screenshot capture-only source must match the capture subset from full movegen");
    okay &= Expect(capture_moves.size == 0,
                   "screenshot black-to-move position must not expose any root capture");

    SearchController controller;
    SearchLimits limits;
    limits.max_depth = 1;
    limits.threads = 1;
    const auto result = controller.Search(position, limits);
    okay &= Expect(!(result.best_move.from == MakeSquare(2, 3) &&
                     result.best_move.to == MakeSquare(2, 1) &&
                     result.best_move.IsCapture()),
                   "depth-1 search must not return engine-text rejected capture (2,3)->(2,1)x");
    okay &= Expect(!(result.best_move.from == MakeSquare(2, 4) &&
                     result.best_move.to == MakeSquare(2, 1) &&
                     result.best_move.IsCapture()),
                   "depth-1 search must not return GUI-observed rejected capture (2,4)->(2,1)x");
    okay &= ValidateCapturePathInvariant(position, result.best_move, "screenshot depth-1 search best move");
    return okay;
}

bool TestAllGeneratedCapturesHaveLoopPaths() {
    const auto files = std::array<std::string, 11>{
        "",
        std::string(BITBOARD_TEST_DATA_DIR) + "\\game1.txt",
        std::string(BITBOARD_TEST_DATA_DIR) + "\\game2.txt",
        std::string(BITBOARD_TEST_DATA_DIR) + "\\game3.txt",
        std::string(BITBOARD_TEST_DATA_DIR) + "\\game4.txt",
        std::string(BITBOARD_TEST_DATA_DIR) + "\\game5.txt",
        std::string(BITBOARD_TEST_DATA_DIR) + "\\game6.txt",
        std::string(BITBOARD_TEST_DATA_DIR) + "\\game7.txt",
        std::string(BITBOARD_TEST_DATA_DIR) + "\\game8.txt",
        std::string(BITBOARD_TEST_DATA_DIR) + "\\game9.txt",
        ScreenshotSameFileCaptureGoldenFile(),
    };

    auto okay = true;
    for (const auto& file_name : files) {
        const auto position = LoadPositionFromFile(file_name);
        for (const auto captures_only : {false, true}) {
            auto moves = surakarta::bitboard::MoveList{};
            surakarta::bitboard::GenerateMoves(position, moves, captures_only);
            for (int i = 0; i < moves.size; ++i) {
                const auto move = moves.moves[i];
                if (captures_only) {
                    okay &= Expect(move.IsCapture(), "capture-only movegen emitted a non-capture move");
                }
                okay &= ValidateCapturePathInvariant(position,
                                                     move,
                                                     (file_name.empty() ? "initial" : file_name) + " " +
                                                         (captures_only ? "capture-only " : "full ") +
                                                         MoveKey(move));
            }
        }

        SearchController controller;
        SearchLimits limits;
        limits.max_depth = 1;
        limits.threads = 1;
        const auto result = controller.Search(position, limits);
        okay &= ValidateCapturePathInvariant(position,
                                             result.best_move,
                                             (file_name.empty() ? "initial" : file_name) + " search best move");
    }
    return okay;
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

bool TestTdTerminalRewardSigns() {
    using surakarta::bitboard::Color;

    const auto black_for_black = surakarta::bitboard::TrainingTerminalTarget(Color::Black, Color::Black);
    const auto black_for_white = surakarta::bitboard::TrainingTerminalTarget(Color::Black, Color::White);
    const auto white_for_black = surakarta::bitboard::TrainingTerminalTarget(Color::White, Color::Black);
    const auto white_for_white = surakarta::bitboard::TrainingTerminalTarget(Color::White, Color::White);
    const auto draw_for_black = surakarta::bitboard::TrainingTerminalTarget(Color::None, Color::Black);
    const auto draw_for_white = surakarta::bitboard::TrainingTerminalTarget(Color::None, Color::White);

    auto okay = true;
    okay &= Expect(black_for_black > 0.0, "black win should be positive for black update");
    okay &= Expect(black_for_white < 0.0, "black win should be negative for white update");
    okay &= Expect(white_for_black < 0.0, "white win should be negative for black update");
    okay &= Expect(white_for_white > 0.0, "white win should be positive for white update");
    okay &= Expect(draw_for_black == 0.0, "draw should be zero for black update");
    okay &= Expect(draw_for_white == 0.0, "draw should be zero for white update");
    okay &= Expect(AlmostEqual(std::abs(black_for_black), std::abs(black_for_white)),
                   "black terminal reward magnitude should not depend on update side");
    okay &= Expect(AlmostEqual(std::abs(white_for_black), std::abs(white_for_white)),
                   "white terminal reward magnitude should not depend on update side");
    return okay;
}

bool TestTdBootstrapAndScoreSymmetry() {
    auto weights = SearchController{}.Evaluator().ExportWeights();
    auto root = LoadPositionFromFile("");
    auto flipped = root;
    flipped.side_to_move = static_cast<std::uint8_t>(surakarta::bitboard::Color::White);

    auto moves = surakarta::bitboard::MoveList{};
    GenerateMoves(root, moves);
    auto next = root;
    auto undo = Undo{};
    MakeMove(next, moves.moves[0], undo);

    auto traces = std::vector<double>(weights.values.size(), 0.0);
    auto context = surakarta::bitboard::TrainingStepContext{};
    context.next_has_legal_moves = true;
    context.next_is_ply_cap = false;
    auto step = surakarta::bitboard::TrainingStepResult{};

    auto okay = true;
    okay &= Expect(surakarta::bitboard::ApplyTrainingStep(weights, traces, root, next, context, 0.0, 0.0, &step),
                   "td audit step should apply with zero alpha");
    okay &= Expect(AlmostEqual(step.target_value, -surakarta::bitboard::EvaluateTrainingValueForSideToMove(weights, next)),
                   "non-terminal bootstrap target should negate next side-to-move value");
    okay &= Expect(surakarta::bitboard::EvaluateTrainingValueForSideToMove(weights, root) ==
                       -surakarta::bitboard::EvaluateTrainingValueForSideToMove(weights, flipped),
                   "training value should flip sign when only side-to-move flips");
    okay &= Expect(weights.Evaluate(root) == -weights.Evaluate(flipped),
                   "serialized weight evaluator should flip sign when only side-to-move flips");
    return okay;
}

bool TestTdTraceResetAndAlternatingPerspective() {
    auto weights = SearchController{}.Evaluator().ExportWeights();
    auto traces = std::vector<double>(weights.values.size(), 0.0);
    if (traces.size() < 3) {
        return Expect(false, "training weight vector should have trace slots");
    }
    traces[0] = 1.0;
    traces[1] = -2.0;
    traces[2] = 3.0;
    surakarta::bitboard::ResetTrainingTraces(traces);

    auto okay = true;
    okay &= Expect(std::all_of(traces.begin(), traces.end(), [](double value) { return value == 0.0; }),
                   "training traces should reset to zero");

    auto root = LoadPositionFromFile("");
    auto first_moves = surakarta::bitboard::MoveList{};
    GenerateMoves(root, first_moves);
    auto after_black = root;
    auto first_undo = Undo{};
    MakeMove(after_black, first_moves.moves[0], first_undo);

    auto context = surakarta::bitboard::TrainingStepContext{};
    auto first_step = surakarta::bitboard::TrainingStepResult{};
    okay &= Expect(surakarta::bitboard::ApplyTrainingStep(weights, traces, root, after_black, context, 0.0, 0.5, &first_step),
                   "first zero-alpha td step should succeed");
    const auto traces_after_first = traces;

    auto second_moves = surakarta::bitboard::MoveList{};
    GenerateMoves(after_black, second_moves);
    auto after_white = after_black;
    auto second_undo = Undo{};
    MakeMove(after_white, second_moves.moves[0], second_undo);

    auto active_second = std::vector<int>{};
    weights.EnumerateActiveWeightIndices(after_black, active_second);
    const auto second_feature_sign =
        after_black.SideToMove() == surakarta::bitboard::Color::Black ? 1.0 : -1.0;

    auto second_step = surakarta::bitboard::TrainingStepResult{};
    okay &= Expect(surakarta::bitboard::ApplyTrainingStep(weights, traces, after_black, after_white, context, 0.0, 0.5, &second_step),
                   "second zero-alpha td step should succeed");

    for (std::size_t i = 0; i < traces.size(); ++i) {
        auto expected = -0.5 * traces_after_first[i];
        if (std::find(active_second.begin(), active_second.end(), static_cast<int>(i)) != active_second.end()) {
            expected += second_feature_sign;
        }
        if (!AlmostEqual(traces[i], expected)) {
            okay &= Expect(false,
                           "td trace should decay with alternating perspective at index " + std::to_string(i) +
                               ": expected=" + std::to_string(expected) +
                               ", actual=" + std::to_string(traces[i]));
            break;
        }
    }
    return okay;
}

bool TestTdZeroAlphaAndSmallDeltaDirection() {
    using surakarta::bitboard::Color;

    auto weights = SearchController{}.Evaluator().ExportWeights();
    const auto before_zero_alpha = weights.values;
    auto root = LoadPositionFromFile("");
    const auto refresh_position = [](surakarta::bitboard::Position& position) {
        position.eval_cache = surakarta::bitboard::MaterialBalance(position);
        position.zobrist_key = surakarta::bitboard::ComputeZobrist(position);
    };

    auto terminal_after_black = root;
    terminal_after_black.side_to_move = static_cast<std::uint8_t>(Color::White);
    terminal_after_black.board.pieces[static_cast<int>(Color::White)] = 0;
    refresh_position(terminal_after_black);

    auto traces = std::vector<double>(weights.values.size(), 0.0);
    auto context = surakarta::bitboard::TrainingStepContext{};
    context.next_has_legal_moves = false;
    auto zero_step = surakarta::bitboard::TrainingStepResult{};

    auto okay = true;
    okay &= Expect(surakarta::bitboard::ApplyTrainingStep(weights, traces, root, terminal_after_black, context, 0.0, 0.0, &zero_step),
                   "zero-alpha terminal td step should succeed");
    okay &= Expect(weights.values == before_zero_alpha, "zero alpha should leave all weights unchanged");
    okay &= Expect(zero_step.td_error > 0.0, "black terminal win should produce positive td error from opening");

    auto active_indices = std::vector<int>{};
    weights.EnumerateActiveWeightIndices(root, active_indices);
    okay &= Expect(!active_indices.empty(), "lambda=0 locality test should have active black features");
    auto active_mask = std::vector<bool>(weights.values.size(), false);
    for (const auto index : active_indices) {
        if (index >= 0 && static_cast<std::size_t>(index) < active_mask.size()) {
            active_mask[static_cast<std::size_t>(index)] = true;
        }
    }

    auto stale_traces = std::vector<double>(weights.values.size(), 3.0);
    auto locality_step = surakarta::bitboard::TrainingStepResult{};
    okay &= Expect(surakarta::bitboard::ApplyTrainingStep(
                       weights, stale_traces, root, terminal_after_black, context, 0.0, 0.0, &locality_step),
                   "lambda=0 locality td step should succeed");
    const auto feature_sign = root.SideToMove() == Color::Black ? 1.0 : -1.0;
    for (std::size_t i = 0; i < stale_traces.size(); ++i) {
        const auto expected_trace = active_mask[i] ? feature_sign : 0.0;
        if (!AlmostEqual(stale_traces[i], expected_trace)) {
            okay &= Expect(false,
                           "lambda=0 should keep only current active trace at index " +
                               std::to_string(i));
            break;
        }
    }

    surakarta::bitboard::ResetTrainingTraces(traces);
    const auto before_small_alpha = weights.values;
    const auto before_black_value = surakarta::bitboard::EvaluateTrainingValueForSideToMove(weights, root);
    auto small_step = surakarta::bitboard::TrainingStepResult{};
    okay &= Expect(surakarta::bitboard::ApplyTrainingStep(weights, traces, root, terminal_after_black, context, 0.001, 0.0, &small_step),
                   "small-alpha terminal td step should succeed");
    okay &= Expect(small_step.td_error > 0.0, "small-alpha black win td error should stay positive");
    const auto after_black_value = surakarta::bitboard::EvaluateTrainingValueForSideToMove(weights, root);
    okay &= Expect(after_black_value > before_black_value,
                   "small-alpha black win should move black-perspective value toward positive target");
    for (const auto index : active_indices) {
        if (!(weights.values[static_cast<std::size_t>(index)] > before_small_alpha[static_cast<std::size_t>(index)])) {
            okay &= Expect(false,
                           "black-to-move positive td error should increase active black-perspective weight at index " +
                               std::to_string(index));
            break;
        }
    }

    auto white_weights = SearchController{}.Evaluator().ExportWeights();
    auto white_current = root;
    white_current.side_to_move = static_cast<std::uint8_t>(Color::White);
    refresh_position(white_current);
    auto terminal_after_white = white_current;
    terminal_after_white.side_to_move = static_cast<std::uint8_t>(Color::Black);
    terminal_after_white.board.pieces[static_cast<int>(Color::Black)] = 0;
    refresh_position(terminal_after_white);

    auto white_traces = std::vector<double>(white_weights.values.size(), 0.0);
    auto active_white_indices = std::vector<int>{};
    white_weights.EnumerateActiveWeightIndices(white_current, active_white_indices);
    const auto before_white_weights = white_weights.values;
    const auto before_white_value =
        surakarta::bitboard::EvaluateTrainingValueForSideToMove(white_weights, white_current);
    auto white_step = surakarta::bitboard::TrainingStepResult{};
    okay &= Expect(surakarta::bitboard::ApplyTrainingStep(
                       white_weights, white_traces, white_current, terminal_after_white, context, 0.001, 0.0, &white_step),
                   "small-alpha white terminal td step should succeed");
    okay &= Expect(white_step.td_error > 0.0, "white terminal win should produce positive td error for white update");
    const auto after_white_value =
        surakarta::bitboard::EvaluateTrainingValueForSideToMove(white_weights, white_current);
    okay &= Expect(after_white_value > before_white_value,
                   "small-alpha white win should move white-perspective value toward positive target");
    for (const auto index : active_white_indices) {
        if (!(white_weights.values[static_cast<std::size_t>(index)] <
              before_white_weights[static_cast<std::size_t>(index)])) {
            okay &= Expect(false,
                           "white-to-move positive td error should decrease raw active weight at index " +
                               std::to_string(index));
            break;
        }
    }
    return okay;
}

bool TestTdErrorClipLimitsAppliedUpdate() {
    using surakarta::bitboard::Color;

    const auto refresh_position = [](surakarta::bitboard::Position& position) {
        position.eval_cache = surakarta::bitboard::MaterialBalance(position);
        position.zobrist_key = surakarta::bitboard::ComputeZobrist(position);
    };

    auto root = LoadPositionFromFile("");
    auto terminal_after_black = root;
    terminal_after_black.side_to_move = static_cast<std::uint8_t>(Color::White);
    terminal_after_black.board.pieces[static_cast<int>(Color::White)] = 0;
    refresh_position(terminal_after_black);

    auto context = surakarta::bitboard::TrainingStepContext{};
    context.next_has_legal_moves = false;

    auto unclipped_weights = SearchController{}.Evaluator().ExportWeights();
    auto unclipped_traces = std::vector<double>(unclipped_weights.values.size(), 0.0);
    auto unclipped_step = surakarta::bitboard::TrainingStepResult{};

    auto clipped_weights = SearchController{}.Evaluator().ExportWeights();
    auto clipped_traces = std::vector<double>(clipped_weights.values.size(), 0.0);
    auto clipped_step = surakarta::bitboard::TrainingStepResult{};

    auto okay = true;
    okay &= Expect(surakarta::bitboard::ApplyTrainingStep(
                       unclipped_weights,
                       unclipped_traces,
                       root,
                       terminal_after_black,
                       context,
                       0.001,
                       0.0,
                       &unclipped_step,
                       1200.0,
                       0.0),
                   "unclipped terminal td step should succeed");
    okay &= Expect(surakarta::bitboard::ApplyTrainingStep(
                       clipped_weights,
                       clipped_traces,
                       root,
                       terminal_after_black,
                       context,
                       0.001,
                       0.0,
                       &clipped_step,
                       1200.0,
                       100.0),
                   "clipped terminal td step should succeed");

    okay &= Expect(unclipped_step.td_error > 100.0, "unclipped terminal td error should exceed clip");
    okay &= Expect(AlmostEqual(clipped_step.td_error, 100.0),
                   "td_error_clip should cap the applied td error");
    okay &= Expect(clipped_step.abs_weight_delta < unclipped_step.abs_weight_delta,
                   "td_error_clip should reduce the applied weight delta");
    return okay;
}


bool TestActiveObjectiveConfigSkeletonDefaultInvalidAndScopedConfig() {
    auto okay = true;
    const auto marker = std::string("Phase 3.41 config default invalid config scoped config skeleton marker");

    auto defaults = TrainingOptions{};
    okay &= Expect(marker.find("config default") != std::string::npos,
                   "Phase 3.41 config default marker should be present");
    okay &= Expect(!defaults.active_objective_config.config_present,
                   "Phase 3.41 config default should be absent");
    okay &= Expect(defaults.active_objective_config.default_off,
                   "Phase 3.41 config default should be default-off");
    okay &= Expect(!defaults.active_objective_config.skeleton_enabled,
                   "Phase 3.41 config default should not enable skeleton");
    okay &= Expect(!defaults.active_objective_config.active_objective_probe_executed,
                   "Phase 3.41 config default should not execute active objective probe");
    okay &= Expect(!defaults.active_objective_config.selection_gate_eligible,
                   "Phase 3.41 config default should not be selection gate eligible");

    auto invalid_config = surakarta::bitboard::ActiveObjectiveConfigSkeleton{};
    auto invalid_error = std::string{};
    okay &= Expect(!surakarta::bitboard::ParseActiveObjectiveConfigSkeleton(
                       "{\"schema_version\":1,\"enabled\":true,\"mode\":\"active_scoped\"}",
                       &invalid_config,
                       &invalid_error),
                   "Phase 3.41 invalid config should be rejected");
    okay &= Expect(invalid_config.config_present,
                   "Phase 3.41 invalid config should still record config presence");
    okay &= Expect(!invalid_config.config_valid,
                   "Phase 3.41 invalid config should not be valid");
    okay &= Expect(!invalid_config.active_objective_probe_executed,
                   "Phase 3.41 invalid config should not execute active objective probe");
    okay &= Expect(!invalid_error.empty(),
                   "Phase 3.41 invalid config should explain rejection");

    auto rejected_options = TrainingOptions{};
    rejected_options.active_objective_config = invalid_config;
    rejected_options.games = 1;
    auto rejected_summary = surakarta::bitboard::TrainingSummary{};
    auto rejected_error = std::string{};
    okay &= Expect(!surakarta::bitboard::RunBitboardTraining(
                       rejected_options, &rejected_summary, &rejected_error),
                   "Phase 4.2F hard reject should return false");
    okay &= Expect(rejected_summary.active_interface_config_present,
                   "Phase 4.2F hard reject should preserve config presence in summary");
    okay &= Expect(!rejected_summary.active_interface_config_valid,
                   "Phase 4.2F hard reject should report invalid config in summary");
    okay &= Expect(rejected_summary.active_interface_config_status == "active_objective_config_rejected",
                   "Phase 4.2F hard reject should report rejection status in summary");
    okay &= Expect(rejected_summary.active_interface_reject_reason == invalid_config.reject_reason,
                   "Phase 4.2F hard reject should report rejection reason in summary");
    okay &= Expect(!rejected_summary.active_objective_probe_executed,
                   "Phase 4.2F hard reject should not execute objective diagnostics");
    okay &= Expect(!rejected_summary.selection_gate_eligible,
                   "Phase 4.2F hard reject should remain not selection eligible");
    okay &= Expect(rejected_summary.games_completed == 0,
                   "Phase 4.2F hard reject should not run training games");
    okay &= Expect(rejected_summary.output_weights_path.empty(),
                   "Phase 4.2F hard reject should not report weight output");
    okay &= Expect(rejected_error.find(invalid_config.reject_reason) != std::string::npos,
                   "Phase 4.2F hard reject error should include rejection reason");

    const auto scoped_config_text =
        std::string("{") +
        "\"schema_version\":1,"
        "\"enabled\":true,"
        "\"mode\":\"active_scoped\","
        "\"scope\":\"opening_root_children_only\","
        "\"report_only\":true,"
        "\"probe_only\":true,"
        "\"no_output_weights\":true,"
        "\"selection_gate_eligible\":false,"
        "\"opening_drift_penalty_weight\":0.005,"
        "\"max_games\":4,"
        "\"max_depth\":4,"
        "\"seed_allowlist\":[20260423]"
        "}";
    auto scoped_config = surakarta::bitboard::ActiveObjectiveConfigSkeleton{};
    auto scoped_error = std::string{};
    okay &= Expect(surakarta::bitboard::ParseActiveObjectiveConfigSkeleton(
                       scoped_config_text, &scoped_config, &scoped_error),
                   "Phase 3.41 valid scoped config should enter skeleton");
    okay &= Expect(scoped_config.config_present && scoped_config.config_valid,
                   "Phase 3.41 scoped config should be present and valid");
    okay &= Expect(scoped_config.skeleton_enabled,
                   "Phase 3.41 scoped config should enable interface skeleton");
    okay &= Expect(!scoped_config.active_objective_probe_executed,
                   "Phase 3.41 scoped config should not execute active objective probe");
    okay &= Expect(!scoped_config.selection_gate_eligible,
                   "Phase 3.41 scoped config should not be selection gate eligible");
    okay &= Expect(scoped_config.no_output_probe_mode,
                   "Phase 3.47 no output mode should be parsed");
    okay &= Expect(scoped_config.weight_artifact_suppressed,
                   "Phase 3.47 weight suppressed marker should be parsed");

    auto skeleton_options = TrainingOptions{};
    skeleton_options.active_objective_config = scoped_config;
    skeleton_options.games = 1;
    skeleton_options.limits.max_depth = 1;
    auto skeleton_summary = surakarta::bitboard::TrainingSummary{};
    auto skeleton_error = std::string{};
    okay &= Expect(surakarta::bitboard::RunBitboardTraining(skeleton_options, &skeleton_summary, &skeleton_error),
                   "Phase 3.47 no output wiring skeleton should not require output weights");
    okay &= Expect(skeleton_summary.active_interface_no_output_probe_mode,
                   "Phase 3.47 no output mode should be reported");
    okay &= Expect(skeleton_summary.active_interface_weight_artifact_suppressed,
                   "Phase 3.47 weight suppressed state should be reported");
    okay &= Expect(skeleton_summary.active_interface_report_only_probe_path,
                   "Phase 3.52 report-only active probe path should be implemented");
    okay &= Expect(skeleton_summary.active_interface_config_status ==
                       "report_only_objective_diagnostic_executed_no_output",
                   "Phase 3.56 guarded report-only path should report objective diagnostic evidence");
    okay &= Expect(skeleton_summary.output_weights_path.empty(),
                   "Phase 3.47 no output mode should suppress weight artifact path");
    okay &= Expect(skeleton_summary.games_completed == 0,
                   "Phase 3.47 no output skeleton should not run training games");
    okay &= Expect(skeleton_summary.active_objective_probe_executed,
                   "Phase 3.56 guarded no-output path should execute report-only objective diagnostic evidence");
    okay &= Expect(!skeleton_summary.selection_gate_eligible,
                   "Phase 3.47 no output skeleton should remain not selection eligible");

    return okay;
}
bool TestBitboardTrainingTraceCli() {
    TempDirGuard temp_dir("surakarta-trace-cli");
    const auto benchmark = BenchmarkExecutablePath();
    const auto command = QuoteCommandArg(benchmark.string()) +
                         " bitboard-trace --games 2 --seed 20260423 --depth 4 --alpha 0.005 --lambda 0.5"
                         " --epsilon 0.02 --epsilon-plies 6 --terminal-reward 500 --td-error-clip 250"
                         " --format json";
    std::string output;
    const int exit_code = RunCommandCapture(command, &output);

    auto okay = true;
    okay &= Expect(exit_code == 0, "bitboard-trace should exit successfully");
    okay &= Expect(!output.empty(), "bitboard-trace should emit json output");
    okay &= Expect(output.front() == '{', "bitboard-trace should emit a JSON object");
    okay &= Expect(JsonHasField(output, "games"), "bitboard-trace json should include games");
    okay &= Expect(JsonHasField(output, "ply"), "bitboard-trace json should include ply");
    okay &= Expect(JsonHasField(output, "value_before"), "bitboard-trace json should include value_before");
    okay &= Expect(JsonHasField(output, "bootstrap_value"), "bitboard-trace json should include bootstrap_value");
    okay &= Expect(JsonHasField(output, "trace_norm"), "bitboard-trace json should include trace_norm");
    okay &= Expect(JsonHasField(output, "weight_delta_norm"), "bitboard-trace json should include weight_delta_norm");
    okay &= Expect(JsonHasField(output, "terminal_reason"), "bitboard-trace json should include terminal_reason");
    okay &= Expect(JsonHasField(output, "terminal_reward"), "bitboard-trace json should include terminal_reward");
    okay &= Expect(JsonHasField(output, "td_error_clip"), "bitboard-trace json should include td_error_clip");
    okay &= Expect(JsonHasField(output, "terminal_only_warmup"), "bitboard-trace json should include terminal_only_warmup");
    okay &= Expect(JsonHasField(output, "near_terminal_curriculum"), "bitboard-trace json should include near_terminal_curriculum");

    const auto csv_command = QuoteCommandArg(benchmark.string()) +
                             " bitboard-trace --games 1 --seed 20260423 --depth 4 --alpha 0.005 --lambda 0.5"
                             " --epsilon 0.02 --epsilon-plies 6 --terminal-reward 500 --td-error-clip 250"
                             " --format csv";
    std::string csv_output;
    const int csv_exit_code = RunCommandCapture(csv_command, &csv_output);
    okay &= Expect(csv_exit_code == 0, "bitboard-trace csv should exit successfully");
    okay &= Expect(csv_output.find("terminal_reward") != std::string::npos,
                   "bitboard-trace csv should include terminal_reward");
    okay &= Expect(csv_output.find("td_error_clip") != std::string::npos,
                   "bitboard-trace csv should include td_error_clip");
    return okay;
}

bool TestBitboardTrainingJsonIncludesStabilizationParameters() {
    TempDirGuard temp_dir("surakarta-train-json-params");
    const auto benchmark = BenchmarkExecutablePath();
    const auto output_weights = temp_dir.path / "candidate.bin";
    const auto command = QuoteCommandArg(benchmark.string()) +
                         " bitboard-train --output " + QuoteCommandArg(output_weights.string()) +
                         " --games 0 --alpha 0.005 --lambda 0.5 --epsilon 0.02 --epsilon-plies 6"
                         " --terminal-reward 500 --td-error-clip 250 --format json";
    std::string output;
    const int exit_code = RunCommandCapture(command, &output);

    auto okay = true;
    okay &= Expect(exit_code == 0, "bitboard-train --format json should exit successfully");
    okay &= Expect(!output.empty(), "bitboard-train should emit json output");
    okay &= Expect(output.front() == '{', "bitboard-train should emit a JSON object");
    okay &= Expect(JsonHasField(output, "terminal_reward"), "bitboard-train json should include terminal_reward");
    okay &= Expect(JsonHasField(output, "td_error_clip"), "bitboard-train json should include td_error_clip");
    okay &= Expect(JsonHasField(output, "terminal_only_warmup"), "bitboard-train json should include terminal_only_warmup");
    okay &= Expect(JsonHasField(output, "near_terminal_curriculum"), "bitboard-train json should include near_terminal_curriculum");
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
    okay &= Expect(first_summary.terminal_checkmate + first_summary.terminal_no_capture_limit +
                       first_summary.terminal_no_legal_move + first_summary.terminal_ply_cap ==
                   first_summary.games_completed,
                   "training terminal reason counts should cover completed games");
    okay &= Expect(first_summary.terminal_checkmate == second_summary.terminal_checkmate,
                   "deterministic training should reproduce checkmate terminal count");
    okay &= Expect(first_summary.terminal_no_capture_limit == second_summary.terminal_no_capture_limit,
                   "deterministic training should reproduce no-capture terminal count");
    okay &= Expect(first_summary.terminal_no_legal_move == second_summary.terminal_no_legal_move,
                   "deterministic training should reproduce no-legal-move terminal count");
    okay &= Expect(first_summary.terminal_ply_cap == second_summary.terminal_ply_cap,
                   "deterministic training should reproduce ply-cap terminal count");
    okay &= Expect(first_summary.checkpoint_count == second_summary.checkpoint_count,
                   "deterministic training should reproduce checkpoint count");
    okay &= Expect(first_summary.checkpoint_summaries.size() == 2,
                   "checkpoint summaries should include one entry per saved checkpoint");
    okay &= Expect(first_summary.checkpoint_summaries.size() == second_summary.checkpoint_summaries.size(),
                   "deterministic training should reproduce checkpoint summary count");
    if (first_summary.checkpoint_summaries.size() == second_summary.checkpoint_summaries.size() &&
        !first_summary.checkpoint_summaries.empty()) {
        for (std::size_t i = 0; i < first_summary.checkpoint_summaries.size(); ++i) {
            const auto& first_checkpoint = first_summary.checkpoint_summaries[i];
            const auto& second_checkpoint = second_summary.checkpoint_summaries[i];
            okay &= Expect(std::filesystem::exists(first_checkpoint.path),
                           "checkpoint summary path should point to a saved checkpoint");
            okay &= Expect(first_checkpoint.games_completed == second_checkpoint.games_completed,
                           "checkpoint summary should reproduce completed games");
            okay &= Expect(first_checkpoint.games_completed == static_cast<int>(i + 1),
                           "checkpoint summary should record checkpoint game count");
            okay &= Expect(first_checkpoint.black_wins + first_checkpoint.white_wins + first_checkpoint.draws ==
                           first_checkpoint.games_completed,
                           "checkpoint summary outcomes should cover completed games");
            okay &= Expect(first_checkpoint.terminal_checkmate + first_checkpoint.terminal_no_capture_limit +
                               first_checkpoint.terminal_no_legal_move + first_checkpoint.terminal_ply_cap ==
                           first_checkpoint.games_completed,
                           "checkpoint summary terminal reasons should cover completed games");
            okay &= Expect(std::abs(first_checkpoint.average_abs_td_error -
                                    second_checkpoint.average_abs_td_error) < 1e-9,
                           "checkpoint summary should reproduce average td error");
            okay &= Expect(std::abs(first_checkpoint.max_abs_weight_delta -
                                    second_checkpoint.max_abs_weight_delta) < 1e-9,
                           "checkpoint summary should reproduce max weight delta");
        }
    }
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

bool TestNearTerminalCurriculumUsesShortTerminalSamples() {
    TempDirGuard temp_dir("surakarta-near-terminal-curriculum");
    const auto candidate_path = temp_dir.path / "candidate.bin";

    auto options = TrainingOptions{};
    options.output_weights_path = candidate_path.string();
    options.games = 4;
    options.seed = 20260423;
    options.limits.max_depth = 1;
    options.alpha = 0.001;
    options.lambda = 0.0;
    options.epsilon = 0.0;
    options.terminal_reward = 500.0;
    options.near_terminal_curriculum = 4;

    auto summary = TrainingSummary{};
    auto error_message = std::string{};
    auto okay = true;
    okay &= Expect(surakarta::bitboard::RunBitboardTraining(options, &summary, &error_message),
                   "near-terminal curriculum training should succeed: " + error_message);
    okay &= Expect(summary.games_completed == 4, "near-terminal curriculum should complete requested games");
    okay &= Expect(summary.terminal_checkmate == 4, "near-terminal curriculum should finish every sample by checkmate");
    okay &= Expect(summary.terminal_no_capture_limit == 0, "near-terminal curriculum should not rely on no-capture draws");
    okay &= Expect(summary.terminal_no_legal_move == 0, "near-terminal curriculum should not rely on no-legal terminals");
    okay &= Expect(summary.terminal_ply_cap == 0, "near-terminal curriculum should not hit the ply cap");
    okay &= Expect(summary.positions_evaluated == 4, "near-terminal curriculum should use one TD step per sample");
    okay &= Expect(summary.update_count == 4, "near-terminal curriculum should update once per sample");
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

bool TestCaptureVariantCapacityInvariant() {
    auto generated = surakarta::bitboard::BitboardTables{};
    try {
        generated = surakarta::bitboard::BitboardGenerator().Build();
    } catch (const std::exception& ex) {
        return Expect(false, std::string("BitboardGenerator::Build should not exceed kMaxCaptureVariants: ") + ex.what());
    }

    auto okay = true;
    auto max_generated_count = 0;
    auto max_static_count = 0;
    auto max_generated_from = surakarta::bitboard::Square{0};
    auto max_generated_to = surakarta::bitboard::Square{0};
    auto max_static_from = surakarta::bitboard::Square{0};
    auto max_static_to = surakarta::bitboard::Square{0};
    const auto& tables = surakarta::bitboard::GetBitboardTables();

    for (surakarta::bitboard::Square from = 0; from < surakarta::bitboard::kSquareCount; ++from) {
        for (surakarta::bitboard::Square to = 0; to < surakarta::bitboard::kSquareCount; ++to) {
            const auto generated_count = static_cast<int>(generated.capture_variant_count[from][to]);
            const auto static_count = static_cast<int>(tables.capture_variant_count[from][to]);
            if (generated_count > max_generated_count) {
                max_generated_count = generated_count;
                max_generated_from = from;
                max_generated_to = to;
            }
            if (static_count > max_static_count) {
                max_static_count = static_count;
                max_static_from = from;
                max_static_to = to;
            }
            okay &= Expect(generated_count <= surakarta::bitboard::kMaxCaptureVariants,
                           "generated capture variant count exceeds capacity at " + SquareLabel(from) +
                               " -> " + SquareLabel(to));
            okay &= Expect(static_count <= surakarta::bitboard::kMaxCaptureVariants,
                           "static capture variant count exceeds capacity at " + SquareLabel(from) +
                               " -> " + SquareLabel(to));
        }
    }

    okay &= Expect(max_generated_count == 8,
                   "current generated max capture variants should be 8 at " + SquareLabel(max_generated_from) +
                       " -> " + SquareLabel(max_generated_to) + ", actual: " +
                       std::to_string(max_generated_count));
    okay &= Expect(max_static_count == 8,
                   "current static max capture variants should be 8 at " + SquareLabel(max_static_from) +
                       " -> " + SquareLabel(max_static_to) + ", actual: " +
                       std::to_string(max_static_count));
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
    std::cerr << "[TEST] TestIllegalCaptureVariantBlocked" << std::endl;
    ok &= TestIllegalCaptureVariantBlocked();
    std::cerr << "[TEST] TestQuietMoveGoldenStillWorks" << std::endl;
    ok &= TestQuietMoveGoldenStillWorks();
    std::cerr << "[TEST] TestScreenshotNoStraightLineCaptureInvariant" << std::endl;
    ok &= TestScreenshotNoStraightLineCaptureInvariant();
    std::cerr << "[TEST] TestAllGeneratedCapturesHaveLoopPaths" << std::endl;
    ok &= TestAllGeneratedCapturesHaveLoopPaths();
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
    std::cerr << "[TEST] TestTdTerminalRewardSigns" << std::endl;
    ok &= TestTdTerminalRewardSigns();
    std::cerr << "[TEST] TestTdBootstrapAndScoreSymmetry" << std::endl;
    ok &= TestTdBootstrapAndScoreSymmetry();
    std::cerr << "[TEST] TestTdTraceResetAndAlternatingPerspective" << std::endl;
    ok &= TestTdTraceResetAndAlternatingPerspective();
    std::cerr << "[TEST] TestTdZeroAlphaAndSmallDeltaDirection" << std::endl;
    ok &= TestTdZeroAlphaAndSmallDeltaDirection();
    std::cerr << "[TEST] TestTdErrorClipLimitsAppliedUpdate" << std::endl;
    ok &= TestTdErrorClipLimitsAppliedUpdate();
    std::cerr << "[TEST] TestActiveObjectiveConfigSkeletonDefaultInvalidAndScopedConfig" << std::endl;
    ok &= TestActiveObjectiveConfigSkeletonDefaultInvalidAndScopedConfig();
    std::cerr << "[TEST] TestBitboardTrainingTraceCli" << std::endl;
    ok &= TestBitboardTrainingTraceCli();
    std::cerr << "[TEST] TestBitboardTrainingJsonIncludesStabilizationParameters" << std::endl;
    ok &= TestBitboardTrainingJsonIncludesStabilizationParameters();
    std::cerr << "[TEST] TestTrainingReproducibility" << std::endl;
    ok &= TestTrainingReproducibility();
    std::cerr << "[TEST] TestNearTerminalCurriculumUsesShortTerminalSamples" << std::endl;
    ok &= TestNearTerminalCurriculumUsesShortTerminalSamples();
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
