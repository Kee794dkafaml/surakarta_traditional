#pragma once

#include <array>
#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "surakarta_bitboard.h"

namespace surakarta::bitboard {

struct SearchLimits {
    int max_depth{4};
    int threads{1};
    int aspiration_window{32};
    std::uint64_t node_limit{0};
    bool enable_null_move{true};
    bool enable_lmr{true};
};

struct SearchStats {
    std::uint64_t nodes{0};
    std::uint64_t qnodes{0};
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
};

struct SearchResult {
    Move best_move{};
    int score{0};
    int depth{0};
    SearchStats stats{};
    std::array<Move, kMaxPly> pv{};
    int pv_length{0};
};

struct SearchSnapshot {
    bool active{false};
    bool stop_requested{false};
    Move best_move{};
    std::array<Move, kMaxPly> pv{};
    int pv_length{0};
    int score{0};
    int depth{0};
    std::uint64_t nodes{0};
    std::uint64_t qnodes{0};
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
    std::uint64_t nps{0};
};

struct SearchStack {
    std::array<Move, kMaxPly> pv{};
    int pv_length{0};
    Move current_move{};
};

enum class TTBound : std::uint8_t {
    None = 0,
    Exact = 1,
    Lower = 2,
    Upper = 3,
};

struct TTEntry {
    std::uint64_t key{0};
    int score{0};
    std::uint8_t depth{0};
    std::uint8_t generation{0};
    TTBound bound{TTBound::None};
    Move best_move{};
};

struct TTProbe {
    bool found{false};
    TTEntry entry{};
};

class TranspositionTable {
   public:
    explicit TranspositionTable(std::size_t cluster_count = 1u << 14);

    void Clear();
    void NewSearch();
    TTProbe Probe(std::uint64_t key) const;
    void Store(std::uint64_t key, int depth, int score, TTBound bound, Move best_move);

   private:
    struct ClusterEntry {
        std::atomic<std::uint64_t> key{0};
        std::atomic<std::uint64_t> meta{0};
        std::atomic<std::uint32_t> move{0};
    };

    struct Cluster {
        std::array<ClusterEntry, 4> entries{};
    };

    std::size_t cluster_count_;
    std::unique_ptr<Cluster[]> clusters_;
    std::atomic<std::uint8_t> generation_{1};
};

struct NTupleWeightHeader {
    std::uint32_t magic{0x5455504C};
    std::uint16_t version{1};
    std::uint16_t reserved{0};
    std::uint64_t tuple_set_hash{0};
    std::uint32_t weight_count{0};
};

struct NTupleWeights {
    NTupleWeightHeader header{};
    std::vector<double> values{};

    NTupleWeights();

    bool LoadBinary(const std::string& file_name);
    bool SaveBinary(const std::string& file_name) const;
    int Evaluate(const Position& position) const;
    void EnumerateActiveWeightIndices(const Position& position, std::vector<int>& out) const;
};

class NTupleEvaluator {
   public:
    NTupleEvaluator();

    bool LoadWeights(const std::string& file_name);
    bool SaveWeights(const std::string& file_name) const;
    bool ApplyWeights(const NTupleWeights& weights);
    NTupleWeights ExportWeights() const;
    int Evaluate(const Position& position) const;
    const NTupleWeightHeader& Header() const { return header_; }
    static std::uint64_t TupleSetHash();

   private:
    void LoadBootstrapWeights();

    NTupleWeightHeader header_{};
    std::unique_ptr<int[]> weights_;
};

class SearchController;

class SearchWorker {
   public:
    SearchWorker(SearchController& controller, int worker_id);
    SearchResult Run(Position root, const SearchLimits& limits);

   private:
    SearchController& controller_;
    int worker_id_;
};

class SearchController {
   public:
    struct SharedSearchState;

    SearchController();
    ~SearchController();

    SearchResult Search(const Position& root, const SearchLimits& limits);
    SearchResult SearchReference(const Position& root, int depth);
    SearchSnapshot Snapshot() const;
    void RequestStop();

    TranspositionTable& Table() { return table_; }
    const TranspositionTable& Table() const { return table_; }
    const NTupleEvaluator& Evaluator() const { return evaluator_; }
    NTupleEvaluator& Evaluator() { return evaluator_; }

   private:
    friend class SearchWorker;

    TranspositionTable table_;
    NTupleEvaluator evaluator_;
    mutable std::mutex state_mutex_;
    std::shared_ptr<SharedSearchState> active_search_state_;
    SearchSnapshot last_snapshot_{};
};

struct BenchmarkResult {
    std::uint64_t movegen_nodes{0};
    std::uint64_t search_nodes{0};
    double movegen_seconds{0.0};
    double search_seconds{0.0};
    SearchResult search_result{};
};

struct TrainingOptions {
    SearchLimits limits{};
    std::string input_weights_path;
    std::string output_weights_path;
    int games{0};
    double alpha{0.01};
    double lambda{0.7};
    double epsilon{0.10};
    int epsilon_plies{6};
    std::uint32_t seed{20260423};
    int checkpoint_every{0};
    std::string checkpoint_dir;
};

struct TrainingSummary {
    int games_requested{0};
    int games_completed{0};
    int black_wins{0};
    int white_wins{0};
    int draws{0};
    std::uint64_t positions_evaluated{0};
    std::uint64_t update_count{0};
    double average_abs_td_error{0.0};
    double max_abs_td_error{0.0};
    double average_abs_weight_delta{0.0};
    double max_abs_weight_delta{0.0};
    int checkpoint_count{0};
    std::string output_weights_path;
};

struct EvalSearchReport {
    std::string case_id;
    int depth{0};
    int threads{0};
    double search_seconds{0.0};
    std::uint64_t nodes{0};
    std::string best_move;
    std::string pv;
    int score{0};
};

struct EvalPositionReport {
    std::string case_id;
    EvalSearchReport candidate{};
    EvalSearchReport baseline{};
};

struct EvalMatchSummary {
    int candidate_wins{0};
    int baseline_wins{0};
    int draws{0};
};

struct EvalMatchGame {
    std::string case_id;
    std::string candidate_color;
    int plies{0};
    std::string outcome;
    std::string final_reason;
};

struct EvalOptions {
    SearchLimits limits{};
    std::string candidate_weights_path;
    std::string baseline_weights_path;
};

struct EvalReport {
    std::string candidate_weights;
    std::string baseline_weights;
    int depth{0};
    std::vector<EvalPositionReport> position_reports{};
    EvalMatchSummary match_summary{};
    std::vector<EvalMatchGame> match_games{};
};

BenchmarkResult RunBitboardBenchmark(SearchController& controller,
                                     const Position& position,
                                     const SearchLimits& limits,
                                     int movegen_iterations);
bool RunBitboardTraining(const TrainingOptions& options,
                         TrainingSummary* summary,
                         std::string* error_message = nullptr);
bool RunBitboardEvaluation(const EvalOptions& options,
                           EvalReport* report,
                           std::string* error_message = nullptr);

}  // namespace surakarta::bitboard
