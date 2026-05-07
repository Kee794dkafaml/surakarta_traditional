#include "surakarta_search.h"

#include <algorithm>
#include <array>
#include <bit>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <random>
#include <sstream>
#include <thread>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "surakarta_game.h"

namespace surakarta::bitboard {

namespace {

bool IsMovePreferred(Move candidate, Move current) {
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

bool IsSearchResultPreferred(const SearchResult& candidate, const SearchResult& current) {
    if (candidate.depth != current.depth) {
        return candidate.depth > current.depth;
    }
    if (candidate.score != current.score) {
        return candidate.score > current.score;
    }
    return IsMovePreferred(candidate.best_move, current.best_move);
}

int RootBoundRank(TTBound bound) {
    switch (bound) {
        case TTBound::Exact:
            return 3;
        case TTBound::Lower:
            return 2;
        case TTBound::Upper:
            return 1;
        case TTBound::None:
        default:
            return 0;
    }
}

void AtomicMax(std::atomic<int>& target, int value) {
    auto observed = target.load(std::memory_order_relaxed);
    while (observed < value &&
           !target.compare_exchange_weak(observed, value,
                                         std::memory_order_relaxed,
                                         std::memory_order_relaxed)) {
    }
}

}  // namespace

struct SearchController::SharedSearchState {
    explicit SharedSearchState(const SearchLimits& limits_ref)
        : limits(limits_ref),
          start_time(std::chrono::steady_clock::now()) {}

    SearchLimits limits;
    std::chrono::steady_clock::time_point start_time;
    std::atomic<bool> stop{false};
    std::atomic<int> current_depth{0};
    std::atomic<std::uint64_t> nodes{0};
    std::atomic<std::uint64_t> qnodes{0};
    std::atomic<std::uint64_t> tt_hits{0};
    std::atomic<std::uint64_t> null_prunes{0};
    std::atomic<std::uint64_t> lmr_reductions{0};
    std::atomic<std::uint64_t> fail_highs{0};
    std::atomic<std::uint64_t> fail_lows{0};
    std::atomic<std::uint64_t> aspiration_retries{0};
    std::atomic<std::uint64_t> aspiration_attempts{0};
    std::atomic<std::uint64_t> full_window_researches{0};
    std::atomic<std::uint64_t> root_best_updates{0};
    std::atomic<std::uint64_t> tt_exact_cutoffs{0};
    std::atomic<std::uint64_t> tt_bound_cutoffs{0};
    std::atomic<std::uint64_t> beta_cutoffs{0};
    std::atomic<std::uint64_t> stale_root_tasks{0};
    std::atomic<std::uint64_t> aborted_root_tasks{0};
    std::atomic<std::uint64_t> worker_idle_spins{0};
    std::atomic<std::uint64_t> thread_spawn_count{0};
    mutable std::mutex best_mutex;
    SearchResult best_result{};
    mutable std::mutex diagnostics_mutex;
    SearchDiagnostics diagnostics{};
    std::atomic<int> diagnostic_attempt{0};

    bool TryVisitNode() {
        if (limits.node_limit == 0) {
            nodes.fetch_add(1, std::memory_order_relaxed);
            return true;
        }

        auto observed = nodes.load(std::memory_order_relaxed);
        while (true) {
            if (observed >= limits.node_limit) {
                stop.store(true, std::memory_order_relaxed);
                return false;
            }
            if (nodes.compare_exchange_weak(observed, observed + 1,
                                            std::memory_order_relaxed,
                                            std::memory_order_relaxed)) {
                return true;
            }
        }
    }

    SearchStats Stats() const {
        SearchStats stats{};
        stats.nodes = nodes.load(std::memory_order_relaxed);
        stats.qnodes = qnodes.load(std::memory_order_relaxed);
        stats.tt_hits = tt_hits.load(std::memory_order_relaxed);
        stats.null_prunes = null_prunes.load(std::memory_order_relaxed);
        stats.lmr_reductions = lmr_reductions.load(std::memory_order_relaxed);
        stats.fail_highs = fail_highs.load(std::memory_order_relaxed);
        stats.fail_lows = fail_lows.load(std::memory_order_relaxed);
        stats.aspiration_retries = aspiration_retries.load(std::memory_order_relaxed);
        stats.aspiration_attempts = aspiration_attempts.load(std::memory_order_relaxed);
        stats.full_window_researches = full_window_researches.load(std::memory_order_relaxed);
        stats.root_best_updates = root_best_updates.load(std::memory_order_relaxed);
        stats.tt_exact_cutoffs = tt_exact_cutoffs.load(std::memory_order_relaxed);
        stats.tt_bound_cutoffs = tt_bound_cutoffs.load(std::memory_order_relaxed);
        stats.beta_cutoffs = beta_cutoffs.load(std::memory_order_relaxed);
        stats.stale_root_tasks = stale_root_tasks.load(std::memory_order_relaxed);
        stats.aborted_root_tasks = aborted_root_tasks.load(std::memory_order_relaxed);
        stats.worker_idle_spins = worker_idle_spins.load(std::memory_order_relaxed);
        stats.thread_spawn_count = thread_spawn_count.load(std::memory_order_relaxed);
        return stats;
    }

    void PublishResult(const SearchResult& candidate) {
        std::lock_guard<std::mutex> lock(best_mutex);
        if (IsSearchResultPreferred(candidate, best_result)) {
            best_result = candidate;
        }
    }

    bool DiagnosticsEnabled() const {
        return limits.enable_diagnostics;
    }

    int RecordRootMoveDiagnostic(const SearchRootMoveDiagnostic& diagnostic) {
        if (!DiagnosticsEnabled()) {
            return -1;
        }
        std::lock_guard<std::mutex> lock(diagnostics_mutex);
        diagnostics.root_moves.push_back(diagnostic);
        return static_cast<int>(diagnostics.root_moves.size()) - 1;
    }

    void RecordRootBestUpdate(int diagnostic_event_id) {
        if (!DiagnosticsEnabled() || diagnostic_event_id < 0) {
            return;
        }
        std::lock_guard<std::mutex> lock(diagnostics_mutex);
        const auto index = static_cast<std::size_t>(diagnostic_event_id);
        if (index < diagnostics.root_moves.size()) {
            ++diagnostics.root_moves[index].root_best_updates;
        }
    }

    void RecordAspirationDiagnostic(const SearchAspirationDiagnostic& diagnostic) {
        if (!DiagnosticsEnabled()) {
            return;
        }
        std::lock_guard<std::mutex> lock(diagnostics_mutex);
        diagnostics.aspiration_attempts.push_back(diagnostic);
    }

    void RecordQsearchDiagnostic(const SearchQsearchDiagnostic& diagnostic) {
        if (!DiagnosticsEnabled() || diagnostic.qnodes == 0) {
            return;
        }
        std::lock_guard<std::mutex> lock(diagnostics_mutex);
        diagnostics.qsearch_sources.push_back(diagnostic);
    }

    SearchDiagnostics Diagnostics() const {
        std::lock_guard<std::mutex> lock(diagnostics_mutex);
        return diagnostics;
    }

