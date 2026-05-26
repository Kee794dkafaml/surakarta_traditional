#include <algorithm>
#include <chrono>
#include <cmath>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "surakarta.h"

#define ANSI_CLEAR_SCREEN "\033[2J"
#define ANSI_MOVE_TO_START "\033[H"

#define WIN_MIME 1
#define WIN_RANDOM 2
#define STALEMATE 3

class ExposiveSurakartaDaemon : public SurakartaDaemon {
   public:
    ExposiveSurakartaDaemon(
        int board_size,
        int max_no_capture_round,
        std::shared_ptr<AgentFactory> black_agent_factory,
        std::shared_ptr<AgentFactory> white_agent_factory)
        : SurakartaDaemon(board_size, max_no_capture_round, black_agent_factory, white_agent_factory) {}

    std::shared_ptr<SurakartaGameInfo> GameInfo() {
        return game_.GetGameInfo();
    }

    std::shared_ptr<SurakartaBoard> Board() {
        return game_.GetBoard();
    }
};

int play(int miliseconds = 50,
         bool display = true,
         int depth = SurakartaMoveWeightUtil::DefaultDepth,
         double alpha = SurakartaMoveWeightUtil::DefaultAlpha,
         double beta = SurakartaMoveWeightUtil::DefaultBeta) {
    const auto move_weight_util_factory = std::make_shared<SurakartaAgentMineFactory::SurakartaMoveWeightUtilFactory>(depth, alpha, beta);
    const auto agent_factory_mine = std::make_shared<SurakartaAgentMineFactory>(move_weight_util_factory);
    const auto agent_factory_random = std::make_shared<SurakartaAgentRandomFactory>();
    const auto my_colour = GlobalRandomGenerator().getInstance()() % 2 ? PieceColor::BLACK : PieceColor::WHITE;
    const auto agent_factory_black = my_colour == PieceColor::BLACK ? (std::shared_ptr<SurakartaDaemon::AgentFactory>)agent_factory_mine : agent_factory_random;
    const auto agent_factory_white = my_colour == PieceColor::WHITE ? (std::shared_ptr<SurakartaDaemon::AgentFactory>)agent_factory_mine : agent_factory_random;
    auto daemon = ExposiveSurakartaDaemon(BOARD_SIZE, MAX_NO_CAPTURE_ROUND, agent_factory_black, agent_factory_white);

    const auto black_pieces = std::make_shared<std::vector<SurakartaPositionWithId>>();
    const auto white_pieces = std::make_shared<std::vector<SurakartaPositionWithId>>();
    bool piece_lists_initialized = false;
    SurakartaOnBoardUpdateUtil on_board_update_util(black_pieces, white_pieces, daemon.Board());

    daemon.OnUpdateBoard.AddListener([&]() {
        if (!piece_lists_initialized) {
            const auto lists = SurakartaInitPositionListsUtil(daemon.Board()).InitPositionList();
            *black_pieces = *lists.black_list;
            *white_pieces = *lists.white_list;
            piece_lists_initialized = true;
        }
        const auto opt_trace = on_board_update_util.UpdateAndGetTrace();
        if (opt_trace.has_value()) {
            PieceColor moved_colour = PieceColor::NONE;
            for (auto& item : *black_pieces) {
                if (item.id == opt_trace->moved_piece.id)
                    moved_colour = PieceColor::BLACK;
            }
            for (auto& item : *white_pieces) {
                if (item.id == opt_trace->moved_piece.id)
                    moved_colour = PieceColor::WHITE;
            }
            if (moved_colour == PieceColor::NONE)
                throw std::runtime_error("moved piece not found in black_pieces or white_pieces");
            const auto guard = opt_trace.value().is_capture ? SurakartaTemporarilyChangeColorGuardUtil(
                                                                  daemon.Board(),
                                                                  SurakartaPosition(
                                                                      opt_trace.value().captured_piece.x,
                                                                      opt_trace.value().captured_piece.y),
                                                                  ReverseColor(moved_colour))
                                                            : SurakartaTemporarilyChangeColorGuardUtil();
            for (auto& fragment : opt_trace.value().path) {
                SurakartaTemporarilyChangeColorGuardUtil guard1(daemon.Board(), fragment.From(), PieceColor::NONE);
                SurakartaTemporarilyChangeColorGuardUtil guard2(daemon.Board(), fragment.To(), moved_colour);
                std::this_thread::sleep_for(std::chrono::milliseconds(
                    fragment.is_curve ? miliseconds * 2 : 0));

                if (display) {
                    std::cout << ANSI_CLEAR_SCREEN << ANSI_MOVE_TO_START;
                    std::cout << "B: " << (my_colour == PieceColor::BLACK ? "Mine" : "Random") << std::endl;
                    std::cout << "W: " << (my_colour == PieceColor::WHITE ? "Mine" : "Random") << std::endl;
                    std::cout << std::endl;
                    std::cout << *daemon.Board() << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
            }
        }
    });

    daemon.Execute();

    const bool is_stalemate = daemon.GameInfo()->Winner() == SurakartaPlayer::NONE;
    const bool has_win = !((daemon.GameInfo()->Winner() == SurakartaPlayer::BLACK) ^ (my_colour == PieceColor::BLACK));
    return is_stalemate ? STALEMATE : (has_win ? WIN_MIME : WIN_RANDOM);
}

void LoadBitboardPosition(const std::string& file_name, surakarta::bitboard::Position& position) {
    SurakartaGame game;
    if (file_name.empty()) {
        game.StartGame();
    } else {
        game.StartGame(file_name);
    }
    position = surakarta::bitboard::PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());
}

enum class TraceOutputFormat {
    Json,
    Csv,
};

enum class OutputFormat {
    Text,
    Json,
};

struct CliRootMoveDiagnostic {
    int depth{0};
    int attempt{0};
    int move_index{0};
    std::string move;
    int alpha{0};
    int beta{0};
    int score{0};
    int bound{0};
    std::uint64_t nodes{0};
    std::uint64_t qnodes{0};
    std::uint64_t fail_highs{0};
    std::uint64_t fail_lows{0};
    std::uint64_t tt_hits{0};
    std::uint64_t beta_cutoffs{0};
    std::uint64_t qsearch_entries{0};
    std::uint64_t qsearch_capture_moves{0};
    std::uint64_t root_best_updates{0};
    std::string pv;
};

struct CliAspirationDiagnostic {
    int depth{0};
    int attempt{0};
    int alpha{0};
    int beta{0};
    int score{0};
    std::string outcome;
    std::uint64_t nodes{0};
    std::uint64_t qnodes{0};
    std::uint64_t root_best_updates{0};
    std::uint64_t fail_highs{0};
    std::uint64_t fail_lows{0};
};

struct CliQsearchDiagnostic {
    int depth{0};
    int attempt{0};
    int root_move_index{0};
    std::string root_move;
    int ply{0};
    int score{0};
    std::uint64_t qnodes{0};
    std::uint64_t capture_moves{0};
    std::string line;
};

struct CliSearchDiagnostics {
    std::vector<CliRootMoveDiagnostic> root_moves{};
    std::vector<CliAspirationDiagnostic> aspiration_attempts{};
    std::vector<CliQsearchDiagnostic> qsearch_sources{};
};

struct CliSearchReport {
    std::string case_id;
    int depth{0};
    int threads{0};
    double search_seconds{0.0};
    std::uint64_t nodes{0};
    std::uint64_t qnodes{0};
    std::uint64_t nps{0};
    std::uint64_t tt_hits{0};
    std::uint64_t null_prunes{0};
    std::uint64_t lmr_reductions{0};
    std::uint64_t fail_highs{0};
    std::uint64_t fail_lows{0};
    std::uint64_t aspiration_retries{0};
    std::uint64_t aspiration_attempts{0};
    std::uint64_t full_window_researches{0};
    std::uint64_t root_best_updates{0};
    std::uint64_t tt_exact_cutoffs{0};
    std::uint64_t tt_bound_cutoffs{0};
    std::uint64_t beta_cutoffs{0};
    std::uint64_t stale_root_tasks{0};
    std::uint64_t aborted_root_tasks{0};
    std::uint64_t worker_idle_spins{0};
    std::uint64_t thread_spawn_count{0};
    std::string pv;
    std::string best_move;
    int score{0};
    bool has_movegen{false};
    std::uint64_t movegen_nodes{0};
    double movegen_seconds{0.0};
    std::uint64_t movegen_nps{0};
    CliSearchDiagnostics diagnostics{};
};

struct BitboardTraceStep {
    int game_index{0};
    int ply{0};
    std::string side_to_move;
    double reward{0.0};
    double value_before{0.0};
    double value_after{0.0};
    double bootstrap_value{0.0};
    double td_error{0.0};
    double trace_norm{0.0};
    double weight_delta_norm{0.0};
    std::string terminal_reason;
};

struct BitboardTraceReport {
    std::uint32_t seed{0};
    int games_requested{0};
    int depth{4};
    double alpha{0.005};
    double lambda{0.5};
    double epsilon{0.02};
    int epsilon_plies{6};
    double terminal_reward{1200.0};
    double td_error_clip{0.0};
    int terminal_only_warmup{0};
    int near_terminal_curriculum{0};
    std::vector<BitboardTraceStep> steps{};
};

