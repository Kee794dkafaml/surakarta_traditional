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
    bool enable_diagnostics{false};
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

struct SearchDiagnosticLine {
    std::array<Move, kMaxPly> moves{};
    int length{0};
};

struct SearchRootMoveDiagnostic {
    int depth{0};
    int attempt{0};
    int move_index{0};
    Move move{};
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
    SearchDiagnosticLine pv{};
};

struct SearchAspirationDiagnostic {
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

struct SearchQsearchDiagnostic {
    int depth{0};
    int attempt{0};
    int root_move_index{0};
    Move root_move{};
    int ply{0};
    int score{0};
    std::uint64_t qnodes{0};
    std::uint64_t capture_moves{0};
    SearchDiagnosticLine line{};
};

struct SearchDiagnostics {
    std::vector<SearchRootMoveDiagnostic> root_moves{};
    std::vector<SearchAspirationDiagnostic> aspiration_attempts{};
    std::vector<SearchQsearchDiagnostic> qsearch_sources{};
};

struct SearchResult {
    Move best_move{};
    int score{0};
    int depth{0};
    SearchStats stats{};
    std::array<Move, kMaxPly> pv{};
    int pv_length{0};
    SearchDiagnostics diagnostics{};
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

struct ActiveObjectiveConfigSkeleton {
    bool config_present = false;
    bool config_valid = false;
    bool default_off = true;
    bool skeleton_enabled = false;
    bool scoped_config = false;
    bool probe_wiring_skeleton = false;
    bool no_output_probe_mode = false;
    bool weight_artifact_suppressed = false;
    bool active_objective_probe_executed = false;
    bool selection_gate_eligible = false;
    std::string mode;
    std::string scope;
    std::string reject_reason;
    double opening_drift_penalty_weight{0.0};
    int max_games{0};
    int max_depth{0};
    std::uint32_t seed{0};
};

struct TrainingOptions {
    SearchLimits limits{};
    std::string input_weights_path;
    std::string output_weights_path;
    int games{0};
    double alpha{0.005};
    double lambda{0.5};
    double epsilon{0.02};
    int epsilon_plies{6};
    double terminal_reward{1200.0};
    double td_error_clip{0.0};
    int terminal_only_warmup{0};
    int near_terminal_curriculum{0};
    std::uint32_t seed{20260423};
    int checkpoint_every{0};
    std::string checkpoint_dir;
    std::string active_objective_config_path;
    ActiveObjectiveConfigSkeleton active_objective_config{};
};

struct TrainingStepContext {
    bool next_has_legal_moves{true};
    bool next_is_ply_cap{false};
};

struct TrainingStepResult {
    double current_value{0.0};
    double target_value{0.0};
    double td_error{0.0};
    double abs_weight_delta{0.0};
    std::uint64_t changed_weight_count{0};
    bool terminal_target{false};
};

struct TrainingCheckpointSummary {
    int games_completed{0};
    std::string path;
    int black_wins{0};
    int white_wins{0};
    int draws{0};
    int terminal_checkmate{0};
    int terminal_no_capture_limit{0};
    int terminal_no_legal_move{0};
    int terminal_ply_cap{0};
    std::uint64_t positions_evaluated{0};
    std::uint64_t update_count{0};
    double average_abs_td_error{0.0};
    double max_abs_td_error{0.0};
    double average_abs_weight_delta{0.0};
    double max_abs_weight_delta{0.0};
};

struct TrainingSummary {
    int games_requested{0};
    int games_completed{0};
    std::uint32_t seed{0};
    int depth{0};
    double alpha{0.0};
    double lambda{0.0};
    double epsilon{0.0};
    int epsilon_plies{0};
    double terminal_reward{0.0};
    double td_error_clip{0.0};
    int terminal_only_warmup{0};
    int near_terminal_curriculum{0};
    int black_wins{0};
    int white_wins{0};
    int draws{0};
    int terminal_checkmate{0};
    int terminal_no_capture_limit{0};
    int terminal_no_legal_move{0};
    int terminal_ply_cap{0};
    std::uint64_t positions_evaluated{0};
    std::uint64_t update_count{0};
    double average_abs_td_error{0.0};
    double max_abs_td_error{0.0};
    double average_abs_weight_delta{0.0};
    double max_abs_weight_delta{0.0};
    int checkpoint_count{0};
    std::vector<TrainingCheckpointSummary> checkpoint_summaries{};
    std::string output_weights_path;
    bool active_interface_config_present = false;
    bool active_interface_config_valid = false;
    bool active_interface_skeleton_enabled = false;
    bool active_interface_scoped_config = false;
    bool active_interface_probe_wiring_skeleton = false;
    bool active_interface_no_output_probe_mode = false;
    bool active_interface_weight_artifact_suppressed = false;
    bool active_interface_report_only_probe_path = false;
    bool active_objective_probe_executed = false;
    bool selection_gate_eligible = false;
    std::string active_interface_config_status{"default_off"};
    std::string active_interface_reject_reason;
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
double EvaluateTrainingValueForSideToMove(const NTupleWeights& weights, const Position& position);
double TrainingTerminalTarget(Color winner, Color side_to_update, double terminal_reward = 1200.0);
Position BuildNearTerminalTrainingPosition(int sample_index);
void ResetTrainingTraces(std::vector<double>& traces);
bool ParseActiveObjectiveConfigSkeleton(const std::string& text,
                                        ActiveObjectiveConfigSkeleton* config,
                                        std::string* error_message = nullptr);
bool ApplyTrainingStep(NTupleWeights& weights,
                       std::vector<double>& traces,
                       const Position& current,
                       const Position& next,
                       const TrainingStepContext& context,
                       double alpha,
                       double lambda,
                       TrainingStepResult* result = nullptr,
                       double terminal_reward = 1200.0,
                       double td_error_clip = 0.0);
bool RunBitboardTraining(const TrainingOptions& options,
                         TrainingSummary* summary,
                         std::string* error_message = nullptr);
bool RunBitboardEvaluation(const EvalOptions& options,
                           EvalReport* report,
                           std::string* error_message = nullptr);

}  // namespace surakarta::bitboard