    SearchSnapshot Snapshot(bool active) const {
        SearchSnapshot snapshot{};
        snapshot.active = active;
        snapshot.stop_requested = stop.load(std::memory_order_relaxed);

        SearchResult best;
        {
            std::lock_guard<std::mutex> lock(best_mutex);
            best = best_result;
        }

        snapshot.best_move = best.best_move;
        snapshot.pv_length = std::clamp(best.pv_length, 0, kMaxPly);
        for (int i = 0; i < snapshot.pv_length; ++i) {
            snapshot.pv[i] = best.pv[i];
        }
        snapshot.score = best.score;
        snapshot.depth = std::max(best.depth, current_depth.load(std::memory_order_relaxed));

        const auto stats = Stats();
        snapshot.nodes = stats.nodes;
        snapshot.qnodes = stats.qnodes;
        snapshot.tt_hits = stats.tt_hits;
        snapshot.null_prunes = stats.null_prunes;
        snapshot.lmr_reductions = stats.lmr_reductions;
        snapshot.fail_highs = stats.fail_highs;
        snapshot.fail_lows = stats.fail_lows;
        snapshot.aspiration_retries = stats.aspiration_retries;
        snapshot.aspiration_attempts = stats.aspiration_attempts;
        snapshot.full_window_researches = stats.full_window_researches;
        snapshot.root_best_updates = stats.root_best_updates;
        snapshot.tt_exact_cutoffs = stats.tt_exact_cutoffs;
        snapshot.tt_bound_cutoffs = stats.tt_bound_cutoffs;
        snapshot.beta_cutoffs = stats.beta_cutoffs;
        snapshot.stale_root_tasks = stats.stale_root_tasks;
        snapshot.aborted_root_tasks = stats.aborted_root_tasks;
        snapshot.worker_idle_spins = stats.worker_idle_spins;
        snapshot.thread_spawn_count = stats.thread_spawn_count;

        const auto elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - start_time).count();
        snapshot.nps = elapsed > 0.0 ? static_cast<std::uint64_t>(snapshot.nodes / elapsed) : 0;
        return snapshot;
    }
};

namespace {

constexpr int kInfinity = 1'000'000;
constexpr int kMateScore = 200'000;
constexpr int kTupleSentinel = -1;
constexpr int kHistoryMax = 32'000;
constexpr std::uint64_t kNodeBatchSize = 128;
constexpr double kTrainingWeightClamp = 4096.0;
constexpr double kTrainingTerminalRewardDefault = 1200.0;
constexpr int kTrainingMaxPlies = 256;

bool IsMateLikeScore(int score) {
    return std::abs(score) >= (kMateScore - kMaxPly - 1);
}

struct TupleDescriptor {
    std::uint8_t length{0};
    std::array<int, 6> squares{};
};

struct RootSearchResult {
    Move best_move{};
    int score{-kInfinity};
    TTBound bound{TTBound::None};
    std::array<Move, kMaxPly> pv{};
    int pv_length{0};
    int move_index{-1};
    int diagnostic_event_id{-1};
};

int TotalWeightCount();
Color WinnerColorFromPosition(const Position& position);

Color OppositeColor(Color color) {
    return color == Color::Black ? Color::White : Color::Black;
}

NTupleWeightHeader DefaultWeightHeader() {
    NTupleWeightHeader header{};
    header.tuple_set_hash = NTupleEvaluator::TupleSetHash();
    header.weight_count = static_cast<std::uint32_t>(TotalWeightCount());
    return header;
}

constexpr std::size_t kSerializedWeightHeaderBytes =
    sizeof(std::uint32_t) +
    sizeof(std::uint16_t) +
    sizeof(std::uint16_t) +
    sizeof(std::uint64_t) +
    sizeof(std::uint32_t);

static_assert(sizeof(NTupleWeightHeader) >= kSerializedWeightHeaderBytes,
              "NTupleWeightHeader cannot shrink below its serialized fields");

bool ReadWeightHeader(std::istream& input, NTupleWeightHeader& header) {
    header = NTupleWeightHeader{};
    input.read(reinterpret_cast<char*>(&header.magic), sizeof(header.magic));
    input.read(reinterpret_cast<char*>(&header.version), sizeof(header.version));
    input.read(reinterpret_cast<char*>(&header.reserved), sizeof(header.reserved));
    input.read(reinterpret_cast<char*>(&header.tuple_set_hash), sizeof(header.tuple_set_hash));
    input.read(reinterpret_cast<char*>(&header.weight_count), sizeof(header.weight_count));
    if (!input.good()) {
        return false;
    }

    constexpr auto padding_bytes = sizeof(NTupleWeightHeader) - kSerializedWeightHeaderBytes;
    if constexpr (padding_bytes > 0) {
        auto padding = std::array<char, padding_bytes>{};
        input.read(padding.data(), static_cast<std::streamsize>(padding.size()));
        if (!input.good()) {
            return false;
        }
    }

    return true;
}

bool WriteWeightHeader(std::ostream& output, const NTupleWeightHeader& header) {
    output.write(reinterpret_cast<const char*>(&header.magic), sizeof(header.magic));
    output.write(reinterpret_cast<const char*>(&header.version), sizeof(header.version));
    output.write(reinterpret_cast<const char*>(&header.reserved), sizeof(header.reserved));
    output.write(reinterpret_cast<const char*>(&header.tuple_set_hash), sizeof(header.tuple_set_hash));
    output.write(reinterpret_cast<const char*>(&header.weight_count), sizeof(header.weight_count));

    constexpr auto padding_bytes = sizeof(NTupleWeightHeader) - kSerializedWeightHeaderBytes;
    if constexpr (padding_bytes > 0) {
        auto padding = std::array<char, padding_bytes>{};
        output.write(padding.data(), static_cast<std::streamsize>(padding.size()));
    }

    return output.good();
}

bool ValidateWeightHeader(const NTupleWeightHeader& header, std::string* error_message) {
    const auto expected = DefaultWeightHeader();
    if (header.magic != expected.magic) {
        if (error_message != nullptr) {
            *error_message = "weight file magic does not match";
        }
        return false;
    }
    if (header.version != expected.version) {
        if (error_message != nullptr) {
            *error_message = "weight file version does not match";
        }
        return false;
    }
    if (header.tuple_set_hash != expected.tuple_set_hash) {
        if (error_message != nullptr) {
            *error_message = "weight file tuple hash does not match";
        }
        return false;
    }
    if (header.weight_count != expected.weight_count) {
        if (error_message != nullptr) {
            *error_message = "weight file count does not match";
        }
        return false;
    }
    return true;
}

constexpr TupleDescriptor MakeTuple(std::initializer_list<int> squares) {
    TupleDescriptor descriptor{};
    descriptor.length = static_cast<std::uint8_t>(squares.size());
    int index = 0;
    for (const auto square : squares) {
        descriptor.squares[index++] = square;
    }
    while (index < static_cast<int>(descriptor.squares.size())) {
        descriptor.squares[index++] = kTupleSentinel;
    }
    return descriptor;
}

const std::array<TupleDescriptor, 45>& TupleDescriptors() {
    static const auto tuples = std::array<TupleDescriptor, 45>{{
        MakeTuple({0, 1, 6, 7}),   MakeTuple({1, 2, 7, 8}),   MakeTuple({2, 3, 8, 9}),   MakeTuple({3, 4, 9, 10}),  MakeTuple({4, 5, 10, 11}),
        MakeTuple({6, 7, 12, 13}), MakeTuple({7, 8, 13, 14}), MakeTuple({8, 9, 14, 15}), MakeTuple({9, 10, 15, 16}), MakeTuple({10, 11, 16, 17}),
        MakeTuple({12, 13, 18, 19}), MakeTuple({13, 14, 19, 20}), MakeTuple({14, 15, 20, 21}), MakeTuple({15, 16, 21, 22}), MakeTuple({16, 17, 22, 23}),
        MakeTuple({18, 19, 24, 25}), MakeTuple({19, 20, 25, 26}), MakeTuple({20, 21, 26, 27}), MakeTuple({21, 22, 27, 28}), MakeTuple({22, 23, 28, 29}),
        MakeTuple({24, 25, 30, 31}), MakeTuple({25, 26, 31, 32}), MakeTuple({26, 27, 32, 33}), MakeTuple({27, 28, 33, 34}), MakeTuple({28, 29, 34, 35}),
        MakeTuple({0, 1, 2, 3, 4, 5}),    MakeTuple({6, 7, 8, 9, 10, 11}),  MakeTuple({12, 13, 14, 15, 16, 17}),
        MakeTuple({18, 19, 20, 21, 22, 23}), MakeTuple({24, 25, 26, 27, 28, 29}), MakeTuple({30, 31, 32, 33, 34, 35}),
        MakeTuple({0, 6, 12, 18, 24, 30}), MakeTuple({1, 7, 13, 19, 25, 31}), MakeTuple({2, 8, 14, 20, 26, 32}),
        MakeTuple({3, 9, 15, 21, 27, 33}), MakeTuple({4, 10, 16, 22, 28, 34}), MakeTuple({5, 11, 17, 23, 29, 35}),
        MakeTuple({1, 2, 6, 7, 8, 12}),    MakeTuple({3, 4, 9, 10, 11, 17}), MakeTuple({18, 24, 25, 26, 30, 31}),
        MakeTuple({24, 28, 29, 33, 34, 35}), MakeTuple({1, 7, 13, 19, 25, 31}), MakeTuple({4, 10, 16, 22, 28, 34}),
        MakeTuple({6, 12, 13, 14, 20, 26}), MakeTuple({9, 15, 21, 22, 23, 29}),
    }};
    return tuples;
}

const std::array<int, 45>& TupleOffsets() {
    static const auto offsets = []() {
        auto result = std::array<int, 45>{};
        int offset = 0;
        const auto& tuples = TupleDescriptors();
        for (std::size_t i = 0; i < tuples.size(); ++i) {
            result[i] = offset;
            int span = 1;
            for (int j = 0; j < tuples[i].length; ++j) {
                span *= 3;
            }
            offset += span;
        }
        return result;
    }();
    return offsets;
}

int TotalWeightCount() {
    static const int count = []() {
        const auto& tuples = TupleDescriptors();
        int total = 0;
        for (const auto& tuple : tuples) {
            int span = 1;
            for (int i = 0; i < tuple.length; ++i) {
                span *= 3;
            }
            total += span;
        }
        return total;
    }();
    return count;
}

int EncodeTuple(const Position& position, const TupleDescriptor& descriptor) {
    int encoded = 0;
    int factor = 1;
    for (int i = 0; i < descriptor.length; ++i) {
        const auto square = static_cast<Square>(descriptor.squares[i]);
        const auto color = position.board.ColorAt(square);
        const int state = color == Color::None ? 0 : color == Color::Black ? 1 : 2;
        encoded += state * factor;
        factor *= 3;
    }
    return encoded;
}

double EvaluateWeightsForSideToMove(const NTupleWeights& weights, const Position& position) {
    double score = static_cast<double>(MaterialBalance(position));
    const auto& tuples = TupleDescriptors();
    const auto& offsets = TupleOffsets();
    for (std::size_t i = 0; i < tuples.size(); ++i) {
        score += weights.values[offsets[i] + EncodeTuple(position, tuples[i])];
    }
    return position.SideToMove() == Color::Black ? score : -score;
}

std::int32_t QuantizeWeight(double value) {
    return static_cast<std::int32_t>(std::llround(std::clamp(value, -kTrainingWeightClamp, kTrainingWeightClamp)));
}

double TrainingTargetFromAfterMove(const Position& position_after_move,
                                   const TrainingStepContext& context,
                                   double terminal_reward) {
    const auto mover = OppositeColor(position_after_move.SideToMove());
    const auto enemy = position_after_move.SideToMove();
    if (position_after_move.board.Count(enemy) == 0) {
        return TrainingTerminalTarget(mover, mover, terminal_reward);
    }
    if (position_after_move.board.Count(mover) == 0) {
        return TrainingTerminalTarget(enemy, mover, terminal_reward);
    }
    if (position_after_move.no_capture_ply >= position_after_move.max_no_capture_round ||
        !context.next_has_legal_moves ||
        context.next_is_ply_cap) {
        return TrainingTerminalTarget(WinnerColorFromPosition(position_after_move), mover, terminal_reward);
    }
    return std::numeric_limits<double>::quiet_NaN();
}

std::string FormatMoveForPosition(const Position& position, Move move) {
    if (!move.IsValid()) {
        return "n/a";
    }

    auto stream = std::ostringstream{};
    stream << PositionAdapter::ToLegacyMove(move, ToLegacyColor(position.SideToMove()));
    return stream.str();
}

std::string FormatPrincipalVariation(
    const Position& root,
    const std::array<Move, kMaxPly>& pv,
    int pv_length) {
    if (pv_length <= 0) {
        return "n/a";
    }

    auto working = root;
    auto stream = std::ostringstream{};
    const auto safe_length = std::clamp(pv_length, 0, kMaxPly);
    for (int ply = 0; ply < safe_length; ++ply) {
        const auto move = pv[ply];
        if (!move.IsValid()) {
            break;
        }
        if (ply > 0) {
            stream << " | ";
        }
        stream << FormatMoveForPosition(working, move);
        auto undo = Undo{};
        MakeMove(working, move, undo);
    }
    const auto text = stream.str();
    return text.empty() ? "n/a" : text;
}

int EvaluateTerminal(const Position& position) {
    const auto side = position.SideToMove();
    const auto enemy = side == Color::Black ? Color::White : Color::Black;
    if (position.board.Count(side) == 0) {
        return -kMateScore + position.ply;
    }
    if (position.board.Count(enemy) == 0) {
        return kMateScore - position.ply;
    }
    if (position.no_capture_ply >= position.max_no_capture_round) {
        const int score = MaterialBalance(position);
        return side == Color::Black ? score : -score;
    }
    return std::numeric_limits<int>::min();
}

std::uint32_t PackMoveBits(Move move) {
    static_assert(sizeof(Move) == sizeof(std::uint32_t), "Move packing assumes 32-bit POD");
    std::uint32_t bits = 0;
    std::memcpy(&bits, &move, sizeof(bits));
    return bits;
}

Move UnpackMoveBits(std::uint32_t bits) {
    auto move = Move{};
    std::memcpy(&move, &bits, sizeof(bits));
    return move;
}

std::uint64_t PackMeta(int score, std::uint8_t depth, std::uint8_t generation, TTBound bound) {
    std::uint32_t score_bits = 0;
    std::memcpy(&score_bits, &score, sizeof(score_bits));
    return static_cast<std::uint64_t>(score_bits) |
           (static_cast<std::uint64_t>(depth) << 32) |
           (static_cast<std::uint64_t>(generation) << 40) |
           (static_cast<std::uint64_t>(static_cast<std::uint8_t>(bound)) << 48);
}

int UnpackScore(std::uint64_t meta) {
    const auto score_bits = static_cast<std::uint32_t>(meta & 0xFFFFFFFFull);
    int score = 0;
    std::memcpy(&score, &score_bits, sizeof(score));
    return score;
}

std::uint8_t UnpackDepth(std::uint64_t meta) {
    return static_cast<std::uint8_t>((meta >> 32) & 0xFFu);
}

std::uint8_t UnpackGeneration(std::uint64_t meta) {
    return static_cast<std::uint8_t>((meta >> 40) & 0xFFu);
}

TTBound UnpackBound(std::uint64_t meta) {
    return static_cast<TTBound>((meta >> 48) & 0xFFu);
}

int ClampPvLength(int pv_length) {
    return std::clamp(pv_length, 0, kMaxPly);
}

SearchResult ToSearchResult(const RootSearchResult& root_result, int depth, const SearchStats& stats) {
    auto result = SearchResult{};
    result.best_move = root_result.best_move;
    result.score = root_result.score;
    result.depth = depth;
    result.stats = stats;
    result.pv_length = ClampPvLength(root_result.pv_length);
    for (int i = 0; i < result.pv_length; ++i) {
        result.pv[i] = root_result.pv[i];
    }
    return result;
}

struct RootTaskTicket {
    int index{-1};
    int alpha{-kInfinity};
    int beta{kInfinity};
    std::uint64_t window_version{0};
};

struct WorkerContext {
    SearchController& controller;
    SearchController::SharedSearchState& shared;
    SearchLimits limits;
    std::array<SearchStack, kMaxPly + 2> stack{};
    std::array<std::array<std::array<int, kSquareCount>, kSquareCount>, 2> history{};
    std::array<std::array<Move, 2>, kMaxPly + 2> killer{};
    std::uint64_t pending_nodes{0};
    std::uint64_t pending_qnodes{0};
    int diagnostic_root_move_index{-1};
    Move diagnostic_root_move{};
    std::uint64_t diagnostic_qsearch_entries{0};
    std::uint64_t diagnostic_qsearch_capture_moves{0};