struct BitboardTraceOptions {
    int games{1};
    int depth{4};
    double alpha{0.005};
    double lambda{0.5};
    double epsilon{0.02};
    int epsilon_plies{6};
    double terminal_reward{1200.0};
    double td_error_clip{0.0};
    int terminal_only_warmup{0};
    int near_terminal_curriculum{0};
    std::uint32_t seed{0};
    std::string weights_file;
    TraceOutputFormat format{TraceOutputFormat::Json};
};

std::string EscapeJsonString(const std::string& value) {
    auto escaped = std::string{};
    escaped.reserve(value.size() + 8);
    for (const auto ch : value) {
        switch (ch) {
            case '\\':
                escaped += "\\\\";
                break;
            case '"':
                escaped += "\\\"";
                break;
            case '\n':
                escaped += "\\n";
                break;
            case '\r':
                escaped += "\\r";
                break;
            case '\t':
                escaped += "\\t";
                break;
            default:
                escaped += ch;
                break;
        }
    }
    return escaped;
}

std::string FormatDouble(double value) {
    auto stream = std::ostringstream{};
    stream << std::fixed << std::setprecision(6) << value;
    return stream.str();
}

std::string QuoteCommandPath(const std::filesystem::path& path) {
    return "\"" + path.string() + "\"";
}

int RunBitboardSelftestCommand(const char* argv0) {
    const auto benchmark_path = std::filesystem::absolute(std::filesystem::path(argv0));
    const auto selftest_path = benchmark_path.parent_path() / "surakarta-bitboard-selftest.exe";
    if (!std::filesystem::exists(selftest_path)) {
        std::cerr << "bitboard-selftest failed: " << selftest_path.string() << " not found" << std::endl;
        return 1;
    }
    return std::system(QuoteCommandPath(selftest_path).c_str());
}

const char* BoolJson(bool value) {
    return value ? "true" : "false";
}

TraceOutputFormat ParseTraceOutputFormat(const std::string& value) {
    return value == "csv" ? TraceOutputFormat::Csv : TraceOutputFormat::Json;
}

std::string BitboardColorName(surakarta::bitboard::Color color) {
    switch (color) {
        case surakarta::bitboard::Color::Black:
            return "black";
        case surakarta::bitboard::Color::White:
            return "white";
        default:
            return "none";
    }
}

bool IsTraceTerminalPosition(const surakarta::bitboard::Position& position) {
    const auto side = position.SideToMove();
    const auto enemy = side == surakarta::bitboard::Color::Black
                           ? surakarta::bitboard::Color::White
                           : surakarta::bitboard::Color::Black;
    return position.board.Count(side) == 0 ||
           position.board.Count(enemy) == 0 ||
           position.no_capture_ply >= position.max_no_capture_round;
}

std::string TraceTerminalReason(const surakarta::bitboard::Position& next,
                                const surakarta::bitboard::TrainingStepContext& context) {
    if (next.no_capture_ply >= next.max_no_capture_round) {
        return "no_capture_limit";
    }
    if (next.board.Count(surakarta::bitboard::Color::Black) == 0 ||
        next.board.Count(surakarta::bitboard::Color::White) == 0) {
        return "terminal";
    }
    if (!context.next_has_legal_moves) {
        return "no_legal_move";
    }
    if (context.next_is_ply_cap) {
        return "ply_cap";
    }
    return "none";
}

double L2Norm(const std::vector<double>& values) {
    auto squared = 0.0;
    for (const auto value : values) {
        squared += value * value;
    }
    return std::sqrt(squared);
}

double L2DeltaNorm(const std::vector<double>& before, const std::vector<double>& after) {
    const auto size = std::min(before.size(), after.size());
    auto squared = 0.0;
    for (std::size_t i = 0; i < size; ++i) {
        const auto delta = after[i] - before[i];
        squared += delta * delta;
    }
    return std::sqrt(squared);
}

void PrintBitboardTraceReport(const BitboardTraceReport& report, TraceOutputFormat format) {
    if (format == TraceOutputFormat::Csv) {
        std::cout << "seed,games_requested,depth,alpha,lambda,epsilon,epsilon_plies,"
                     "terminal_reward,td_error_clip,terminal_only_warmup,near_terminal_curriculum,"
                     "game,ply,side_to_move,reward,value_before,value_after,bootstrap_value,"
                     "td_error,trace_norm,weight_delta_norm,terminal_reason"
                  << std::endl;
        for (const auto& step : report.steps) {
            std::cout << report.seed << ","
                      << report.games_requested << ","
                      << report.depth << ","
                      << FormatDouble(report.alpha) << ","
                      << FormatDouble(report.lambda) << ","
                      << FormatDouble(report.epsilon) << ","
                      << report.epsilon_plies << ","
                      << FormatDouble(report.terminal_reward) << ","
                      << FormatDouble(report.td_error_clip) << ","
                      << report.terminal_only_warmup << ","
                      << report.near_terminal_curriculum << ","
                      << step.game_index << ","
                      << step.ply << ","
                      << step.side_to_move << ","
                      << FormatDouble(step.reward) << ","
                      << FormatDouble(step.value_before) << ","
                      << FormatDouble(step.value_after) << ","
                      << FormatDouble(step.bootstrap_value) << ","
                      << FormatDouble(step.td_error) << ","
                      << FormatDouble(step.trace_norm) << ","
                      << FormatDouble(step.weight_delta_norm) << ","
                      << step.terminal_reason << std::endl;
        }
        return;
    }

    std::cout << "{"
              << "\"seed\":" << report.seed << ","
              << "\"games\":" << report.games_requested << ","
              << "\"depth\":" << report.depth << ","
              << "\"alpha\":" << FormatDouble(report.alpha) << ","
              << "\"lambda\":" << FormatDouble(report.lambda) << ","
              << "\"epsilon\":" << FormatDouble(report.epsilon) << ","
              << "\"epsilon_plies\":" << report.epsilon_plies << ","
              << "\"terminal_reward\":" << FormatDouble(report.terminal_reward) << ","
              << "\"td_error_clip\":" << FormatDouble(report.td_error_clip) << ","
              << "\"terminal_only_warmup\":" << report.terminal_only_warmup << ","
              << "\"near_terminal_curriculum\":" << report.near_terminal_curriculum << ","
              << "\"steps\":[";
    for (std::size_t i = 0; i < report.steps.size(); ++i) {
        const auto& step = report.steps[i];
        if (i > 0) {
            std::cout << ",";
        }
        std::cout << "{"
                  << "\"game\":" << step.game_index << ","
                  << "\"ply\":" << step.ply << ","
                  << "\"side_to_move\":\"" << EscapeJsonString(step.side_to_move) << "\","
                  << "\"reward\":" << FormatDouble(step.reward) << ","
                  << "\"value_before\":" << FormatDouble(step.value_before) << ","
                  << "\"value_after\":" << FormatDouble(step.value_after) << ","
                  << "\"bootstrap_value\":" << FormatDouble(step.bootstrap_value) << ","
                  << "\"td_error\":" << FormatDouble(step.td_error) << ","
                  << "\"trace_norm\":" << FormatDouble(step.trace_norm) << ","
                  << "\"weight_delta_norm\":" << FormatDouble(step.weight_delta_norm) << ","
                  << "\"terminal_reason\":\"" << EscapeJsonString(step.terminal_reason) << "\""
                  << "}";
    }
    std::cout << "]}" << std::endl;
}

std::string DeriveCaseId(const std::string& explicit_case_id, const std::string& file_name) {
    if (!explicit_case_id.empty()) {
        return explicit_case_id;
    }
    if (file_name.empty()) {
        return "opening";
    }
    return std::filesystem::path(file_name).stem().string();
}

std::string FormatMoveForPosition(const surakarta::bitboard::Position& position,
                                  surakarta::bitboard::Move move) {
    if (!move.IsValid()) {
        return "n/a";
    }

    auto stream = std::ostringstream{};
    stream << surakarta::bitboard::PositionAdapter::ToLegacyMove(
        move,
        surakarta::bitboard::ToLegacyColor(position.SideToMove()));
    return stream.str();
}

std::string FormatPrincipalVariation(
    const surakarta::bitboard::Position& root,
    const std::array<surakarta::bitboard::Move, surakarta::bitboard::kMaxPly>& pv,
    int pv_length) {
    if (pv_length <= 0) {
        return "n/a";
    }

    auto working = root;
    auto stream = std::ostringstream{};
    const auto safe_length = std::clamp(pv_length, 0, surakarta::bitboard::kMaxPly);
    for (int ply = 0; ply < safe_length; ++ply) {
        const auto move = pv[ply];
        if (!move.IsValid()) {
            break;
        }
        if (ply > 0) {
            stream << " | ";
        }
        stream << FormatMoveForPosition(working, move);
        auto undo = surakarta::bitboard::Undo{};
        surakarta::bitboard::MakeMove(working, move, undo);
    }
    const auto text = stream.str();
    return text.empty() ? "n/a" : text;
}