    explicit WorkerContext(SearchController& controller_ref,
                           SearchController::SharedSearchState& shared_ref,
                           const SearchLimits& limits_ref)
        : controller(controller_ref),
          shared(shared_ref),
          limits(limits_ref) {}

    ~WorkerContext() {
        FlushCounters();
    }

    void FlushCounters() {
        if (pending_nodes > 0) {
            shared.nodes.fetch_add(pending_nodes, std::memory_order_relaxed);
            pending_nodes = 0;
        }
        if (pending_qnodes > 0) {
            shared.qnodes.fetch_add(pending_qnodes, std::memory_order_relaxed);
            pending_qnodes = 0;
        }
    }

    bool TouchNode() {
        if (limits.node_limit == 0) {
            ++pending_nodes;
            if (pending_nodes >= kNodeBatchSize) {
                shared.nodes.fetch_add(pending_nodes, std::memory_order_relaxed);
                pending_nodes = 0;
            }
            return true;
        }

        FlushCounters();
        return shared.TryVisitNode();
    }

    void TouchQNode() {
        ++pending_qnodes;
        if (pending_qnodes >= kNodeBatchSize) {
            shared.qnodes.fetch_add(pending_qnodes, std::memory_order_relaxed);
            pending_qnodes = 0;
        }
    }

    std::uint64_t ObservedNodes() const {
        return shared.nodes.load(std::memory_order_relaxed) + pending_nodes;
    }

    std::uint64_t ObservedQNodes() const {
        return shared.qnodes.load(std::memory_order_relaxed) + pending_qnodes;
    }

    SearchDiagnosticLine BuildDiagnosticLine(int ply) const {
        auto line = SearchDiagnosticLine{};
        line.length = std::clamp(ply, 0, kMaxPly);
        for (int i = 0; i < line.length; ++i) {
            line.moves[i] = stack[i + 1].current_move;
        }
        return line;
    }

    int Evaluate(const Position& position) const {
        return controller.Evaluator().Evaluate(position);
    }

    void UpdatePv(int ply, Move move) {
        stack[ply].pv[0] = move;
        stack[ply].pv_length = 1;
        const int child_pv_length = ClampPvLength(stack[ply + 1].pv_length);
        const int copy_count = std::min(child_pv_length, kMaxPly - 1);
        if (copy_count > 0) {
            for (int i = 0; i < copy_count; ++i) {
                stack[ply].pv[i + 1] = stack[ply + 1].pv[i];
            }
            stack[ply].pv_length += copy_count;
        }
    }

    bool SameMove(Move lhs, Move rhs) const {
        return lhs.IsValid() &&
               rhs.IsValid() &&
               lhs.from == rhs.from &&
               lhs.to == rhs.to &&
               lhs.flags == rhs.flags &&
               lhs.aux == rhs.aux;
    }

    bool IsRecapture(Move move, int ply) const {
        return ply > 0 &&
               stack[ply].current_move.IsCapture() &&
               move.IsCapture() &&
               stack[ply].current_move.to == move.to;
    }

    int ScoreCapture(Move move, int ply) const {
        const auto side_index = static_cast<int>(ply & 1);
        auto score = 500'000 + std::min(history[side_index][move.from][move.to], 15'000);
        if (IsRecapture(move, ply)) {
            score += 20'000;
        }
        if (SameMove(move, killer[ply][0])) {
            score += 5'000;
        } else if (SameMove(move, killer[ply][1])) {
            score += 4'000;
        }
        return score;
    }

    void ScoreMoves(MoveList& move_list, Move tt_move, Move preferred_move, int ply) {
        const auto side_index = static_cast<int>(ply & 1);
        for (int i = 0; i < move_list.size; ++i) {
            auto score = 0;
            const auto move = move_list.moves[i];
            if (SameMove(move, tt_move)) {
                score = 1'000'000;
            } else if (ply == 0 && SameMove(move, preferred_move)) {
                score = 900'000;
            } else if (move.IsCapture()) {
                score = ScoreCapture(move, ply);
            } else if (SameMove(move, killer[ply][0])) {
                score = 400'000;
            } else if (SameMove(move, killer[ply][1])) {
                score = 399'000;
            } else {
                score = history[side_index][move.from][move.to];
            }
            move_list.scores[i] = score;
        }
    }

    void SortMoves(MoveList& move_list) {
        for (int i = 0; i < move_list.size; ++i) {
            int best = i;
            for (int j = i + 1; j < move_list.size; ++j) {
                if (move_list.scores[j] > move_list.scores[best]) {
                    best = j;
                }
            }
            if (best != i) {
                std::swap(move_list.moves[i], move_list.moves[best]);
                std::swap(move_list.scores[i], move_list.scores[best]);
            }
        }
    }

    void UpdateHeuristics(Move move, int depth, int ply) {
        auto& entry = history[ply & 1][move.from][move.to];
        entry = std::min(kHistoryMax, entry + depth * depth);
        if (move.IsCapture()) {
            return;
        }
        if (!SameMove(move, killer[ply][0])) {
            killer[ply][1] = killer[ply][0];
            killer[ply][0] = move;
        }
    }

    int Quiescence(Position& position, int alpha, int beta, int ply) {
        stack[ply].pv_length = 0;
        TouchQNode();
        const int terminal = EvaluateTerminal(position);
        if (terminal != std::numeric_limits<int>::min()) {
            return terminal;
        }

        const int stand_pat = Evaluate(position);
        if (stand_pat >= beta) {
            shared.beta_cutoffs.fetch_add(1, std::memory_order_relaxed);
            return beta;
        }
        if (stand_pat > alpha) {
            alpha = stand_pat;
        }

        auto move_list = MoveList{};
        GenerateMoves(position, move_list, true);
        if (shared.DiagnosticsEnabled()) {
            diagnostic_qsearch_capture_moves += static_cast<std::uint64_t>(move_list.size);
        }
        ScoreMoves(move_list, Move{}, Move{}, ply);
        SortMoves(move_list);

        for (int i = 0; i < move_list.size; ++i) {
            const auto move = move_list.moves[i];
            Undo undo;
            MakeMove(position, move, undo);
            stack[ply + 1].current_move = move;
            const int score = -Quiescence(position, -beta, -alpha, ply + 1);
            UnmakeMove(position, move, undo);
            if (shared.stop.load(std::memory_order_relaxed)) {
                return alpha;
            }
            if (score >= beta) {
                shared.beta_cutoffs.fetch_add(1, std::memory_order_relaxed);
                return beta;
            }
            if (score > alpha) {
                alpha = score;
                UpdatePv(ply, move);
            }
        }

        return alpha;
    }

    int Pvs(Position& position, int depth, int alpha, int beta, int ply, bool pv_node, bool allow_null) {
        stack[ply].pv_length = 0;
        if (shared.stop.load(std::memory_order_relaxed)) {
            return alpha;
        }
        if (!TouchNode()) {
            return alpha;
        }

        const int terminal = EvaluateTerminal(position);
        if (terminal != std::numeric_limits<int>::min()) {
            stack[ply].pv_length = 0;
            return terminal;
        }
        if (depth <= 0) {
            stack[ply].pv_length = 0;
            if (!shared.DiagnosticsEnabled()) {
                return Quiescence(position, alpha, beta, ply);
            }

            const auto qnodes_before = ObservedQNodes();
            const auto capture_moves_before = diagnostic_qsearch_capture_moves;
            ++diagnostic_qsearch_entries;
            const auto line = BuildDiagnosticLine(ply);
            const int score = Quiescence(position, alpha, beta, ply);
            const auto qnodes_after = ObservedQNodes();

            auto diagnostic = SearchQsearchDiagnostic{};
            diagnostic.depth = shared.current_depth.load(std::memory_order_relaxed);
            diagnostic.attempt = shared.diagnostic_attempt.load(std::memory_order_relaxed);
            diagnostic.root_move_index = diagnostic_root_move_index;
            diagnostic.root_move = diagnostic_root_move;
            diagnostic.ply = ply;
            diagnostic.score = score;
            diagnostic.qnodes = qnodes_after - qnodes_before;
            diagnostic.capture_moves = diagnostic_qsearch_capture_moves - capture_moves_before;
            diagnostic.line = line;
            shared.RecordQsearchDiagnostic(diagnostic);
            return score;
        }

        const int original_alpha = alpha;
        auto tt_move = Move{};
        const auto tt_probe = controller.Table().Probe(position.zobrist_key);
        if (tt_probe.found) {
            shared.tt_hits.fetch_add(1, std::memory_order_relaxed);
            tt_move = tt_probe.entry.best_move;
            if (tt_probe.entry.depth >= depth) {
                if (tt_probe.entry.bound == TTBound::Exact) {
                    shared.tt_exact_cutoffs.fetch_add(1, std::memory_order_relaxed);
                    return tt_probe.entry.score;
                }
                if (tt_probe.entry.bound == TTBound::Lower) {
                    alpha = std::max(alpha, tt_probe.entry.score);
                } else if (tt_probe.entry.bound == TTBound::Upper) {
                    beta = std::min(beta, tt_probe.entry.score);
                }
                if (alpha >= beta) {
                    shared.tt_bound_cutoffs.fetch_add(1, std::memory_order_relaxed);
                    return tt_probe.entry.score;
                }
            }
        }

        if (!pv_node &&
            limits.enable_null_move &&
            allow_null &&
            depth >= 4 &&
            position.board.Count(position.SideToMove()) > 2 &&
            HasAnyQuietMove(position)) {
            Undo undo;
            MakeNullMove(position, undo);
            const int reduction = 2 + depth / 5;
            const int score = -Pvs(position, depth - reduction - 1, -beta, -beta + 1, ply + 1, false, false);
            UnmakeNullMove(position, undo);
            if (score >= beta) {
                shared.null_prunes.fetch_add(1, std::memory_order_relaxed);
                shared.beta_cutoffs.fetch_add(1, std::memory_order_relaxed);
                return beta;
            }
        }

        auto move_list = MoveList{};
        GenerateMoves(position, move_list);
        if (move_list.size == 0) {
            stack[ply].pv_length = 0;
            return Evaluate(position);
        }

        ScoreMoves(move_list, tt_move, Move{}, ply);
        SortMoves(move_list);

        Move best_move{};
        int best_score = -kInfinity;
        bool searched_first = false;

        for (int i = 0; i < move_list.size; ++i) {
            const auto move = move_list.moves[i];
            Undo undo;
            MakeMove(position, move, undo);
            stack[ply + 1].current_move = move;

            int score = -kInfinity;
            int reduction = 0;
            if (limits.enable_lmr &&
                !pv_node &&
                !move.IsCapture() &&
                depth >= 3 &&
                i >= 4 &&
                !SameMove(move, killer[ply][0]) &&
                !SameMove(move, tt_move)) {
                reduction = 1;
                if (depth >= 6 && i >= 10) {
                    ++reduction;
                }
                shared.lmr_reductions.fetch_add(1, std::memory_order_relaxed);
            }

            if (!searched_first) {
                score = -Pvs(position, depth - 1, -beta, -alpha, ply + 1, true, true);
                searched_first = true;
            } else {
                score = -Pvs(position, depth - 1 - reduction, -alpha - 1, -alpha, ply + 1, false, true);
                if (reduction > 0 && score > alpha) {
                    score = -Pvs(position, depth - 1, -alpha - 1, -alpha, ply + 1, false, true);
                }
                if (score > alpha && score < beta) {
                    score = -Pvs(position, depth - 1, -beta, -alpha, ply + 1, pv_node, true);
                }
            }
            UnmakeMove(position, move, undo);

            if (shared.stop.load(std::memory_order_relaxed)) {
                return alpha;
            }
            if (score > best_score) {
                best_score = score;
                best_move = move;
            }
            if (score > alpha) {
                alpha = score;
                UpdatePv(ply, move);
            }
            if (alpha >= beta) {
                shared.beta_cutoffs.fetch_add(1, std::memory_order_relaxed);
                UpdateHeuristics(move, depth, ply);
                break;
            }
        }

        TTBound bound = TTBound::Exact;
        if (best_score <= original_alpha) {
            bound = TTBound::Upper;
        } else if (best_score >= beta) {
            bound = TTBound::Lower;
        }
        controller.Table().Store(position.zobrist_key, depth, best_score, bound, best_move);
        return best_score;
    }