std::string FormatDiagnosticLine(const surakarta::bitboard::Position& root,
                                 const surakarta::bitboard::SearchDiagnosticLine& line) {
    return FormatPrincipalVariation(root, line.moves, line.length);
}

CliSearchReport BuildSearchReport(const std::string& case_id,
                                  const surakarta::bitboard::Position& position,
                                  const surakarta::bitboard::SearchLimits& limits,
                                  const surakarta::bitboard::SearchResult& result,
                                  double search_seconds) {
    auto report = CliSearchReport{};
    report.case_id = case_id;
    report.depth = result.depth;
    report.threads = limits.threads;
    report.search_seconds = search_seconds;
    report.nodes = result.stats.nodes;
    report.qnodes = result.stats.qnodes;
    report.nps = search_seconds > 0.0 ? static_cast<std::uint64_t>(result.stats.nodes / search_seconds) : 0;
    report.tt_hits = result.stats.tt_hits;
    report.null_prunes = result.stats.null_prunes;
    report.lmr_reductions = result.stats.lmr_reductions;
    report.fail_highs = result.stats.fail_highs;
    report.fail_lows = result.stats.fail_lows;
    report.aspiration_retries = result.stats.aspiration_retries;
    report.aspiration_attempts = result.stats.aspiration_attempts;
    report.full_window_researches = result.stats.full_window_researches;
    report.root_best_updates = result.stats.root_best_updates;
    report.tt_exact_cutoffs = result.stats.tt_exact_cutoffs;
    report.tt_bound_cutoffs = result.stats.tt_bound_cutoffs;
    report.beta_cutoffs = result.stats.beta_cutoffs;
    report.stale_root_tasks = result.stats.stale_root_tasks;
    report.aborted_root_tasks = result.stats.aborted_root_tasks;
    report.worker_idle_spins = result.stats.worker_idle_spins;
    report.thread_spawn_count = result.stats.thread_spawn_count;
    report.pv = FormatPrincipalVariation(position, result.pv, result.pv_length);
    report.best_move = FormatMoveForPosition(position, result.best_move);
    report.score = result.score;

    for (const auto& entry : result.diagnostics.root_moves) {
        auto diagnostic = CliRootMoveDiagnostic{};
        diagnostic.depth = entry.depth;
        diagnostic.attempt = entry.attempt;
        diagnostic.move_index = entry.move_index;
        diagnostic.move = FormatMoveForPosition(position, entry.move);
        diagnostic.alpha = entry.alpha;
        diagnostic.beta = entry.beta;
        diagnostic.score = entry.score;
        diagnostic.bound = entry.bound;
        diagnostic.nodes = entry.nodes;
        diagnostic.qnodes = entry.qnodes;
        diagnostic.fail_highs = entry.fail_highs;
        diagnostic.fail_lows = entry.fail_lows;
        diagnostic.tt_hits = entry.tt_hits;
        diagnostic.beta_cutoffs = entry.beta_cutoffs;
        diagnostic.qsearch_entries = entry.qsearch_entries;
        diagnostic.qsearch_capture_moves = entry.qsearch_capture_moves;
        diagnostic.root_best_updates = entry.root_best_updates;
        diagnostic.pv = FormatDiagnosticLine(position, entry.pv);
        report.diagnostics.root_moves.push_back(diagnostic);
    }

    for (const auto& entry : result.diagnostics.aspiration_attempts) {
        auto diagnostic = CliAspirationDiagnostic{};
        diagnostic.depth = entry.depth;
        diagnostic.attempt = entry.attempt;
        diagnostic.alpha = entry.alpha;
        diagnostic.beta = entry.beta;
        diagnostic.score = entry.score;
        diagnostic.outcome = entry.outcome;
        diagnostic.nodes = entry.nodes;
        diagnostic.qnodes = entry.qnodes;
        diagnostic.root_best_updates = entry.root_best_updates;
        diagnostic.fail_highs = entry.fail_highs;
        diagnostic.fail_lows = entry.fail_lows;
        report.diagnostics.aspiration_attempts.push_back(diagnostic);
    }

    for (const auto& entry : result.diagnostics.qsearch_sources) {
        auto diagnostic = CliQsearchDiagnostic{};
        diagnostic.depth = entry.depth;
        diagnostic.attempt = entry.attempt;
        diagnostic.root_move_index = entry.root_move_index;
        diagnostic.root_move = FormatMoveForPosition(position, entry.root_move);
        diagnostic.ply = entry.ply;
        diagnostic.score = entry.score;
        diagnostic.qnodes = entry.qnodes;
        diagnostic.capture_moves = entry.capture_moves;
        diagnostic.line = FormatDiagnosticLine(position, entry.line);
        report.diagnostics.qsearch_sources.push_back(diagnostic);
    }
    return report;
}

bool HasSearchDiagnostics(const CliSearchReport& report) {
    return !report.diagnostics.root_moves.empty() ||
           !report.diagnostics.aspiration_attempts.empty() ||
           !report.diagnostics.qsearch_sources.empty();
}

void PrintSearchDiagnosticsJson(const CliSearchDiagnostics& diagnostics) {
    std::cout << "\"diagnostics\":{";
    std::cout << "\"root_moves\":[";
    for (std::size_t i = 0; i < diagnostics.root_moves.size(); ++i) {
        const auto& entry = diagnostics.root_moves[i];
        if (i > 0) {
            std::cout << ",";
        }
        std::cout << "{"
                  << "\"depth\":" << entry.depth << ","
                  << "\"attempt\":" << entry.attempt << ","
                  << "\"move_index\":" << entry.move_index << ","
                  << "\"move\":\"" << EscapeJsonString(entry.move) << "\","
                  << "\"alpha\":" << entry.alpha << ","
                  << "\"beta\":" << entry.beta << ","
                  << "\"score\":" << entry.score << ","
                  << "\"bound\":" << entry.bound << ","
                  << "\"nodes\":" << entry.nodes << ","
                  << "\"qnodes\":" << entry.qnodes << ","
                  << "\"fail_highs\":" << entry.fail_highs << ","
                  << "\"fail_lows\":" << entry.fail_lows << ","
                  << "\"tt_hits\":" << entry.tt_hits << ","
                  << "\"beta_cutoffs\":" << entry.beta_cutoffs << ","
                  << "\"qsearch_entries\":" << entry.qsearch_entries << ","
                  << "\"qsearch_capture_moves\":" << entry.qsearch_capture_moves << ","
                  << "\"root_best_updates\":" << entry.root_best_updates << ","
                  << "\"pv\":\"" << EscapeJsonString(entry.pv) << "\""
                  << "}";
    }
    std::cout << "],\"aspiration_attempts\":[";
    for (std::size_t i = 0; i < diagnostics.aspiration_attempts.size(); ++i) {
        const auto& entry = diagnostics.aspiration_attempts[i];
        if (i > 0) {
            std::cout << ",";
        }
        std::cout << "{"
                  << "\"depth\":" << entry.depth << ","
                  << "\"attempt\":" << entry.attempt << ","
                  << "\"alpha\":" << entry.alpha << ","
                  << "\"beta\":" << entry.beta << ","
                  << "\"score\":" << entry.score << ","
                  << "\"outcome\":\"" << EscapeJsonString(entry.outcome) << "\","
                  << "\"nodes\":" << entry.nodes << ","
                  << "\"qnodes\":" << entry.qnodes << ","
                  << "\"root_best_updates\":" << entry.root_best_updates << ","
                  << "\"fail_highs\":" << entry.fail_highs << ","
                  << "\"fail_lows\":" << entry.fail_lows
                  << "}";
    }
    std::cout << "],\"qsearch_sources\":[";
    for (std::size_t i = 0; i < diagnostics.qsearch_sources.size(); ++i) {
        const auto& entry = diagnostics.qsearch_sources[i];
        if (i > 0) {
            std::cout << ",";
        }
        std::cout << "{"
                  << "\"depth\":" << entry.depth << ","
                  << "\"attempt\":" << entry.attempt << ","
                  << "\"root_move_index\":" << entry.root_move_index << ","
                  << "\"root_move\":\"" << EscapeJsonString(entry.root_move) << "\","
                  << "\"ply\":" << entry.ply << ","
                  << "\"score\":" << entry.score << ","
                  << "\"qnodes\":" << entry.qnodes << ","
                  << "\"capture_moves\":" << entry.capture_moves << ","
                  << "\"line\":\"" << EscapeJsonString(entry.line) << "\""
                  << "}";
    }
    std::cout << "]}";
}

void PrintSearchReport(const CliSearchReport& report, OutputFormat format) {
    if (format == OutputFormat::Json) {
        std::cout << "{"
                  << "\"case_id\":\"" << EscapeJsonString(report.case_id) << "\","
                  << "\"depth\":" << report.depth << ","
                  << "\"threads\":" << report.threads << ","
                  << "\"search_seconds\":" << FormatDouble(report.search_seconds) << ","
                  << "\"nodes\":" << report.nodes << ","
                  << "\"qnodes\":" << report.qnodes << ","
                  << "\"nps\":" << report.nps << ","
                  << "\"tt_hits\":" << report.tt_hits << ","
                  << "\"null_prunes\":" << report.null_prunes << ","
                  << "\"lmr_reductions\":" << report.lmr_reductions << ","
                  << "\"fail_highs\":" << report.fail_highs << ","
                  << "\"fail_lows\":" << report.fail_lows << ","
                  << "\"aspiration_retries\":" << report.aspiration_retries << ","
                  << "\"aspiration_attempts\":" << report.aspiration_attempts << ","
                  << "\"full_window_researches\":" << report.full_window_researches << ","
                  << "\"root_best_updates\":" << report.root_best_updates << ","
                  << "\"tt_exact_cutoffs\":" << report.tt_exact_cutoffs << ","
                  << "\"tt_bound_cutoffs\":" << report.tt_bound_cutoffs << ","
                  << "\"beta_cutoffs\":" << report.beta_cutoffs << ","
                  << "\"stale_root_tasks\":" << report.stale_root_tasks << ","
                  << "\"aborted_root_tasks\":" << report.aborted_root_tasks << ","
                  << "\"worker_idle_spins\":" << report.worker_idle_spins << ","
                  << "\"thread_spawn_count\":" << report.thread_spawn_count << ","
                  << "\"pv\":\"" << EscapeJsonString(report.pv) << "\","
                  << "\"best_move\":\"" << EscapeJsonString(report.best_move) << "\","
                  << "\"score\":" << report.score;
        if (report.has_movegen) {
            std::cout << ",\"movegen_nodes\":" << report.movegen_nodes
                      << ",\"movegen_seconds\":" << FormatDouble(report.movegen_seconds)
                      << ",\"movegen_nps\":" << report.movegen_nps;
        }
        if (HasSearchDiagnostics(report)) {
            std::cout << ",";
            PrintSearchDiagnosticsJson(report.diagnostics);
        }
        std::cout << "}" << std::endl;
        return;
    }

    std::cout << "case_id: " << report.case_id << std::endl;
    if (report.has_movegen) {
        std::cout << "movegen_nodes: " << report.movegen_nodes << std::endl;
        std::cout << "movegen_seconds: " << report.movegen_seconds << std::endl;
        std::cout << "movegen_nps: " << report.movegen_nps << std::endl;
    }
    std::cout << "search_seconds: " << report.search_seconds << std::endl;
    std::cout << "depth: " << report.depth << std::endl;
    std::cout << "threads: " << report.threads << std::endl;
    std::cout << "nodes: " << report.nodes << std::endl;
    std::cout << "qnodes: " << report.qnodes << std::endl;
    std::cout << "nps: " << report.nps << std::endl;
    std::cout << "tt_hits: " << report.tt_hits << std::endl;
    std::cout << "null_prunes: " << report.null_prunes << std::endl;
    std::cout << "lmr_reductions: " << report.lmr_reductions << std::endl;
    std::cout << "fail_highs: " << report.fail_highs << std::endl;
    std::cout << "fail_lows: " << report.fail_lows << std::endl;
    std::cout << "aspiration_retries: " << report.aspiration_retries << std::endl;
    std::cout << "aspiration_attempts: " << report.aspiration_attempts << std::endl;
    std::cout << "full_window_researches: " << report.full_window_researches << std::endl;
    std::cout << "root_best_updates: " << report.root_best_updates << std::endl;
    std::cout << "tt_exact_cutoffs: " << report.tt_exact_cutoffs << std::endl;
    std::cout << "tt_bound_cutoffs: " << report.tt_bound_cutoffs << std::endl;
    std::cout << "beta_cutoffs: " << report.beta_cutoffs << std::endl;
    std::cout << "stale_root_tasks: " << report.stale_root_tasks << std::endl;
    std::cout << "aborted_root_tasks: " << report.aborted_root_tasks << std::endl;
    std::cout << "worker_idle_spins: " << report.worker_idle_spins << std::endl;
    std::cout << "thread_spawn_count: " << report.thread_spawn_count << std::endl;
    std::cout << "pv: " << report.pv << std::endl;
    std::cout << "best_move: " << report.best_move << std::endl;
    std::cout << "score: " << report.score << std::endl;
    if (HasSearchDiagnostics(report)) {
        for (const auto& entry : report.diagnostics.aspiration_attempts) {
            std::cout << "aspiration_attempt: depth=" << entry.depth
                      << " attempt=" << entry.attempt
                      << " window=[" << entry.alpha << "," << entry.beta << "]"
                      << " score=" << entry.score
                      << " outcome=" << entry.outcome
                      << " nodes=" << entry.nodes
                      << " qnodes=" << entry.qnodes
                      << std::endl;
        }
        for (const auto& entry : report.diagnostics.root_moves) {
            std::cout << "root_move: depth=" << entry.depth
                      << " attempt=" << entry.attempt
                      << " index=" << entry.move_index
                      << " move=" << entry.move
                      << " score=" << entry.score
                      << " nodes=" << entry.nodes
                      << " qnodes=" << entry.qnodes
                      << " fail_highs=" << entry.fail_highs
                      << " root_best_updates=" << entry.root_best_updates
                      << " pv=" << entry.pv
                      << std::endl;
        }
        for (const auto& entry : report.diagnostics.qsearch_sources) {
            std::cout << "qsearch_source: depth=" << entry.depth
                      << " attempt=" << entry.attempt
                      << " root_move=" << entry.root_move
                      << " ply=" << entry.ply
                      << " qnodes=" << entry.qnodes
                      << " captures=" << entry.capture_moves
                      << " line=" << entry.line
                      << std::endl;
        }
    }
}

struct CommonBitboardOptions {
    int depth{4};
    int threads{1};
    int aspiration_window{32};
    std::string file_name;
    std::string weights_file;
    std::string case_id;
    OutputFormat format{OutputFormat::Text};
    bool search_diagnostics{false};
};

bool RequireValue(int argc, char** argv, int* index, const char* option_name) {
    if (*index + 1 >= argc) {
        std::cerr << "Missing value for option: " << option_name << std::endl;
        return false;
    }
    ++(*index);
    return true;
}

bool ParseCommonBitboardOptions(int argc, char** argv, CommonBitboardOptions* options) {
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--depth") == 0 || strcmp(argv[i], "-d") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->depth = atoi(argv[i]);
        } else if (strcmp(argv[i], "--threads") == 0 || strcmp(argv[i], "-j") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->threads = atoi(argv[i]);
        } else if (strcmp(argv[i], "--aspiration") == 0 || strcmp(argv[i], "-A") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->aspiration_window = atoi(argv[i]);
        } else if (strcmp(argv[i], "--file") == 0 || strcmp(argv[i], "-f") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->file_name = argv[i];
        } else if (strcmp(argv[i], "--weights") == 0 || strcmp(argv[i], "-w") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->weights_file = argv[i];
        } else if (strcmp(argv[i], "--format") == 0 || strcmp(argv[i], "-F") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->format = std::string(argv[i]) == "json" ? OutputFormat::Json : OutputFormat::Text;
        } else if (strcmp(argv[i], "--case-id") == 0 || strcmp(argv[i], "-c") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->case_id = argv[i];
        } else if (strcmp(argv[i], "--search-diagnostics") == 0 ||
                   strcmp(argv[i], "--debug-search-tree") == 0) {
            options->search_diagnostics = true;
        } else {
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            return false;
        }
    }
    options->case_id = DeriveCaseId(options->case_id, options->file_name);
    return true;
}