    RootSearchResult SearchRootMove(const Position& root, Move move, int move_index, int depth, int alpha, int beta) {
        auto result = RootSearchResult{};
        result.best_move = move;
        result.bound = TTBound::Exact;
        result.pv[0] = move;
        result.pv_length = 1;
        result.move_index = move_index;

        const int original_alpha = alpha;
        const int original_beta = beta;
        const auto previous_root_move_index = diagnostic_root_move_index;
        const auto previous_root_move = diagnostic_root_move;
        diagnostic_root_move_index = move_index;
        diagnostic_root_move = move;
        const auto nodes_before = ObservedNodes();
        const auto qnodes_before = ObservedQNodes();
        const auto tt_hits_before = shared.tt_hits.load(std::memory_order_relaxed);
        const auto beta_cutoffs_before = shared.beta_cutoffs.load(std::memory_order_relaxed);
        const auto qsearch_entries_before = diagnostic_qsearch_entries;
        const auto qsearch_capture_moves_before = diagnostic_qsearch_capture_moves;

        auto working = root;
        Undo undo;
        MakeMove(working, move, undo);
        stack[1].pv_length = 0;
        stack[1].current_move = move;
        const int score = -Pvs(working, depth - 1, -beta, -alpha, 1, true, true);
        UnmakeMove(working, move, undo);
        FlushCounters();

        result.score = score;
        if (score <= original_alpha) {
            result.bound = TTBound::Upper;
        } else if (score >= original_beta) {
            result.bound = TTBound::Lower;
        }
        const int child_pv_length = ClampPvLength(stack[1].pv_length);
        for (int i = 0; i < child_pv_length && i + 1 < kMaxPly; ++i) {
            result.pv[i + 1] = stack[1].pv[i];
        }
        result.pv_length = std::min(kMaxPly, child_pv_length + 1);
        if (shared.DiagnosticsEnabled()) {
            auto diagnostic = SearchRootMoveDiagnostic{};
            diagnostic.depth = shared.current_depth.load(std::memory_order_relaxed);
            diagnostic.attempt = shared.diagnostic_attempt.load(std::memory_order_relaxed);
            diagnostic.move_index = move_index;
            diagnostic.move = move;
            diagnostic.alpha = original_alpha;
            diagnostic.beta = original_beta;
            diagnostic.score = score;
            diagnostic.bound = static_cast<int>(result.bound);
            diagnostic.nodes = ObservedNodes() - nodes_before;
            diagnostic.qnodes = ObservedQNodes() - qnodes_before;
            diagnostic.fail_highs = result.bound == TTBound::Lower ? 1 : 0;
            diagnostic.fail_lows = result.bound == TTBound::Upper ? 1 : 0;
            diagnostic.tt_hits = shared.tt_hits.load(std::memory_order_relaxed) - tt_hits_before;
            diagnostic.beta_cutoffs =
                shared.beta_cutoffs.load(std::memory_order_relaxed) - beta_cutoffs_before;
            diagnostic.qsearch_entries = diagnostic_qsearch_entries - qsearch_entries_before;
            diagnostic.qsearch_capture_moves =
                diagnostic_qsearch_capture_moves - qsearch_capture_moves_before;
            diagnostic.pv.length = ClampPvLength(result.pv_length);
            for (int i = 0; i < diagnostic.pv.length; ++i) {
                diagnostic.pv.moves[i] = result.pv[i];
            }
            result.diagnostic_event_id = shared.RecordRootMoveDiagnostic(diagnostic);
        }
        diagnostic_root_move_index = previous_root_move_index;
        diagnostic_root_move = previous_root_move;
        return result;
    }
};

struct RootSearchTask {
    Position root;
    MoveList root_moves{};
    int depth{0};
    std::atomic<int> next_index{1};
    std::atomic<int> shared_alpha{-kInfinity};
    std::atomic<int> shared_beta{kInfinity};
    std::atomic<std::uint64_t> window_version{0};
    std::atomic<bool> cutoff{false};
    std::mutex best_mutex;
    RootSearchResult best{};
};

bool IsRootResultPreferred(const RootSearchResult& candidate, const RootSearchResult& current) {
    if (candidate.score != current.score) {
        return candidate.score > current.score;
    }

    const auto candidate_rank = RootBoundRank(candidate.bound);
    const auto current_rank = RootBoundRank(current.bound);
    if (candidate_rank != current_rank) {
        return candidate_rank > current_rank;
    }

    return candidate.bound == TTBound::Exact &&
           IsMovePreferred(candidate.best_move, current.best_move);
}

void MaybeUpdateRootBest(RootSearchTask& task,
                         SearchController::SharedSearchState& shared,
                         const RootSearchResult& candidate) {
    bool is_better = false;
    auto published = RootSearchResult{};
    {
        std::lock_guard<std::mutex> lock(task.best_mutex);
        if (IsRootResultPreferred(candidate, task.best)) {
            task.best = candidate;
            is_better = true;
        }
        published = task.best;
    }

    if (!is_better) {
        return;
    }

    shared.root_best_updates.fetch_add(1, std::memory_order_relaxed);
    shared.RecordRootBestUpdate(candidate.diagnostic_event_id);
    const auto previous_alpha = task.shared_alpha.load(std::memory_order_relaxed);
    if (published.score > previous_alpha) {
        AtomicMax(task.shared_alpha, published.score);
        task.window_version.fetch_add(1, std::memory_order_relaxed);
    }
    if (published.score >= task.shared_beta.load(std::memory_order_relaxed)) {
        task.cutoff.store(true, std::memory_order_relaxed);
    }
    shared.PublishResult(ToSearchResult(published, task.depth, shared.Stats()));
}

bool TryAcquireRootTask(RootSearchTask& task,
                        SearchController::SharedSearchState& shared,
                        RootTaskTicket* ticket) {
    while (!shared.stop.load(std::memory_order_relaxed)) {
        if (task.cutoff.load(std::memory_order_relaxed)) {
            return false;
        }
        const int index = task.next_index.fetch_add(1, std::memory_order_relaxed);
        if (index >= task.root_moves.size) {
            return false;
        }
        ticket->index = index;
        ticket->alpha = task.shared_alpha.load(std::memory_order_relaxed);
        ticket->beta = task.shared_beta.load(std::memory_order_relaxed);
        ticket->window_version = task.window_version.load(std::memory_order_relaxed);
        if (task.cutoff.load(std::memory_order_relaxed)) {
            shared.aborted_root_tasks.fetch_add(1, std::memory_order_relaxed);
            return false;
        }
        return true;
    }
    return false;
}

void ConsumeRootQueue(WorkerContext& context,
                      SearchController::SharedSearchState& shared,
                      RootSearchTask& task) {
    auto ticket = RootTaskTicket{};
    while (TryAcquireRootTask(task, shared, &ticket)) {
        const auto candidate = context.SearchRootMove(task.root,
                                                      task.root_moves.moves[ticket.index],
                                                      ticket.index,
                                                      task.depth,
                                                      ticket.alpha,
                                                      ticket.beta);
        if (ticket.window_version != task.window_version.load(std::memory_order_relaxed)) {
            shared.stale_root_tasks.fetch_add(1, std::memory_order_relaxed);
        }
        MaybeUpdateRootBest(task, shared, candidate);
        if (task.cutoff.load(std::memory_order_relaxed)) {
            return;
        }
    }
}

struct RootWorkerPool {
    SearchController& controller;
    SearchController::SharedSearchState& shared;
    SearchLimits limits;
    int worker_count{0};
    std::mutex control_mutex;
    std::condition_variable start_cv;
    std::condition_variable done_cv;
    RootSearchTask* active_task{nullptr};
    std::uint64_t generation{0};
    int completed_workers{0};
    bool shutdown{false};
    std::vector<std::thread> workers;

    explicit RootWorkerPool(SearchController& controller_ref,
                            SearchController::SharedSearchState& shared_ref,
                            const SearchLimits& limits_ref)
        : controller(controller_ref),
          shared(shared_ref),
          limits(limits_ref),
          worker_count(std::max(0, limits_ref.threads - 1)) {
        workers.reserve(static_cast<std::size_t>(worker_count));
        for (int i = 0; i < worker_count; ++i) {
            shared.thread_spawn_count.fetch_add(1, std::memory_order_relaxed);
            workers.emplace_back([this]() { WorkerLoop(); });
        }
    }

    ~RootWorkerPool() {
        {
            std::lock_guard<std::mutex> lock(control_mutex);
            shutdown = true;
            ++generation;
        }
        start_cv.notify_all();
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void WorkerLoop() {
        auto context = WorkerContext(controller, shared, limits);
        auto seen_generation = std::uint64_t{0};
        while (true) {
            auto lock = std::unique_lock<std::mutex>(control_mutex);
            start_cv.wait(lock, [&]() { return shutdown || generation != seen_generation; });
            if (shutdown) {
                return;
            }

            seen_generation = generation;
            auto* task = active_task;
            lock.unlock();

            if (task != nullptr) {
                ConsumeRootQueue(context, shared, *task);
            }

            lock.lock();
            ++completed_workers;
            if (completed_workers >= worker_count) {
                done_cv.notify_one();
            }
        }
    }

    void Run(RootSearchTask& task, WorkerContext& main_context) {
        if (worker_count <= 0) {
            ConsumeRootQueue(main_context, shared, task);
            return;
        }

        {
            std::lock_guard<std::mutex> lock(control_mutex);
            active_task = &task;
            completed_workers = 0;
            ++generation;
        }
        start_cv.notify_all();

        ConsumeRootQueue(main_context, shared, task);

        auto lock = std::unique_lock<std::mutex>(control_mutex);
        done_cv.wait(lock, [&]() { return completed_workers >= worker_count; });
        active_task = nullptr;
    }
};

SearchResult SearchDepthAttempt(SearchController& controller,
                                const Position& root,
                                SearchController::SharedSearchState& shared,
                                WorkerContext& main_context,
                                RootWorkerPool* worker_pool,
                                int depth,
                                int alpha,
                                int beta,
                                Move preferred_root_move) {
    shared.current_depth.store(depth, std::memory_order_relaxed);

    auto root_moves = MoveList{};
    GenerateMoves(root, root_moves);
    if (root_moves.size == 0) {
        auto result = SearchResult{};
        result.depth = depth;
        result.score = controller.Evaluator().Evaluate(root);
        result.stats = shared.Stats();
        shared.PublishResult(result);
        return result;
    }

    auto tt_move = Move{};
    const auto tt_probe = controller.Table().Probe(root.zobrist_key);
    if (tt_probe.found) {
        tt_move = tt_probe.entry.best_move;
    }
    main_context.ScoreMoves(root_moves, tt_move, preferred_root_move, 0);
    main_context.SortMoves(root_moves);

    auto task = RootSearchTask{};
    task.root = root;
    task.root_moves = root_moves;
    task.depth = depth;
    task.shared_alpha.store(alpha, std::memory_order_relaxed);
    task.shared_beta.store(beta, std::memory_order_relaxed);

    const auto first_candidate = main_context.SearchRootMove(root, root_moves.moves[0], 0, depth, alpha, beta);
    MaybeUpdateRootBest(task, shared, first_candidate);

    if (!shared.stop.load(std::memory_order_relaxed) &&
        root_moves.size > 1 &&
        !task.cutoff.load(std::memory_order_relaxed)) {
        if (worker_pool != nullptr) {
            worker_pool->Run(task, main_context);
        } else {
            ConsumeRootQueue(main_context, shared, task);
        }
    }

    if (task.cutoff.load(std::memory_order_relaxed)) {
        const auto next_index = std::min(task.root_moves.size, task.next_index.load(std::memory_order_relaxed));
        const auto aborted = std::max(0, task.root_moves.size - next_index);
        if (aborted > 0) {
            shared.aborted_root_tasks.fetch_add(static_cast<std::uint64_t>(aborted), std::memory_order_relaxed);
        }
    }

    main_context.FlushCounters();
    auto result = ToSearchResult(task.best, depth, shared.Stats());
    result.stats = shared.Stats();
    return result;
}

int ReferenceAlphaBeta(const NTupleEvaluator& evaluator, Position& position, int depth, int alpha, int beta, SearchStats& stats) {
    ++stats.nodes;
    const int terminal = EvaluateTerminal(position);
    if (terminal != std::numeric_limits<int>::min()) {
        return terminal;
    }
    if (depth <= 0) {
        ++stats.qnodes;
        return evaluator.Evaluate(position);
    }

    auto move_list = MoveList{};
    GenerateMoves(position, move_list);
    if (move_list.size == 0) {
        return evaluator.Evaluate(position);
    }

    int best = -kInfinity;
    for (int i = 0; i < move_list.size; ++i) {
        Undo undo;
        MakeMove(position, move_list.moves[i], undo);
        const int score = -ReferenceAlphaBeta(evaluator, position, depth - 1, -beta, -alpha, stats);
        UnmakeMove(position, move_list.moves[i], undo);
        best = std::max(best, score);
        alpha = std::max(alpha, score);
        if (alpha >= beta) {
            break;
        }
    }
    return best;
}

}  // namespace

TranspositionTable::TranspositionTable(std::size_t cluster_count)
    : cluster_count_(cluster_count),
      clusters_(std::make_unique<Cluster[]>(cluster_count)) {}

void TranspositionTable::Clear() {
    for (std::size_t i = 0; i < cluster_count_; ++i) {
        for (auto& entry : clusters_[i].entries) {
            entry.key.store(0, std::memory_order_relaxed);
            entry.meta.store(0, std::memory_order_relaxed);
            entry.move.store(0, std::memory_order_relaxed);
        }
    }
    generation_ = 1;
}

void TranspositionTable::NewSearch() {
    ++generation_;
}

TTProbe TranspositionTable::Probe(std::uint64_t key) const {
    const auto& cluster = clusters_[key % cluster_count_];
    for (const auto& entry : cluster.entries) {
        const auto loaded_key = entry.key.load(std::memory_order_acquire);
        if (loaded_key != key || loaded_key == 0) {
            continue;
        }

        const auto meta = entry.meta.load(std::memory_order_acquire);
        const auto move_bits = entry.move.load(std::memory_order_relaxed);
        if (entry.key.load(std::memory_order_acquire) != key) {
            continue;
        }

        TTEntry snapshot{};
        snapshot.key = loaded_key;
        snapshot.score = UnpackScore(meta);
        snapshot.depth = UnpackDepth(meta);
        snapshot.generation = UnpackGeneration(meta);
        snapshot.bound = UnpackBound(meta);
        snapshot.best_move = UnpackMoveBits(move_bits);
        if (snapshot.bound != TTBound::None) {
            return {true, snapshot};
        }
    }
    return {};
}

void TranspositionTable::Store(std::uint64_t key, int depth, int score, TTBound bound, Move best_move) {
    auto& cluster = clusters_[key % cluster_count_];
    auto* replacement = &cluster.entries[0];
    const auto current_generation = generation_.load(std::memory_order_relaxed);
    auto replacement_meta = replacement->meta.load(std::memory_order_relaxed);
    auto replacement_score = std::numeric_limits<int>::min();

    for (auto& entry : cluster.entries) {
        const auto entry_key = entry.key.load(std::memory_order_acquire);
        const auto entry_meta = entry.meta.load(std::memory_order_relaxed);
        const auto entry_depth = static_cast<int>(UnpackDepth(entry_meta));
        const auto entry_bound = UnpackBound(entry_meta);

        if (entry_key == key) {
            replacement = &entry;
            break;
        }
        if (entry_key == 0 || entry_bound == TTBound::None) {
            replacement = &entry;
            break;
        }
        const auto entry_generation = UnpackGeneration(entry_meta);
        const auto age = static_cast<int>(static_cast<std::uint8_t>(current_generation - entry_generation));
        auto bound_protection = 0;
        if (entry_bound == TTBound::Exact) {
            bound_protection = 512;
        } else if (entry_bound == TTBound::Lower || entry_bound == TTBound::Upper) {
            bound_protection = 256;
        }
        const auto candidate_score = age * 1024 - entry_depth * 32 - bound_protection;
        if (candidate_score > replacement_score) {
            replacement = &entry;
            replacement_score = candidate_score;
        }
    }

    replacement->key.store(0, std::memory_order_relaxed);
    replacement->meta.store(PackMeta(score,
                                     static_cast<std::uint8_t>(std::max(0, std::min(depth, 255))),
                                     current_generation,
                                     bound),
                            std::memory_order_relaxed);
    replacement->move.store(PackMoveBits(best_move), std::memory_order_relaxed);
    replacement->key.store(key, std::memory_order_release);
}

NTupleWeights::NTupleWeights()
    : header(DefaultWeightHeader()),
      values(header.weight_count, 0.0) {}

bool NTupleWeights::LoadBinary(const std::string& file_name) {
    auto input = std::ifstream(file_name, std::ios::binary);
    if (!input.is_open()) {
        return false;
    }

    auto external_header = NTupleWeightHeader{};
    if (!ReadWeightHeader(input, external_header)) {
        return false;
    }

    auto error_message = std::string{};
    if (!ValidateWeightHeader(external_header, &error_message)) {
        return false;
    }

    auto quantized = std::vector<std::int32_t>(external_header.weight_count, 0);
    input.read(reinterpret_cast<char*>(quantized.data()),
               static_cast<std::streamsize>(sizeof(std::int32_t) * quantized.size()));
    if (!input.good()) {
        return false;
    }

    header = external_header;
    values.resize(header.weight_count);
    for (std::size_t i = 0; i < values.size(); ++i) {
        values[i] = static_cast<double>(quantized[i]);
    }
    return true;
}

bool NTupleWeights::SaveBinary(const std::string& file_name) const {
    auto header_error = std::string{};
    if (!ValidateWeightHeader(header, &header_error) ||
        values.size() != static_cast<std::size_t>(header.weight_count)) {
        return false;
    }

    const auto path = std::filesystem::path(file_name);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    auto output = std::ofstream(file_name, std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }

    auto quantized = std::vector<std::int32_t>(values.size(), 0);
    for (std::size_t i = 0; i < values.size(); ++i) {
        quantized[i] = QuantizeWeight(values[i]);
    }

    if (!WriteWeightHeader(output, header)) {
        return false;
    }
    output.write(reinterpret_cast<const char*>(quantized.data()),
                 static_cast<std::streamsize>(sizeof(std::int32_t) * quantized.size()));
    return output.good();
}

int NTupleWeights::Evaluate(const Position& position) const {
    return static_cast<int>(std::llround(EvaluateWeightsForSideToMove(*this, position)));
}

void NTupleWeights::EnumerateActiveWeightIndices(const Position& position, std::vector<int>& out) const {
    out.clear();
    const auto& tuples = TupleDescriptors();
    const auto& offsets = TupleOffsets();
    out.reserve(tuples.size());
    for (std::size_t i = 0; i < tuples.size(); ++i) {
        out.push_back(offsets[i] + EncodeTuple(position, tuples[i]));
    }
}

double EvaluateTrainingValueForSideToMove(const NTupleWeights& weights, const Position& position) {
    return EvaluateWeightsForSideToMove(weights, position);
}

double TrainingTerminalTarget(Color winner, Color side_to_update, double terminal_reward) {
    if (winner == Color::None) {
        return 0.0;
    }
    return winner == side_to_update ? terminal_reward : -terminal_reward;
}

Position BuildNearTerminalTrainingPosition(int sample_index) {
    const auto mover = (sample_index & 1) == 0 ? Color::Black : Color::White;
    const auto enemy = mover == Color::Black ? Color::White : Color::Black;

    auto position = Position{};
    position.board.SetPiece(mover, MakeSquare(5, 1));
    position.board.SetPiece(enemy, MakeSquare(1, 1));
    position.side_to_move = static_cast<std::uint8_t>(mover);
    position.no_capture_ply = 0;
    position.ply = 0;
    position.max_no_capture_round = MAX_NO_CAPTURE_ROUND;
    position.eval_cache = MaterialBalance(position);
    position.zobrist_key = ComputeZobrist(position);
    return position;
}

void ResetTrainingTraces(std::vector<double>& traces) {
    std::fill(traces.begin(), traces.end(), 0.0);
}

bool ApplyTrainingStep(NTupleWeights& weights,
                       std::vector<double>& traces,
                       const Position& current,
                       const Position& next,
                       const TrainingStepContext& context,
                       double alpha,
                       double lambda,
                       TrainingStepResult* result,
                       double terminal_reward,
                       double td_error_clip) {
    if (traces.size() != weights.values.size()) {
        return false;
    }

    auto active_indices = std::vector<int>{};
    weights.EnumerateActiveWeightIndices(current, active_indices);
    const auto current_value = EvaluateTrainingValueForSideToMove(weights, current);

    auto target_value = TrainingTargetFromAfterMove(next, context, terminal_reward);
    const auto terminal_target = !std::isnan(target_value);
    if (!terminal_target) {
        target_value = -EvaluateTrainingValueForSideToMove(weights, next);
    }

    for (auto& trace : traces) {
        trace *= -lambda;
    }
    const auto feature_sign = current.SideToMove() == Color::Black ? 1.0 : -1.0;
    for (const auto index : active_indices) {
        const auto trace_index = static_cast<std::size_t>(index);
        if (trace_index >= traces.size()) {
            return false;
        }
        traces[trace_index] += feature_sign;
    }

    const auto raw_delta = target_value - current_value;
    const auto delta = td_error_clip > 0.0
                           ? std::clamp(raw_delta, -td_error_clip, td_error_clip)
                           : raw_delta;
    auto step_abs_weight_delta = 0.0;
    auto changed_weight_count = std::uint64_t{0};
    for (std::size_t i = 0; i < weights.values.size(); ++i) {
        if (traces[i] == 0.0) {
            continue;
        }
        const auto change = alpha * delta * traces[i];
        if (change == 0.0) {
            continue;
        }
        weights.values[i] = std::clamp(weights.values[i] + change, -kTrainingWeightClamp, kTrainingWeightClamp);
        step_abs_weight_delta += std::abs(change);
        ++changed_weight_count;
    }

    if (result != nullptr) {
        result->current_value = current_value;
        result->target_value = target_value;
        result->td_error = delta;
        result->abs_weight_delta = step_abs_weight_delta;
        result->changed_weight_count = changed_weight_count;
        result->terminal_target = terminal_target;
    }
    return true;
}

NTupleEvaluator::NTupleEvaluator()
    : header_(DefaultWeightHeader()),
      weights_(std::make_unique<int[]>(header_.weight_count)) {
    LoadBootstrapWeights();
}

bool NTupleEvaluator::LoadWeights(const std::string& file_name) {
    auto loaded = NTupleWeights{};
    if (!loaded.LoadBinary(file_name)) {
        return false;
    }
    return ApplyWeights(loaded);
}

bool NTupleEvaluator::SaveWeights(const std::string& file_name) const {
    return ExportWeights().SaveBinary(file_name);
}

bool NTupleEvaluator::ApplyWeights(const NTupleWeights& weights) {
    auto error_message = std::string{};
    if (!ValidateWeightHeader(weights.header, &error_message) ||
        weights.values.size() != static_cast<std::size_t>(header_.weight_count)) {
        return false;
    }

    header_ = weights.header;
    if (weights_ == nullptr) {
        weights_ = std::make_unique<int[]>(header_.weight_count);
    }
    for (std::size_t i = 0; i < weights.values.size(); ++i) {
        weights_[i] = static_cast<int>(QuantizeWeight(weights.values[i]));
    }
    return true;
}

NTupleWeights NTupleEvaluator::ExportWeights() const {
    auto weights = NTupleWeights{};
    weights.header = header_;
    weights.values.resize(header_.weight_count);
    for (std::size_t i = 0; i < weights.values.size(); ++i) {
        weights.values[i] = static_cast<double>(weights_[i]);
    }
    return weights;
}

int NTupleEvaluator::Evaluate(const Position& position) const {
    int score = MaterialBalance(position);
    const auto& tuples = TupleDescriptors();
    const auto& offsets = TupleOffsets();
    for (std::size_t i = 0; i < tuples.size(); ++i) {
        score += weights_[offsets[i] + EncodeTuple(position, tuples[i])];
    }
    return position.SideToMove() == Color::Black ? score : -score;
}

std::uint64_t NTupleEvaluator::TupleSetHash() {
    std::uint64_t hash = 1469598103934665603ull;
    const auto& tuples = TupleDescriptors();
    for (const auto& tuple : tuples) {
        hash ^= tuple.length;
        hash *= 1099511628211ull;
        for (int i = 0; i < tuple.length; ++i) {
            hash ^= static_cast<std::uint64_t>(tuple.squares[i] + 1);
            hash *= 1099511628211ull;
        }
    }
    return hash;
}

void NTupleEvaluator::LoadBootstrapWeights() {
    const auto& tuples = TupleDescriptors();
    const auto& offsets = TupleOffsets();
    for (std::size_t i = 0; i < tuples.size(); ++i) {
        int span = 1;
        for (int j = 0; j < tuples[i].length; ++j) {
            span *= 3;
        }
        for (int code = 0; code < span; ++code) {
            int black = 0;
            int white = 0;
            int value = code;
            for (int k = 0; k < tuples[i].length; ++k) {
                const int state = value % 3;
                value /= 3;
                if (state == 1) {
                    ++black;
                } else if (state == 2) {
                    ++white;
                }
            }
            weights_[offsets[i] + code] = (black - white) * 6;
        }
    }
}

SearchWorker::SearchWorker(SearchController& controller, int worker_id)
    : controller_(controller),
      worker_id_(worker_id) {}

SearchResult SearchWorker::Run(Position root, const SearchLimits& limits) {
    (void)worker_id_;
    auto single_thread_limits = limits;
    single_thread_limits.threads = 1;
    return controller_.Search(root, single_thread_limits);
}

SearchController::SearchController()
    : table_(),
      evaluator_() {}

SearchController::~SearchController() = default;

SearchResult SearchController::Search(const Position& root, const SearchLimits& limits) {
    table_.NewSearch();
    auto shared = std::make_shared<SharedSearchState>(limits);
    {
        std::lock_guard<std::mutex> lock(state_mutex_);
        active_search_state_ = shared;
        last_snapshot_ = shared->Snapshot(true);
    }

    const auto finalize = [&](const SearchSnapshot& snapshot) {
        std::lock_guard<std::mutex> lock(state_mutex_);
        if (active_search_state_ == shared) {
            active_search_state_.reset();
        }
        last_snapshot_ = snapshot;
    };

    auto result = SearchResult{};
    int previous_score = 0;
    auto main_context = WorkerContext(*this, *shared, limits);
    auto worker_pool = std::unique_ptr<RootWorkerPool>{};
    if (limits.threads > 1) {
        worker_pool = std::make_unique<RootWorkerPool>(*this, *shared, limits);
    }

    try {
        for (int depth = 1; depth <= limits.max_depth && !shared->stop.load(std::memory_order_relaxed); ++depth) {
            int alpha = -kInfinity;
            int beta = kInfinity;
            auto window = std::max(1, limits.aspiration_window);
            auto full_window_mode = false;
            if (depth > 1) {
                if (IsMateLikeScore(previous_score)) {
                    full_window_mode = true;
                    shared->full_window_researches.fetch_add(1, std::memory_order_relaxed);
                } else {
                    alpha = previous_score - window;
                    beta = previous_score + window;
                }
            }

            auto depth_result = SearchResult{};
            int attempt = 0;
            while (true) {
                ++attempt;
                shared->diagnostic_attempt.store(attempt, std::memory_order_relaxed);
                const auto attempt_nodes_before = shared->nodes.load(std::memory_order_relaxed) + main_context.pending_nodes;
                const auto attempt_qnodes_before = shared->qnodes.load(std::memory_order_relaxed) + main_context.pending_qnodes;
                const auto attempt_root_updates_before = shared->root_best_updates.load(std::memory_order_relaxed);
                shared->aspiration_attempts.fetch_add(1, std::memory_order_relaxed);
                depth_result = SearchDepthAttempt(*this,
                                                  root,
                                                  *shared,
                                                  main_context,
                                                  worker_pool.get(),
                                                  depth,
                                                  alpha,
                                                  beta,
                                                  result.best_move);
                main_context.FlushCounters();
                const auto attempt_nodes_after = shared->nodes.load(std::memory_order_relaxed);
                const auto attempt_qnodes_after = shared->qnodes.load(std::memory_order_relaxed);
                const auto attempt_root_updates_after = shared->root_best_updates.load(std::memory_order_relaxed);
                if (shared->stop.load(std::memory_order_relaxed)) {
                    auto diagnostic = SearchAspirationDiagnostic{};
                    diagnostic.depth = depth;
                    diagnostic.attempt = attempt;
                    diagnostic.alpha = alpha;
                    diagnostic.beta = beta;
                    diagnostic.score = depth_result.score;
                    diagnostic.outcome = "stopped";
                    diagnostic.nodes = attempt_nodes_after - attempt_nodes_before;
                    diagnostic.qnodes = attempt_qnodes_after - attempt_qnodes_before;
                    diagnostic.root_best_updates = attempt_root_updates_after - attempt_root_updates_before;
                    shared->RecordAspirationDiagnostic(diagnostic);
                    break;
                }
                if (depth_result.score <= alpha) {
                    auto diagnostic = SearchAspirationDiagnostic{};
                    diagnostic.depth = depth;
                    diagnostic.attempt = attempt;
                    diagnostic.alpha = alpha;
                    diagnostic.beta = beta;
                    diagnostic.score = depth_result.score;
                    diagnostic.outcome = "fail_low";
                    diagnostic.nodes = attempt_nodes_after - attempt_nodes_before;
                    diagnostic.qnodes = attempt_qnodes_after - attempt_qnodes_before;
                    diagnostic.root_best_updates = attempt_root_updates_after - attempt_root_updates_before;
                    diagnostic.fail_lows = 1;
                    shared->RecordAspirationDiagnostic(diagnostic);
                    shared->fail_lows.fetch_add(1, std::memory_order_relaxed);
                    shared->aspiration_retries.fetch_add(1, std::memory_order_relaxed);
                    if (!full_window_mode && IsMateLikeScore(depth_result.score)) {
                        full_window_mode = true;
                        shared->full_window_researches.fetch_add(1, std::memory_order_relaxed);
                        alpha = -kInfinity;
                        beta = kInfinity;
                    } else if (!full_window_mode) {
                        window = std::max(window * 2, limits.aspiration_window * 2);
                        alpha = depth_result.score - window;
                        beta = depth_result.score + window;
                    }
                    continue;
                }
                if (depth_result.score >= beta) {
                    auto diagnostic = SearchAspirationDiagnostic{};
                    diagnostic.depth = depth;
                    diagnostic.attempt = attempt;
                    diagnostic.alpha = alpha;
                    diagnostic.beta = beta;
                    diagnostic.score = depth_result.score;
                    diagnostic.outcome = "fail_high";
                    diagnostic.nodes = attempt_nodes_after - attempt_nodes_before;
                    diagnostic.qnodes = attempt_qnodes_after - attempt_qnodes_before;
                    diagnostic.root_best_updates = attempt_root_updates_after - attempt_root_updates_before;
                    diagnostic.fail_highs = 1;
                    shared->RecordAspirationDiagnostic(diagnostic);
                    shared->fail_highs.fetch_add(1, std::memory_order_relaxed);
                    shared->aspiration_retries.fetch_add(1, std::memory_order_relaxed);
                    if (!full_window_mode && IsMateLikeScore(depth_result.score)) {
                        full_window_mode = true;
                        shared->full_window_researches.fetch_add(1, std::memory_order_relaxed);
                        alpha = -kInfinity;
                        beta = kInfinity;
                    } else if (!full_window_mode) {
                        window = std::max(window * 2, limits.aspiration_window * 2);
                        alpha = depth_result.score - window;
                        beta = depth_result.score + window;
                    }
                    continue;
                }
                auto diagnostic = SearchAspirationDiagnostic{};
                diagnostic.depth = depth;
                diagnostic.attempt = attempt;
                diagnostic.alpha = alpha;
                diagnostic.beta = beta;
                diagnostic.score = depth_result.score;
                diagnostic.outcome = "exact";
                diagnostic.nodes = attempt_nodes_after - attempt_nodes_before;
                diagnostic.qnodes = attempt_qnodes_after - attempt_qnodes_before;
                diagnostic.root_best_updates = attempt_root_updates_after - attempt_root_updates_before;
                shared->RecordAspirationDiagnostic(diagnostic);
                break;
            }

            if (depth_result.depth > 0) {
                result = depth_result;
                previous_score = depth_result.score;
                shared->PublishResult(result);
            }
        }

        result.stats = shared->Stats();
        result.diagnostics = shared->Diagnostics();
        auto snapshot = shared->Snapshot(false);
        snapshot.active = false;
        finalize(snapshot);
        return result;
    } catch (...) {
        auto snapshot = shared->Snapshot(false);
        snapshot.active = false;
        finalize(snapshot);
        throw;
    }
}

SearchResult SearchController::SearchReference(const Position& root, int depth) {
    auto result = SearchResult{};
    auto position = root;
    result.score = ReferenceAlphaBeta(evaluator_, position, depth, -kInfinity, kInfinity, result.stats);
    result.depth = depth;
    auto moves = MoveList{};
    GenerateMoves(root, moves);
    int best_score = -kInfinity;
    for (int i = 0; i < moves.size; ++i) {
        auto working = root;
        Undo undo;
        MakeMove(working, moves.moves[i], undo);
        SearchStats stats{};
        const int score = -ReferenceAlphaBeta(evaluator_, working, depth - 1, -kInfinity, kInfinity, stats);
        if (score > best_score) {
            best_score = score;
            result.best_move = moves.moves[i];
        }
        result.stats.nodes += stats.nodes;
        result.stats.qnodes += stats.qnodes;
    }
    result.score = best_score;
    return result;
}

SearchSnapshot SearchController::Snapshot() const {
    std::shared_ptr<SharedSearchState> shared;
    {
        std::lock_guard<std::mutex> lock(state_mutex_);
        shared = active_search_state_;
        if (!shared) {
            return last_snapshot_;
        }
    }
    return shared->Snapshot(true);
}

void SearchController::RequestStop() {
    std::shared_ptr<SharedSearchState> shared;
    {
        std::lock_guard<std::mutex> lock(state_mutex_);
        shared = active_search_state_;
    }
    if (shared) {
        shared->stop.store(true, std::memory_order_relaxed);
    }
}

BenchmarkResult RunBitboardBenchmark(SearchController& controller,
                                     const Position& position,
                                     const SearchLimits& limits,
                                     int movegen_iterations) {
    BenchmarkResult benchmark{};
    auto move_list = MoveList{};
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < movegen_iterations; ++i) {
        GenerateMoves(position, move_list);
        benchmark.movegen_nodes += static_cast<std::uint64_t>(move_list.size);
    }
    auto mid = std::chrono::steady_clock::now();