void PrintTrainingSummary(const surakarta::bitboard::TrainingSummary& summary, OutputFormat format) {
    if (format == OutputFormat::Json) {
        std::cout << "{"
                  << "\"games_requested\":" << summary.games_requested << ","
                  << "\"games_completed\":" << summary.games_completed << ","
                  << "\"seed\":" << summary.seed << ","
                  << "\"depth\":" << summary.depth << ","
                  << "\"alpha\":" << FormatDouble(summary.alpha) << ","
                  << "\"lambda\":" << FormatDouble(summary.lambda) << ","
                  << "\"epsilon\":" << FormatDouble(summary.epsilon) << ","
                  << "\"epsilon_plies\":" << summary.epsilon_plies << ","
                  << "\"terminal_reward\":" << FormatDouble(summary.terminal_reward) << ","
                  << "\"td_error_clip\":" << FormatDouble(summary.td_error_clip) << ","
                  << "\"terminal_only_warmup\":" << summary.terminal_only_warmup << ","
                  << "\"near_terminal_curriculum\":" << summary.near_terminal_curriculum << ","
                  << "\"black_wins\":" << summary.black_wins << ","
                  << "\"white_wins\":" << summary.white_wins << ","
                  << "\"draws\":" << summary.draws << ","
                  << "\"terminal_checkmate\":" << summary.terminal_checkmate << ","
                  << "\"terminal_no_capture_limit\":" << summary.terminal_no_capture_limit << ","
                  << "\"terminal_no_legal_move\":" << summary.terminal_no_legal_move << ","
                  << "\"terminal_ply_cap\":" << summary.terminal_ply_cap << ","
                  << "\"positions_evaluated\":" << summary.positions_evaluated << ","
                  << "\"update_count\":" << summary.update_count << ","
                  << "\"average_abs_td_error\":" << FormatDouble(summary.average_abs_td_error) << ","
                  << "\"max_abs_td_error\":" << FormatDouble(summary.max_abs_td_error) << ","
                  << "\"average_abs_weight_delta\":" << FormatDouble(summary.average_abs_weight_delta) << ","
                  << "\"max_abs_weight_delta\":" << FormatDouble(summary.max_abs_weight_delta) << ","
                  << "\"active_interface_config_present\":" << BoolJson(summary.active_interface_config_present) << ","
                  << "\"active_interface_config_valid\":" << BoolJson(summary.active_interface_config_valid) << ","
                  << "\"active_interface_skeleton_enabled\":" << BoolJson(summary.active_interface_skeleton_enabled) << ","
                  << "\"active_interface_scoped_config\":" << BoolJson(summary.active_interface_scoped_config) << ","
                  << "\"active_interface_probe_wiring_skeleton\":"
                  << BoolJson(summary.active_interface_probe_wiring_skeleton) << ","
                  << "\"active_interface_no_output_probe_mode\":"
                  << BoolJson(summary.active_interface_no_output_probe_mode) << ","
                  << "\"active_interface_weight_artifact_suppressed\":"
                  << BoolJson(summary.active_interface_weight_artifact_suppressed) << ","
                  << "\"active_interface_report_only_probe_path\":"
                  << BoolJson(summary.active_interface_report_only_probe_path) << ","
                  << "\"active_objective_probe_executed\":"
                  << BoolJson(summary.active_objective_probe_executed) << ","
                  << "\"selection_gate_eligible\":" << BoolJson(summary.selection_gate_eligible) << ","
                  << "\"active_interface_config_status\":\""
                  << EscapeJsonString(summary.active_interface_config_status) << "\","

                  << "\"checkpoint_count\":" << summary.checkpoint_count << ","
                  << "\"checkpoint_summaries\":[";
        for (std::size_t i = 0; i < summary.checkpoint_summaries.size(); ++i) {
            const auto& checkpoint = summary.checkpoint_summaries[i];
            if (i > 0) {
                std::cout << ",";
            }
            std::cout << "{"
                      << "\"games_completed\":" << checkpoint.games_completed << ","
                      << "\"path\":\"" << EscapeJsonString(checkpoint.path) << "\","
                      << "\"black_wins\":" << checkpoint.black_wins << ","
                      << "\"white_wins\":" << checkpoint.white_wins << ","
                      << "\"draws\":" << checkpoint.draws << ","
                      << "\"terminal_checkmate\":" << checkpoint.terminal_checkmate << ","
                      << "\"terminal_no_capture_limit\":" << checkpoint.terminal_no_capture_limit << ","
                      << "\"terminal_no_legal_move\":" << checkpoint.terminal_no_legal_move << ","
                      << "\"terminal_ply_cap\":" << checkpoint.terminal_ply_cap << ","
                      << "\"positions_evaluated\":" << checkpoint.positions_evaluated << ","
                      << "\"update_count\":" << checkpoint.update_count << ","
                      << "\"average_abs_td_error\":" << FormatDouble(checkpoint.average_abs_td_error) << ","
                      << "\"max_abs_td_error\":" << FormatDouble(checkpoint.max_abs_td_error) << ","
                      << "\"average_abs_weight_delta\":" << FormatDouble(checkpoint.average_abs_weight_delta) << ","
                      << "\"max_abs_weight_delta\":" << FormatDouble(checkpoint.max_abs_weight_delta)
                      << "}";
        }
        std::cout << "],"
                  << "\"output_weights\":\"" << EscapeJsonString(summary.output_weights_path) << "\""
                  << "}" << std::endl;
        return;
    }

    std::cout << "games_requested: " << summary.games_requested << std::endl;
    std::cout << "games_completed: " << summary.games_completed << std::endl;
    std::cout << "seed: " << summary.seed << std::endl;
    std::cout << "depth: " << summary.depth << std::endl;
    std::cout << "alpha: " << summary.alpha << std::endl;
    std::cout << "lambda: " << summary.lambda << std::endl;
    std::cout << "epsilon: " << summary.epsilon << std::endl;
    std::cout << "epsilon_plies: " << summary.epsilon_plies << std::endl;
    std::cout << "terminal_reward: " << summary.terminal_reward << std::endl;
    std::cout << "td_error_clip: " << summary.td_error_clip << std::endl;
    std::cout << "terminal_only_warmup: " << summary.terminal_only_warmup << std::endl;
    std::cout << "near_terminal_curriculum: " << summary.near_terminal_curriculum << std::endl;
    std::cout << "black_wins: " << summary.black_wins << std::endl;
    std::cout << "white_wins: " << summary.white_wins << std::endl;
    std::cout << "draws: " << summary.draws << std::endl;
    std::cout << "terminal_checkmate: " << summary.terminal_checkmate << std::endl;
    std::cout << "terminal_no_capture_limit: " << summary.terminal_no_capture_limit << std::endl;
    std::cout << "terminal_no_legal_move: " << summary.terminal_no_legal_move << std::endl;
    std::cout << "terminal_ply_cap: " << summary.terminal_ply_cap << std::endl;
    std::cout << "positions_evaluated: " << summary.positions_evaluated << std::endl;
    std::cout << "update_count: " << summary.update_count << std::endl;
    std::cout << "average_abs_td_error: " << summary.average_abs_td_error << std::endl;
    std::cout << "max_abs_td_error: " << summary.max_abs_td_error << std::endl;
    std::cout << "average_abs_weight_delta: " << summary.average_abs_weight_delta << std::endl;
    std::cout << "max_abs_weight_delta: " << summary.max_abs_weight_delta << std::endl;
    std::cout << "active_interface_config_present: " << BoolJson(summary.active_interface_config_present) << std::endl;
    std::cout << "active_interface_config_valid: " << BoolJson(summary.active_interface_config_valid) << std::endl;
    std::cout << "active_interface_skeleton_enabled: "
              << BoolJson(summary.active_interface_skeleton_enabled) << std::endl;
    std::cout << "active_interface_scoped_config: " << BoolJson(summary.active_interface_scoped_config) << std::endl;
    std::cout << "active_interface_probe_wiring_skeleton: "
              << BoolJson(summary.active_interface_probe_wiring_skeleton) << std::endl;
    std::cout << "active_interface_no_output_probe_mode: "
              << BoolJson(summary.active_interface_no_output_probe_mode) << std::endl;
    std::cout << "active_interface_weight_artifact_suppressed: "
              << BoolJson(summary.active_interface_weight_artifact_suppressed) << std::endl;
    std::cout << "active_interface_report_only_probe_path: "
              << BoolJson(summary.active_interface_report_only_probe_path) << std::endl;
    std::cout << "active_objective_probe_executed: " << BoolJson(summary.active_objective_probe_executed) << std::endl;
    std::cout << "selection_gate_eligible: " << BoolJson(summary.selection_gate_eligible) << std::endl;
    std::cout << "active_interface_config_status: " << summary.active_interface_config_status << std::endl;
    std::cout << "checkpoint_count: " << summary.checkpoint_count << std::endl;
    for (const auto& checkpoint : summary.checkpoint_summaries) {
        std::cout << "checkpoint_summary: games_completed=" << checkpoint.games_completed
                  << " path=" << checkpoint.path
                  << " black_wins=" << checkpoint.black_wins
                  << " white_wins=" << checkpoint.white_wins
                  << " draws=" << checkpoint.draws
                  << " average_abs_td_error=" << checkpoint.average_abs_td_error
                  << " max_abs_weight_delta=" << checkpoint.max_abs_weight_delta
                  << std::endl;
    }
    std::cout << "output_weights: " << summary.output_weights_path << std::endl;
}

void PrintEvalReport(const surakarta::bitboard::EvalReport& report, OutputFormat format) {
    if (format == OutputFormat::Json) {
        std::cout << "{"
                  << "\"candidate_weights\":\"" << EscapeJsonString(report.candidate_weights) << "\","
                  << "\"baseline_weights\":\"" << EscapeJsonString(report.baseline_weights) << "\","
                  << "\"depth\":" << report.depth << ","
                  << "\"position_reports\":[";
        for (std::size_t i = 0; i < report.position_reports.size(); ++i) {
            const auto& entry = report.position_reports[i];
            if (i > 0) {
                std::cout << ",";
            }
            std::cout << "{"
                      << "\"case_id\":\"" << EscapeJsonString(entry.case_id) << "\","
                      << "\"candidate\":{"
                      << "\"case_id\":\"" << EscapeJsonString(entry.candidate.case_id) << "\","
                      << "\"depth\":" << entry.candidate.depth << ","
                      << "\"threads\":" << entry.candidate.threads << ","
                      << "\"search_seconds\":" << FormatDouble(entry.candidate.search_seconds) << ","
                      << "\"nodes\":" << entry.candidate.nodes << ","
                      << "\"best_move\":\"" << EscapeJsonString(entry.candidate.best_move) << "\","
                      << "\"pv\":\"" << EscapeJsonString(entry.candidate.pv) << "\","
                      << "\"score\":" << entry.candidate.score
                      << "},"
                      << "\"baseline\":{"
                      << "\"case_id\":\"" << EscapeJsonString(entry.baseline.case_id) << "\","
                      << "\"depth\":" << entry.baseline.depth << ","
                      << "\"threads\":" << entry.baseline.threads << ","
                      << "\"search_seconds\":" << FormatDouble(entry.baseline.search_seconds) << ","
                      << "\"nodes\":" << entry.baseline.nodes << ","
                      << "\"best_move\":\"" << EscapeJsonString(entry.baseline.best_move) << "\","
                      << "\"pv\":\"" << EscapeJsonString(entry.baseline.pv) << "\","
                      << "\"score\":" << entry.baseline.score
                      << "}"
                      << "}";
        }
        std::cout << "],"
                  << "\"match_summary\":{"
                  << "\"candidate_wins\":" << report.match_summary.candidate_wins << ","
                  << "\"baseline_wins\":" << report.match_summary.baseline_wins << ","
                  << "\"draws\":" << report.match_summary.draws
                  << "},"
                  << "\"match_games\":[";
        for (std::size_t i = 0; i < report.match_games.size(); ++i) {
            const auto& game = report.match_games[i];
            if (i > 0) {
                std::cout << ",";
            }
            std::cout << "{"
                      << "\"case_id\":\"" << EscapeJsonString(game.case_id) << "\","
                      << "\"candidate_color\":\"" << EscapeJsonString(game.candidate_color) << "\","
                      << "\"plies\":" << game.plies << ","
                      << "\"outcome\":\"" << EscapeJsonString(game.outcome) << "\","
                      << "\"final_reason\":\"" << EscapeJsonString(game.final_reason) << "\""
                      << "}";
        }
        std::cout << "]}" << std::endl;
        return;
    }

    std::cout << "candidate_weights: " << report.candidate_weights << std::endl;
    std::cout << "baseline_weights: " << report.baseline_weights << std::endl;
    std::cout << "depth: " << report.depth << std::endl;
    for (const auto& entry : report.position_reports) {
        std::cout << "case_id: " << entry.case_id << std::endl;
        std::cout << "candidate_best_move: " << entry.candidate.best_move << std::endl;
        std::cout << "candidate_score: " << entry.candidate.score << std::endl;
        std::cout << "candidate_pv: " << entry.candidate.pv << std::endl;
        std::cout << "baseline_best_move: " << entry.baseline.best_move << std::endl;
        std::cout << "baseline_score: " << entry.baseline.score << std::endl;
        std::cout << "baseline_pv: " << entry.baseline.pv << std::endl;
    }
    std::cout << "candidate_wins: " << report.match_summary.candidate_wins << std::endl;
    std::cout << "baseline_wins: " << report.match_summary.baseline_wins << std::endl;
    std::cout << "draws: " << report.match_summary.draws << std::endl;
    for (const auto& game : report.match_games) {
        std::cout << "match_game: " << game.case_id
                  << " candidate_color=" << game.candidate_color
                  << " outcome=" << game.outcome
                  << " final_reason=" << game.final_reason
                  << " plies=" << game.plies << std::endl;
    }
}

int RunBitboardSearch(const std::string& file_name,
                      const std::string& case_id,
                      int depth,
                      int threads,
                      int aspiration_window,
                      const std::string& weights_file,
                      bool search_diagnostics,
                      OutputFormat format) {
    auto position = surakarta::bitboard::Position{};
    LoadBitboardPosition(file_name, position);
    auto limits = surakarta::bitboard::SearchLimits{};
    limits.max_depth = depth;
    limits.threads = threads;
    limits.aspiration_window = aspiration_window;
    limits.enable_diagnostics = search_diagnostics;

    auto controller = surakarta::bitboard::SearchController{};
    if (!weights_file.empty() && !controller.Evaluator().LoadWeights(weights_file)) {
        std::cerr << "Failed to load weight file: " << weights_file << std::endl;
        return 1;
    }

    const auto start = std::chrono::steady_clock::now();
    const auto result = controller.Search(position, limits);
    const auto end = std::chrono::steady_clock::now();
    const auto report = BuildSearchReport(case_id,
                                          position,
                                          limits,
                                          result,
                                          std::chrono::duration<double>(end - start).count());
    PrintSearchReport(report, format);
    return result.best_move.IsValid() ? 0 : 1;
}

int RunBitboardBenchmarkCli(const std::string& file_name,
                            const std::string& case_id,
                            int depth,
                            int threads,
                            int aspiration_window,
                            int movegen_iterations,
                            const std::string& weights_file,
                            bool search_diagnostics,
                            OutputFormat format) {
    auto position = surakarta::bitboard::Position{};
    LoadBitboardPosition(file_name, position);
    auto limits = surakarta::bitboard::SearchLimits{};
    limits.max_depth = depth;
    limits.threads = threads;
    limits.aspiration_window = aspiration_window;
    limits.enable_diagnostics = search_diagnostics;

    auto controller = surakarta::bitboard::SearchController{};
    if (!weights_file.empty() && !controller.Evaluator().LoadWeights(weights_file)) {
        std::cerr << "Failed to load weight file: " << weights_file << std::endl;
        return 1;
    }

    const auto benchmark = surakarta::bitboard::RunBitboardBenchmark(controller, position, limits, movegen_iterations);
    const auto movegen_rate = benchmark.movegen_seconds > 0.0 ? benchmark.movegen_nodes / benchmark.movegen_seconds : 0.0;

    auto report = BuildSearchReport(case_id, position, limits, benchmark.search_result, benchmark.search_seconds);
    report.has_movegen = true;
    report.movegen_nodes = benchmark.movegen_nodes;
    report.movegen_seconds = benchmark.movegen_seconds;
    report.movegen_nps = static_cast<std::uint64_t>(movegen_rate);
    PrintSearchReport(report, format);
    return benchmark.search_result.best_move.IsValid() ? 0 : 1;
}

int RunBitboardSearchCommand(int argc, char** argv) {
    auto options = CommonBitboardOptions{};
    if (!ParseCommonBitboardOptions(argc, argv, &options)) {
        return 1;
    }
    return RunBitboardSearch(options.file_name,
                             options.case_id,
                             options.depth,
                              options.threads,
                              options.aspiration_window,
                              options.weights_file,
                              options.search_diagnostics,
                              options.format);
}

int RunBitboardBenchmarkCommand(int argc, char** argv) {
    auto options = CommonBitboardOptions{};
    auto movegen_iterations = 100000;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--movegen-iters") == 0 || strcmp(argv[i], "-m") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            movegen_iterations = atoi(argv[i]);
        } else if (strcmp(argv[i], "--depth") == 0 ||
                   strcmp(argv[i], "-d") == 0 ||
                   strcmp(argv[i], "--threads") == 0 ||
                   strcmp(argv[i], "-j") == 0 ||
                   strcmp(argv[i], "--aspiration") == 0 ||
                   strcmp(argv[i], "-A") == 0 ||
                   strcmp(argv[i], "--file") == 0 ||
                   strcmp(argv[i], "-f") == 0 ||
                   strcmp(argv[i], "--weights") == 0 ||
                   strcmp(argv[i], "-w") == 0 ||
                   strcmp(argv[i], "--format") == 0 ||
                   strcmp(argv[i], "-F") == 0 ||
                   strcmp(argv[i], "--case-id") == 0 ||
                   strcmp(argv[i], "-c") == 0) {
            continue;
        }
    }

    auto filtered = std::vector<char*>{};
    filtered.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--movegen-iters") == 0 || strcmp(argv[i], "-m") == 0) {
            ++i;
            continue;
        }
        filtered.push_back(argv[i]);
    }
    if (!ParseCommonBitboardOptions(static_cast<int>(filtered.size()), filtered.data(), &options)) {
        return 1;
    }

    return RunBitboardBenchmarkCli(options.file_name,
                                   options.case_id,
                                   options.depth,
                                     options.threads,
                                     options.aspiration_window,
                                     movegen_iterations,
                                     options.weights_file,
                                     options.search_diagnostics,
                                     options.format);
}