    benchmark.search_result = controller.Search(position, limits);
    auto end = std::chrono::steady_clock::now();

    benchmark.movegen_seconds = std::chrono::duration<double>(mid - start).count();
    benchmark.search_seconds = std::chrono::duration<double>(end - mid).count();
    benchmark.search_nodes = benchmark.search_result.stats.nodes;
    return benchmark;
}

namespace {

SearchLimits NormalizeSingleThreadLimits(SearchLimits limits) {
    if (limits.max_depth <= 0) {
        limits.max_depth = 4;
    }
    if (limits.aspiration_window <= 0) {
        limits.aspiration_window = 32;
    }
    limits.threads = 1;
    return limits;
}

void SetErrorMessage(std::string* error_message, const std::string& message) {
    if (error_message != nullptr) {
        *error_message = message;
    }
}

Position LoadPositionFromFileOrDefault(const std::string& file_name) {
    auto game = SurakartaGame{};
    if (file_name.empty()) {
        game.StartGame();
    } else {
        game.StartGame(file_name);
    }
    return PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());
}

NTupleWeights BootstrapWeights() {
    auto controller = SearchController{};
    return controller.Evaluator().ExportWeights();
}

Color WinnerColorFromPosition(const Position& position) {
    if (position.board.Count(Color::Black) == 0) {
        return Color::White;
    }
    if (position.board.Count(Color::White) == 0) {
        return Color::Black;
    }

    const int material = MaterialBalance(position);
    if (material > 0) {
        return Color::Black;
    }
    if (material < 0) {
        return Color::White;
    }
    return Color::None;
}