bool ParseBitboardTraceOptions(int argc, char** argv, BitboardTraceOptions* options) {
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--weights") == 0 || strcmp(argv[i], "-w") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->weights_file = argv[i];
        } else if (strcmp(argv[i], "--games") == 0 || strcmp(argv[i], "-g") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->games = atoi(argv[i]);
        } else if (strcmp(argv[i], "--depth") == 0 || strcmp(argv[i], "-d") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->depth = atoi(argv[i]);
        } else if (strcmp(argv[i], "--alpha") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->alpha = atof(argv[i]);
        } else if (strcmp(argv[i], "--lambda") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->lambda = atof(argv[i]);
        } else if (strcmp(argv[i], "--epsilon") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->epsilon = atof(argv[i]);
        } else if (strcmp(argv[i], "--epsilon-plies") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->epsilon_plies = atoi(argv[i]);
        } else if (strcmp(argv[i], "--terminal-reward") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->terminal_reward = atof(argv[i]);
        } else if (strcmp(argv[i], "--td-error-clip") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->td_error_clip = atof(argv[i]);
        } else if (strcmp(argv[i], "--terminal-only-warmup") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->terminal_only_warmup = atoi(argv[i]);
        } else if (strcmp(argv[i], "--near-terminal-curriculum") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->near_terminal_curriculum = atoi(argv[i]);
        } else if (strcmp(argv[i], "--seed") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->seed = static_cast<std::uint32_t>(std::strtoul(argv[i], nullptr, 10));
        } else if (strcmp(argv[i], "--format") == 0 || strcmp(argv[i], "-F") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return false;
            }
            options->format = ParseTraceOutputFormat(argv[i]);
        } else {
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            return false;
        }
    }
    return true;
}

int RunBitboardTraceCommand(int argc, char** argv) {
    constexpr int kTraceTrainingMaxPlies = 256;

    auto options = BitboardTraceOptions{};
    if (!ParseBitboardTraceOptions(argc, argv, &options)) {
        return 1;
    }
    options.games = std::max(1, options.games);
    options.depth = options.depth > 0 ? options.depth : 4;
    options.epsilon_plies = std::max(0, options.epsilon_plies);
    if (options.terminal_reward <= 0.0) {
        std::cerr << "terminal reward must be positive" << std::endl;
        return 1;
    }
    if (options.td_error_clip < 0.0) {
        std::cerr << "td error clip must be non-negative" << std::endl;
        return 1;
    }
    options.terminal_only_warmup = std::max(0, options.terminal_only_warmup);
    options.near_terminal_curriculum = std::max(0, options.near_terminal_curriculum);

    auto controller = surakarta::bitboard::SearchController{};
    if (!options.weights_file.empty() && !controller.Evaluator().LoadWeights(options.weights_file)) {
        std::cerr << "Failed to load weight file: " << options.weights_file << std::endl;
        return 1;
    }

    auto weights = controller.Evaluator().ExportWeights();
    auto traces = std::vector<double>(weights.values.size(), 0.0);
    auto rng = std::mt19937(options.seed);
    auto random_unit = std::uniform_real_distribution<double>(0.0, 1.0);

    auto limits = surakarta::bitboard::SearchLimits{};
    limits.max_depth = options.depth;
    limits.threads = 1;
    limits.aspiration_window = 32;

    auto report = BitboardTraceReport{};
    report.seed = options.seed;
    report.games_requested = options.games;
    report.depth = options.depth;
    report.alpha = options.alpha;
    report.lambda = options.lambda;
    report.epsilon = options.epsilon;
    report.epsilon_plies = options.epsilon_plies;
    report.terminal_reward = options.terminal_reward;
    report.td_error_clip = options.td_error_clip;
    report.terminal_only_warmup = options.terminal_only_warmup;
    report.near_terminal_curriculum = options.near_terminal_curriculum;

    const auto curriculum_games = std::max(options.terminal_only_warmup, options.near_terminal_curriculum);
    for (int game_index = 0; game_index < options.games; ++game_index) {
        auto position = surakarta::bitboard::Position{};
        if (game_index < curriculum_games) {
            position = surakarta::bitboard::BuildNearTerminalTrainingPosition(game_index);
        } else {
            LoadBitboardPosition("", position);
        }
        surakarta::bitboard::ResetTrainingTraces(traces);
        auto plies = 0;

        for (;;) {
            if (IsTraceTerminalPosition(position)) {
                break;
            }

            auto legal_moves = surakarta::bitboard::MoveList{};
            surakarta::bitboard::GenerateMoves(position, legal_moves);
            if (legal_moves.size == 0) {
                break;
            }

            auto chosen_move = surakarta::bitboard::Move{};
            if (plies < options.epsilon_plies && random_unit(rng) < options.epsilon) {
                const auto move_index = std::uniform_int_distribution<int>(0, legal_moves.size - 1)(rng);
                chosen_move = legal_moves.moves[move_index];
            } else {
                if (!controller.Evaluator().ApplyWeights(weights)) {
                    std::cerr << "Failed to apply trace weights to search controller" << std::endl;
                    return 1;
                }
                controller.Table().Clear();
                const auto result = controller.Search(position, limits);
                chosen_move = result.best_move.IsValid() ? result.best_move : legal_moves.moves[0];
            }

            auto next = position;
            auto undo = surakarta::bitboard::Undo{};
            surakarta::bitboard::MakeMove(next, chosen_move, undo);

            auto context = surakarta::bitboard::TrainingStepContext{};
            context.next_is_ply_cap = (plies + 1) >= kTraceTrainingMaxPlies;
            const auto next_terminal = IsTraceTerminalPosition(next);
            auto next_legal_moves = surakarta::bitboard::MoveList{};
            if (!next_terminal && !context.next_is_ply_cap) {
                surakarta::bitboard::GenerateMoves(next, next_legal_moves);
                context.next_has_legal_moves = next_legal_moves.size > 0;
            }

            const auto before_weights = weights.values;
            auto step_result = surakarta::bitboard::TrainingStepResult{};
            if (!surakarta::bitboard::ApplyTrainingStep(
                    weights,
                    traces,
                    position,
                    next,
                    context,
                    options.alpha,
                    options.lambda,
                    &step_result,
                    options.terminal_reward,
                    options.td_error_clip)) {
                std::cerr << "Failed to apply trace TD step" << std::endl;
                return 1;
            }

            auto step = BitboardTraceStep{};
            step.game_index = game_index;
            step.ply = plies;
            step.side_to_move = BitboardColorName(position.SideToMove());
            step.reward = step_result.terminal_target ? step_result.target_value : 0.0;
            step.value_before = step_result.current_value;
            step.value_after = surakarta::bitboard::EvaluateTrainingValueForSideToMove(weights, position);
            step.bootstrap_value = step_result.terminal_target ? 0.0 : step_result.target_value;
            step.td_error = step_result.td_error;
            step.trace_norm = L2Norm(traces);
            step.weight_delta_norm = L2DeltaNorm(before_weights, weights.values);
            step.terminal_reason = TraceTerminalReason(next, context);
            report.steps.push_back(step);

            position = next;
            ++plies;
            if (next_terminal || !context.next_has_legal_moves || context.next_is_ply_cap) {
                break;
            }
        }
    }

    PrintBitboardTraceReport(report, options.format);
    return 0;
}

int RunBitboardTrainCommand(int argc, char** argv) {
    auto options = surakarta::bitboard::TrainingOptions{};
    auto format = OutputFormat::Text;
    options.games = 1;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-o") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.output_weights_path = argv[i];
        } else if (strcmp(argv[i], "--weights") == 0 || strcmp(argv[i], "-w") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.input_weights_path = argv[i];
        } else if (strcmp(argv[i], "--games") == 0 || strcmp(argv[i], "-g") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.games = atoi(argv[i]);
        } else if (strcmp(argv[i], "--depth") == 0 || strcmp(argv[i], "-d") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.limits.max_depth = atoi(argv[i]);
        } else if (strcmp(argv[i], "--alpha") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.alpha = atof(argv[i]);
        } else if (strcmp(argv[i], "--lambda") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.lambda = atof(argv[i]);
        } else if (strcmp(argv[i], "--epsilon") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.epsilon = atof(argv[i]);
        } else if (strcmp(argv[i], "--epsilon-plies") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.epsilon_plies = atoi(argv[i]);
        } else if (strcmp(argv[i], "--terminal-reward") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.terminal_reward = atof(argv[i]);
        } else if (strcmp(argv[i], "--td-error-clip") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.td_error_clip = atof(argv[i]);
        } else if (strcmp(argv[i], "--terminal-only-warmup") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.terminal_only_warmup = atoi(argv[i]);
        } else if (strcmp(argv[i], "--near-terminal-curriculum") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.near_terminal_curriculum = atoi(argv[i]);
        } else if (strcmp(argv[i], "--seed") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.seed = static_cast<std::uint32_t>(std::strtoul(argv[i], nullptr, 10));
        } else if (strcmp(argv[i], "--checkpoint-every") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.checkpoint_every = atoi(argv[i]);
        } else if (strcmp(argv[i], "--checkpoint-dir") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.checkpoint_dir = argv[i];
        } else if (strcmp(argv[i], "--config") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.active_objective_config_path = argv[i];
            auto config_file = std::ifstream(options.active_objective_config_path);
            if (!config_file) {
                std::cerr << "failed to open config: " << options.active_objective_config_path << std::endl;
                return 1;
            }
            auto config_stream = std::ostringstream{};
            config_stream << config_file.rdbuf();
            auto config_error = std::string{};
            if (!surakarta::bitboard::ParseActiveObjectiveConfigSkeleton(
                    config_stream.str(), &options.active_objective_config, &config_error)) {
                std::cerr << "invalid config: " << config_error << std::endl;
                return 1;
            }
        } else if (strcmp(argv[i], "--format") == 0 || strcmp(argv[i], "-F") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            format = std::string(argv[i]) == "json" ? OutputFormat::Json : OutputFormat::Text;
        } else {
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            return 1;
        }
    }

    if (options.checkpoint_every > 0 && options.checkpoint_dir.empty()) {
        const auto output_path = std::filesystem::path(options.output_weights_path);
        options.checkpoint_dir = (output_path.parent_path() / "checkpoints").string();
    }

    auto summary = surakarta::bitboard::TrainingSummary{};
    auto error_message = std::string{};
    if (!surakarta::bitboard::RunBitboardTraining(options, &summary, &error_message)) {
        std::cerr << "bitboard-train failed: " << error_message << std::endl;
        return 1;
    }
    PrintTrainingSummary(summary, format);
    return 0;
}

int RunBitboardEvalCommand(int argc, char** argv) {
    auto options = surakarta::bitboard::EvalOptions{};
    auto format = OutputFormat::Text;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--candidate") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.candidate_weights_path = argv[i];
        } else if (strcmp(argv[i], "--baseline") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.baseline_weights_path = argv[i];
        } else if (strcmp(argv[i], "--depth") == 0 || strcmp(argv[i], "-d") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            options.limits.max_depth = atoi(argv[i]);
        } else if (strcmp(argv[i], "--format") == 0 || strcmp(argv[i], "-F") == 0) {
            if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                return 1;
            }
            format = std::string(argv[i]) == "json" ? OutputFormat::Json : OutputFormat::Text;
        } else {
            std::cerr << "Unknown option: " << argv[i] << std::endl;
            return 1;
        }
    }

    auto report = surakarta::bitboard::EvalReport{};
    auto error_message = std::string{};
    if (!surakarta::bitboard::RunBitboardEvaluation(options, &report, &error_message)) {
        std::cerr << "bitboard-eval failed: " << error_message << std::endl;
        return 1;
    }
    PrintEvalReport(report, format);
    return 0;
}

void PrintUsage(const char* executable) {
    std::cout << "Usage: " << executable << " play [args..] [--delay|-D <delay>]" << std::endl;
    std::cout << "       " << executable << " bitboard-search [args..] [--file|-f <board-file>]" << std::endl;
    std::cout << "       " << executable << " bitboard-benchmark [args..] [--movegen-iters|-m <count>]" << std::endl;
    std::cout << "       " << executable << " bitboard-trace [--games|-g <n>] [--seed <n>] [--format json|csv]" << std::endl;
    std::cout << "       " << executable << " bitboard-train --output|-o <weights.bin> [args..]" << std::endl;
    std::cout << "       " << executable << " bitboard-eval --candidate <weights.bin> [args..]" << std::endl;
    std::cout << "       " << executable << " bitboard-selftest" << std::endl;
    std::cout << "       " << executable << " statistic [args..] [-j <concurrency>] [-n <total_rounds>]" << std::endl;
    std::cout << "Args:" << std::endl;
    std::cout << "  --depth|-d <depth>  Search depth for bitboard commands, default: 4" << std::endl;
    std::cout << "  --threads|-j <n>    Bitboard search worker count, default: 1" << std::endl;
    std::cout << "  --aspiration|-A <n> Aspiration window size, default: 32" << std::endl;
    std::cout << "  --file|-f <path>    Optional board file for bitboard-search/bitboard-benchmark" << std::endl;
    std::cout << "  --weights|-w <path> Optional N-Tuple weight file for bitboard-search/bitboard-benchmark" << std::endl;
    std::cout << "  --movegen-iters|-m  Move generation iterations for bitboard-benchmark, default: 100000" << std::endl;
    std::cout << "  --format|-F <mode>  Output format: text or json, default: text" << std::endl;
    std::cout << "  --case-id|-c <id>   Optional report case identifier, default: opening or file stem" << std::endl;
    std::cout << "  --search-diagnostics Enable root/aspiration/qsearch diagnostic output for bitboard search commands" << std::endl;
    std::cout << "  bitboard-trace args: --weights|-w --games|-g --alpha --lambda --epsilon --epsilon-plies --terminal-reward --td-error-clip --terminal-only-warmup --near-terminal-curriculum --seed --format json|csv" << std::endl;
    std::cout << "  bitboard-train args: --weights|-w --games|-g --alpha --lambda --epsilon --epsilon-plies --terminal-reward --td-error-clip --terminal-only-warmup --near-terminal-curriculum --seed --checkpoint-every --checkpoint-dir --config" << std::endl;
    std::cout << "  bitboard-eval args: --baseline --depth|-d --format|-F" << std::endl;
    std::cout << "  bitboard-selftest runs the bitboard selftest executable next to this benchmark binary" << std::endl;
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        PrintUsage(argv[0]);
        return 0;
    }

    const auto command = std::string(argv[1]);
    if (command == "play") {
        int depth = SurakartaMoveWeightUtil::DefaultDepth;
        double alpha = SurakartaMoveWeightUtil::DefaultAlpha;
        double beta = SurakartaMoveWeightUtil::DefaultBeta;
        int delay = 500;
        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "--depth") == 0 || strcmp(argv[i], "-d") == 0) {
                if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                    return 1;
                }
                depth = atoi(argv[i]);
            } else if (strcmp(argv[i], "--alpha") == 0 || strcmp(argv[i], "-a") == 0) {
                if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                    return 1;
                }
                alpha = atof(argv[i]);
            } else if (strcmp(argv[i], "--beta") == 0 || strcmp(argv[i], "-b") == 0) {
                if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                    return 1;
                }
                beta = atof(argv[i]);
            } else if (strcmp(argv[i], "--delay") == 0 || strcmp(argv[i], "-D") == 0) {
                if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                    return 1;
                }
                delay = atoi(argv[i]);
            } else {
                std::cerr << "Unknown option: " << argv[i] << std::endl;
                return 1;
            }
        }
        play(delay, true, depth, alpha, beta);
        return 0;
    }

    if (command == "bitboard-search") {
        return RunBitboardSearchCommand(argc - 2, argv + 2);
    }
    if (command == "bitboard-benchmark") {
        return RunBitboardBenchmarkCommand(argc - 2, argv + 2);
    }
    if (command == "bitboard-trace") {
        return RunBitboardTraceCommand(argc - 2, argv + 2);
    }
    if (command == "bitboard-train") {
        return RunBitboardTrainCommand(argc - 2, argv + 2);
    }
    if (command == "bitboard-eval") {
        return RunBitboardEvalCommand(argc - 2, argv + 2);
    }
    if (command == "bitboard-selftest") {
        return RunBitboardSelftestCommand(argv[0]);
    }
    if (command == "statistic") {
        int depth = SurakartaMoveWeightUtil::DefaultDepth;
        double alpha = SurakartaMoveWeightUtil::DefaultAlpha;
        double beta = SurakartaMoveWeightUtil::DefaultBeta;
        int concurrency = std::thread::hardware_concurrency();
        int total_rounds = INT_MAX;
        for (int i = 2; i < argc; ++i) {
            if (strcmp(argv[i], "--depth") == 0 || strcmp(argv[i], "-d") == 0) {
                if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                    return 1;
                }
                depth = atoi(argv[i]);
            } else if (strcmp(argv[i], "--alpha") == 0 || strcmp(argv[i], "-a") == 0) {
                if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                    return 1;
                }
                alpha = atof(argv[i]);
            } else if (strcmp(argv[i], "--beta") == 0 || strcmp(argv[i], "-b") == 0) {
                if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                    return 1;
                }
                beta = atof(argv[i]);
            } else if (strcmp(argv[i], "-j") == 0) {
                if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                    return 1;
                }
                concurrency = atoi(argv[i]);
            } else if (strcmp(argv[i], "-n") == 0) {
                if (!RequireValue(argc, argv, &i, argv[i - 0])) {
                    return 1;
                }
                total_rounds = atoi(argv[i]);
            } else {
                std::cerr << "Unknown option: " << argv[i] << std::endl;
                return 1;
            }
        }

        int cnt_play = 0;
        int cnt_win = 0;
        int cnt_lost = 0;
        const auto worker_threads = std::make_unique<std::thread[]>(concurrency);
        for (int i = 0; i < concurrency; i++) {
            worker_threads[i] = std::thread([&]() {
                while (true) {
                    const int result = play(0, false, depth, alpha, beta);
                    cnt_play++;
                    if (result == WIN_MIME)
                        cnt_win++;
                    if (result == WIN_RANDOM)
                        cnt_lost++;
                    if (cnt_play <= total_rounds)
                        printf(
                            "Win Rate: %6.2f   Not Lost Rate: %6.2f   Win: %5d Lost: %5d Stalemate: %5d\n",
                            100.0 * cnt_win / cnt_play,
                            100.0 * (cnt_play - cnt_lost) / cnt_play,
                            cnt_win,
                            cnt_lost,
                            cnt_play - cnt_win - cnt_lost);
                    if (cnt_play >= total_rounds)
                        break;
                }
            });
        }
        for (int i = 0; i < concurrency; i++) {
            worker_threads[i].join();
        }
        return 0;
    }

    PrintUsage(argv[0]);
    return 1;
}