void AccumulateOutcome(Color winner, TrainingSummary& summary) {
    if (winner == Color::Black) {
        ++summary.black_wins;
    } else if (winner == Color::White) {
        ++summary.white_wins;
    } else {
        ++summary.draws;
    }
}

void AccumulateCheckmateTerminal(Color winner, TrainingSummary& summary) {
    AccumulateOutcome(winner, summary);
    ++summary.terminal_checkmate;
}

void AccumulateNoCaptureTerminal(Color winner, TrainingSummary& summary) {
    AccumulateOutcome(winner, summary);
    ++summary.terminal_no_capture_limit;
}

void AccumulateNoLegalMoveTerminal(Color winner, TrainingSummary& summary) {
    AccumulateOutcome(winner, summary);
    ++summary.terminal_no_legal_move;
}

void AccumulatePlyCapTerminal(Color winner, TrainingSummary& summary) {
    AccumulateOutcome(winner, summary);
    ++summary.terminal_ply_cap;
}

TrainingCheckpointSummary BuildTrainingCheckpointSummary(const TrainingSummary& summary,
                                                         const std::string& checkpoint_path,
                                                         double total_abs_td_error,
                                                         double total_abs_weight_delta) {
    auto checkpoint = TrainingCheckpointSummary{};
    checkpoint.games_completed = summary.games_completed;
    checkpoint.path = checkpoint_path;
    checkpoint.black_wins = summary.black_wins;
    checkpoint.white_wins = summary.white_wins;
    checkpoint.draws = summary.draws;
    checkpoint.terminal_checkmate = summary.terminal_checkmate;
    checkpoint.terminal_no_capture_limit = summary.terminal_no_capture_limit;
    checkpoint.terminal_no_legal_move = summary.terminal_no_legal_move;
    checkpoint.terminal_ply_cap = summary.terminal_ply_cap;
    checkpoint.positions_evaluated = summary.positions_evaluated;
    checkpoint.update_count = summary.update_count;
    checkpoint.max_abs_td_error = summary.max_abs_td_error;
    checkpoint.max_abs_weight_delta = summary.max_abs_weight_delta;
    if (summary.update_count > 0) {
        checkpoint.average_abs_td_error =
            total_abs_td_error / static_cast<double>(summary.update_count);
        checkpoint.average_abs_weight_delta =
            total_abs_weight_delta / static_cast<double>(summary.update_count);
    }
    return checkpoint;
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

std::filesystem::path ResolveEvaluationDataPath(const std::string& file_name) {
    const auto relative = std::filesystem::path("test") / "test_data" / file_name;
    auto candidates = std::vector<std::filesystem::path>{std::filesystem::current_path() / relative};

    auto parent = CurrentExecutablePath().parent_path();
    for (int i = 0; i < 8 && !parent.empty(); ++i) {
        candidates.push_back(parent / relative);
        parent = parent.parent_path();
    }

    for (const auto& candidate : candidates) {
        if (std::filesystem::exists(candidate)) {
            return std::filesystem::weakly_canonical(candidate);
        }
    }
    return {};
}

EvalSearchReport BuildEvalSearchReport(const NTupleWeights& weights,
                                       const Position& position,
                                       const SearchLimits& limits,
                                       const std::string& case_id) {
    auto controller = SearchController{};
    controller.Evaluator().ApplyWeights(weights);
    controller.Table().Clear();

    const auto start = std::chrono::steady_clock::now();
    const auto result = controller.Search(position, limits);
    const auto end = std::chrono::steady_clock::now();

    auto report = EvalSearchReport{};
    report.case_id = case_id;
    report.depth = result.depth;
    report.threads = limits.threads;
    report.search_seconds = std::chrono::duration<double>(end - start).count();
    report.nodes = result.stats.nodes;
    report.best_move = FormatMoveForPosition(position, result.best_move);
    report.pv = FormatPrincipalVariation(position, result.pv, result.pv_length);
    report.score = result.score;
    return report;
}

EvalMatchGame PlayEvaluationMatchGame(const std::string& case_id,
                                      const Position& start_position,
                                      const NTupleWeights& candidate_weights,
                                      const NTupleWeights& baseline_weights,
                                      const SearchLimits& limits,
                                      Color candidate_color) {
    auto report = EvalMatchGame{};
    report.case_id = case_id;
    report.candidate_color = candidate_color == Color::Black ? "black" : "white";

    auto candidate_controller = SearchController{};
    auto baseline_controller = SearchController{};
    candidate_controller.Evaluator().ApplyWeights(candidate_weights);
    baseline_controller.Evaluator().ApplyWeights(baseline_weights);

    auto position = start_position;
    for (;;) {
        const int terminal = EvaluateTerminal(position);
        if (terminal != std::numeric_limits<int>::min()) {
            report.final_reason = position.no_capture_ply >= position.max_no_capture_round ? "no_capture_limit" : "terminal";
            break;
        }

        auto legal_moves = MoveList{};
        GenerateMoves(position, legal_moves);
        if (legal_moves.size == 0) {
            report.final_reason = "no_legal_move";
            break;
        }

        const bool candidate_turn = position.SideToMove() == candidate_color;
        auto& controller = candidate_turn ? candidate_controller : baseline_controller;
        controller.Table().Clear();
        const auto result = controller.Search(position, limits);
        const auto chosen_move = result.best_move.IsValid() ? result.best_move : legal_moves.moves[0];

        auto undo = Undo{};
        MakeMove(position, chosen_move, undo);
        ++report.plies;

        if (report.plies >= 256) {
            report.final_reason = "ply_cap";
            break;
        }
    }

    const auto winner = WinnerColorFromPosition(position);
    if (winner == Color::None) {
        report.outcome = "draw";
    } else if (winner == candidate_color) {
        report.outcome = "candidate_win";
    } else {
        report.outcome = "baseline_win";
    }
    return report;
}

}  // namespace

bool RunBitboardTraining(const TrainingOptions& options,
                         TrainingSummary* summary,
                         std::string* error_message) {
    if (summary != nullptr) {
        *summary = TrainingSummary{};
    }
    if (options.output_weights_path.empty()) {
        SetErrorMessage(error_message, "output weight path is required");
        return false;
    }
    if (options.games < 0) {
        SetErrorMessage(error_message, "game count must be non-negative");
        return false;
    }
    if (options.terminal_reward <= 0.0) {
        SetErrorMessage(error_message, "terminal reward must be positive");
        return false;
    }
    if (options.td_error_clip < 0.0) {
        SetErrorMessage(error_message, "td error clip must be non-negative");
        return false;
    }
    if (options.terminal_only_warmup < 0) {
        SetErrorMessage(error_message, "terminal-only warmup must be non-negative");
        return false;
    }
    if (options.near_terminal_curriculum < 0) {
        SetErrorMessage(error_message, "near-terminal curriculum must be non-negative");
        return false;
    }

    try {
        auto weights = BootstrapWeights();
        if (!options.input_weights_path.empty() && !weights.LoadBinary(options.input_weights_path)) {
            SetErrorMessage(error_message, "failed to load input weights: " + options.input_weights_path);
            return false;
        }

        const auto limits = NormalizeSingleThreadLimits(options.limits);
        auto local_summary = TrainingSummary{};
        local_summary.games_requested = options.games;
        local_summary.seed = options.seed;
        local_summary.depth = limits.max_depth;
        local_summary.alpha = options.alpha;
        local_summary.lambda = options.lambda;
        local_summary.epsilon = options.epsilon;
        local_summary.epsilon_plies = options.epsilon_plies;
        local_summary.terminal_reward = options.terminal_reward;
        local_summary.td_error_clip = options.td_error_clip;
        local_summary.terminal_only_warmup = options.terminal_only_warmup;
        local_summary.near_terminal_curriculum = options.near_terminal_curriculum;
        local_summary.output_weights_path = options.output_weights_path;

        if (options.games == 0) {
            if (!weights.SaveBinary(options.output_weights_path)) {
                SetErrorMessage(error_message, "failed to save exported weights");
                return false;
            }
            if (summary != nullptr) {
                *summary = local_summary;
            }
            return true;
        }

        auto controller = SearchController{};
        auto traces = std::vector<double>(weights.values.size(), 0.0);
        auto rng = std::mt19937(options.seed);
        auto random_unit = std::uniform_real_distribution<double>(0.0, 1.0);
        auto total_abs_td_error = 0.0;
        auto total_abs_weight_delta = 0.0;

        const auto curriculum_games = std::max(options.terminal_only_warmup, options.near_terminal_curriculum);
        for (int game_index = 0; game_index < options.games; ++game_index) {
            auto position = game_index < curriculum_games
                                ? BuildNearTerminalTrainingPosition(game_index)
                                : LoadPositionFromFileOrDefault("");
            ResetTrainingTraces(traces);
            int plies = 0;

            for (;;) {
                if (EvaluateTerminal(position) != std::numeric_limits<int>::min()) {
                    const auto winner = WinnerColorFromPosition(position);
                    if (position.no_capture_ply >= position.max_no_capture_round) {
                        AccumulateNoCaptureTerminal(winner, local_summary);
                    } else {
                        AccumulateCheckmateTerminal(winner, local_summary);
                    }
                    break;
                }

                auto legal_moves = MoveList{};
                GenerateMoves(position, legal_moves);
                if (legal_moves.size == 0) {
                    AccumulateNoLegalMoveTerminal(WinnerColorFromPosition(position), local_summary);
                    break;
                }

                auto chosen_move = Move{};
                if (plies < options.epsilon_plies && random_unit(rng) < options.epsilon) {
                    auto move_index = std::uniform_int_distribution<int>(0, legal_moves.size - 1)(rng);
                    chosen_move = legal_moves.moves[move_index];
                } else {
                    controller.Evaluator().ApplyWeights(weights);
                    controller.Table().Clear();
                    const auto result = controller.Search(position, limits);
                    chosen_move = result.best_move.IsValid() ? result.best_move : legal_moves.moves[0];
                }

                auto next = position;
                auto undo = Undo{};
                MakeMove(next, chosen_move, undo);

                auto context = TrainingStepContext{};
                context.next_is_ply_cap = (plies + 1) >= kTrainingMaxPlies;
                const auto next_terminal = EvaluateTerminal(next) != std::numeric_limits<int>::min();
                auto next_legal_moves = MoveList{};
                if (!next_terminal && !context.next_is_ply_cap) {
                    GenerateMoves(next, next_legal_moves);
                    context.next_has_legal_moves = next_legal_moves.size > 0;
                }

                auto step_result = TrainingStepResult{};
                if (!ApplyTrainingStep(weights,
                                       traces,
                                       position,
                                       next,
                                       context,
                                       options.alpha,
                                       options.lambda,
                                       &step_result,
                                       options.terminal_reward,
                                       options.td_error_clip)) {
                    SetErrorMessage(error_message, "failed to apply training td step");
                    return false;
                }
                ++local_summary.positions_evaluated;
                ++local_summary.update_count;
                total_abs_td_error += std::abs(step_result.td_error);
                local_summary.max_abs_td_error = std::max(local_summary.max_abs_td_error, std::abs(step_result.td_error));
                total_abs_weight_delta += step_result.abs_weight_delta;
                local_summary.max_abs_weight_delta =
                    std::max(local_summary.max_abs_weight_delta, step_result.abs_weight_delta);

                position = next;
                ++plies;
                if (next_terminal) {
                    const auto winner = WinnerColorFromPosition(position);
                    if (position.no_capture_ply >= position.max_no_capture_round) {
                        AccumulateNoCaptureTerminal(winner, local_summary);
                    } else {
                        AccumulateCheckmateTerminal(winner, local_summary);
                    }
                    break;
                }
                if (!context.next_has_legal_moves) {
                    AccumulateNoLegalMoveTerminal(WinnerColorFromPosition(position), local_summary);
                    break;
                }
                if (context.next_is_ply_cap) {
                    AccumulatePlyCapTerminal(WinnerColorFromPosition(position), local_summary);
                    break;
                }
            }

            ++local_summary.games_completed;
            if (options.checkpoint_every > 0 &&
                !options.checkpoint_dir.empty() &&
                ((game_index + 1) % options.checkpoint_every) == 0) {
                std::filesystem::create_directories(options.checkpoint_dir);
                const auto checkpoint_path =
                    std::filesystem::path(options.checkpoint_dir) /
                    ("checkpoint-" + std::to_string(game_index + 1) + ".bin");
                if (!weights.SaveBinary(checkpoint_path.string())) {
                    SetErrorMessage(error_message, "failed to save checkpoint weights");
                    return false;
                }
                ++local_summary.checkpoint_count;
                local_summary.checkpoint_summaries.push_back(
                    BuildTrainingCheckpointSummary(local_summary,
                                                   checkpoint_path.string(),
                                                   total_abs_td_error,
                                                   total_abs_weight_delta));
            }
        }

        if (local_summary.update_count > 0) {
            local_summary.average_abs_td_error =
                total_abs_td_error / static_cast<double>(local_summary.update_count);
            local_summary.average_abs_weight_delta =
                total_abs_weight_delta / static_cast<double>(local_summary.update_count);
        }

        if (!weights.SaveBinary(options.output_weights_path)) {
            SetErrorMessage(error_message, "failed to save output weights");
            return false;
        }

        if (summary != nullptr) {
            *summary = local_summary;
        }
        return true;
    } catch (const std::exception& exception) {
        SetErrorMessage(error_message, exception.what());
        return false;
    }
}

bool RunBitboardEvaluation(const EvalOptions& options,
                           EvalReport* report,
                           std::string* error_message) {
    if (report != nullptr) {
        *report = EvalReport{};
    }
    if (options.candidate_weights_path.empty()) {
        SetErrorMessage(error_message, "candidate weight path is required");
        return false;
    }

    try {
        auto candidate_weights = NTupleWeights{};
        if (!candidate_weights.LoadBinary(options.candidate_weights_path)) {
            SetErrorMessage(error_message, "failed to load candidate weights: " + options.candidate_weights_path);
            return false;
        }

        auto baseline_weights = BootstrapWeights();
        auto baseline_label = std::string{"bootstrap"};
        if (!options.baseline_weights_path.empty()) {
            if (!baseline_weights.LoadBinary(options.baseline_weights_path)) {
                SetErrorMessage(error_message, "failed to load baseline weights: " + options.baseline_weights_path);
                return false;
            }
            baseline_label = options.baseline_weights_path;
        }

        const auto limits = NormalizeSingleThreadLimits(options.limits);
        const auto capture_heavy_path = ResolveEvaluationDataPath("game1.txt");
        const auto threat_heavy_path = ResolveEvaluationDataPath("game10.txt");
        const auto endgame_path = ResolveEvaluationDataPath("game6.txt");
        const auto no_capture_critical_path = ResolveEvaluationDataPath("game8.txt");
        if (capture_heavy_path.empty() ||
            threat_heavy_path.empty() ||
            endgame_path.empty() ||
            no_capture_critical_path.empty()) {
            SetErrorMessage(error_message, "failed to resolve benchmark test positions");
            return false;
        }

        auto local_report = EvalReport{};
        local_report.candidate_weights = options.candidate_weights_path;
        local_report.baseline_weights = baseline_label;
        local_report.depth = limits.max_depth;

        const auto cases = std::array<std::pair<std::string, std::string>, 5>{
            std::pair<std::string, std::string>{"opening", ""},
            {"capture-heavy", capture_heavy_path.string()},
            {"threat-heavy", threat_heavy_path.string()},
            {"endgame", endgame_path.string()},
            {"no-capture-critical", no_capture_critical_path.string()},
        };

        for (const auto& [case_id, file_name] : cases) {
            const auto position = LoadPositionFromFileOrDefault(file_name);

            auto position_report = EvalPositionReport{};
            position_report.case_id = case_id;
            position_report.candidate = BuildEvalSearchReport(candidate_weights, position, limits, case_id);
            position_report.baseline = BuildEvalSearchReport(baseline_weights, position, limits, case_id);
            local_report.position_reports.push_back(position_report);

            for (const auto candidate_color : {Color::Black, Color::White}) {
                auto game_report =
                    PlayEvaluationMatchGame(case_id, position, candidate_weights, baseline_weights, limits, candidate_color);
                if (game_report.outcome == "candidate_win") {
                    ++local_report.match_summary.candidate_wins;
                } else if (game_report.outcome == "baseline_win") {
                    ++local_report.match_summary.baseline_wins;
                } else {
                    ++local_report.match_summary.draws;
                }
                local_report.match_games.push_back(game_report);
            }
        }

        if (report != nullptr) {
            *report = local_report;
        }
        return true;
    } catch (const std::exception& exception) {
        SetErrorMessage(error_message, exception.what());
        return false;
    }
}

}  // namespace surakarta::bitboard
