#include "surakarta_dev_session.h"

#include <algorithm>
#include <fstream>

#include "surakarta_game.h"
#include "surakarta_rule_manager_bitboard.h"

namespace surakarta::devtools {

namespace {

bool TryLoadPositionFromFile(const std::string& file_name,
                             bitboard::Position* position,
                             std::string* error) {
    auto game = SurakartaGame{};
    if (file_name.empty()) {
        game.StartGame();
    } else {
        auto input = std::ifstream(file_name);
        if (!input.is_open()) {
            if (error != nullptr) {
                *error = "Failed to open board file: " + file_name;
            }
            return false;
        }
        game.StartGame(file_name);
    }

    *position = bitboard::PositionBuilder::FromLegacy(*game.GetBoard(), *game.GetGameInfo());
    return true;
}

}  // namespace

DevelopmentSession::DevelopmentSession() {
    limits_.max_depth = 4;
    limits_.threads = 1;
    limits_.aspiration_window = 32;
    Reset();
}

DevelopmentSession::~DevelopmentSession() {
    CancelSearch();
}

void DevelopmentSession::Reset() {
    std::string ignored_error;
    CancelSearch();
    LoadPosition("", &ignored_error);
}

bool DevelopmentSession::LoadFromFile(const std::string& file_name, std::string* error) {
    CancelSearch();
    return LoadPosition(file_name, error);
}

void DevelopmentSession::SetHumanColor(bitboard::Color color) {
    if (human_color_ == color) {
        return;
    }
    CancelSearch();
    human_color_ = color;
    RebuildSelection();
    RebuildHoverPath();
    StartEngineSearchIfNeeded();
}

void DevelopmentSession::SetSearchLimits(const bitboard::SearchLimits& limits) {
    auto sanitized = limits;
    sanitized.max_depth = std::max(1, sanitized.max_depth);
    sanitized.threads = std::max(1, sanitized.threads);
    sanitized.aspiration_window = std::max(1, sanitized.aspiration_window);
    CancelSearch();
    limits_ = sanitized;
    StartEngineSearchIfNeeded();
}

bool DevelopmentSession::SelectSquare(bitboard::Square square) {
    Update();
    if (SearchActive() || status_.terminal || position_.SideToMove() != human_color_) {
        return false;
    }
    if (!position_.board.HasPiece(human_color_, square)) {
        return false;
    }

    selected_square_ = square;
    legal_targets_ = bitboard::DescribeLegalTargets(position_, human_color_, square);
    if (legal_targets_.empty()) {
        ClearSelection();
        return false;
    }

    RebuildHoverPath();
    return true;
}

bool DevelopmentSession::ApplyHumanMove(bitboard::Square destination) {
    Update();
    if (SearchActive() || status_.terminal || position_.SideToMove() != human_color_) {
        return false;
    }

    const auto target = FindTarget(destination);
    if (!target.has_value()) {
        return false;
    }

    auto move = bitboard::Move{};
    move.from = target->from;
    move.to = target->to;
    move.flags = target->flags;
    move.aux = target->primary_variant;
    ApplyMove(move);
    return true;
}

void DevelopmentSession::ClearSelection() {
    selected_square_.reset();
    hovered_target_.reset();
    legal_targets_.clear();
    hover_path_.clear();
}

void DevelopmentSession::SetHoveredTarget(bitboard::Square square) {
    hovered_target_ = square;
    RebuildHoverPath();
}

void DevelopmentSession::ClearHoveredTarget() {
    hovered_target_.reset();
    hover_path_.clear();
}

void DevelopmentSession::Update() {
    RefreshSearchSnapshot();
    ApplyCompletedSearchIfReady();
}

bool DevelopmentSession::WaitForIdle(std::chrono::milliseconds timeout) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (true) {
        Update();
        if (!SearchActive() && !search_thread_.joinable()) {
            return true;
        }
        if (std::chrono::steady_clock::now() >= deadline) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    Update();
    return !SearchActive() && !search_thread_.joinable();
}

bitboard::SearchSnapshot DevelopmentSession::SearchSnapshot() const {
    std::lock_guard<std::mutex> lock(search_mutex_);
    if (ui_snapshot_generation_ != active_generation_.load(std::memory_order_acquire)) {
        return {};
    }
    return ui_snapshot_;
}

bool DevelopmentSession::SearchActive() const {
    return search_running_.load(std::memory_order_acquire);
}

bool DevelopmentSession::IsEngineTurn() const {
    return !status_.terminal && position_.SideToMove() != human_color_;
}

int DevelopmentSession::EvalScore() const {
    return controller_.Evaluator().Evaluate(position_);
}

bool DevelopmentSession::LoadPosition(const std::string& file_name, std::string* error) {
    auto loaded = bitboard::Position{};
    if (!TryLoadPositionFromFile(file_name, &loaded, error)) {
        return false;
    }

    position_ = loaded;
    loaded_file_ = file_name;
    last_move_.reset();
    ClearSelection();
    ResetSearchArtifacts(active_generation_.load(std::memory_order_acquire));
    RefreshDerivedState();
    StartEngineSearchIfNeeded();
    return true;
}

std::uint64_t DevelopmentSession::AdvanceGeneration() {
    const auto generation = ++generation_counter_;
    active_generation_.store(generation, std::memory_order_release);
    return generation;
}

void DevelopmentSession::StartEngineSearchIfNeeded() {
    if (search_thread_.joinable()) {
        search_thread_.join();
    }
    const auto generation = active_generation_.load(std::memory_order_acquire);
    if (!IsEngineTurn()) {
        {
            std::lock_guard<std::mutex> lock(search_mutex_);
            if (ui_snapshot_generation_ == generation &&
                (ui_snapshot_.depth > 0 || ui_snapshot_.nodes > 0 || ui_snapshot_.best_move.IsValid())) {
                ui_snapshot_.active = false;
                return;
            }
        }
        ResetSearchArtifacts(generation);
        return;
    }

    ResetSearchArtifacts(generation, true);
    const auto root = position_;
    const auto limits = limits_;
    search_running_.store(true, std::memory_order_release);
    search_thread_ = std::thread([this, generation, root, limits]() mutable {
        const auto result = controller_.Search(root, limits);
        const auto final_snapshot = controller_.Snapshot();
        {
            std::lock_guard<std::mutex> lock(search_mutex_);
            if (generation == active_generation_.load(std::memory_order_acquire)) {
                completed_search_ = CompletedSearch{generation, result};
                ui_snapshot_ = final_snapshot;
                ui_snapshot_generation_ = generation;
            }
        }
        search_running_.store(false, std::memory_order_release);
    });
}

void DevelopmentSession::CancelSearch() {
    const auto generation = AdvanceGeneration();
    controller_.RequestStop();
    if (search_thread_.joinable()) {
        search_thread_.join();
    }
    search_running_.store(false, std::memory_order_release);
    ResetSearchArtifacts(generation);
}

void DevelopmentSession::RebuildSelection() {
    if (!selected_square_.has_value() || position_.SideToMove() != human_color_) {
        ClearSelection();
        return;
    }

    legal_targets_ = bitboard::DescribeLegalTargets(position_, human_color_, *selected_square_);
    if (legal_targets_.empty()) {
        ClearSelection();
    }
}

void DevelopmentSession::RebuildHoverPath() {
    hover_path_.clear();
    if (!selected_square_.has_value() || !hovered_target_.has_value()) {
        return;
    }

    const auto target = FindTarget(*hovered_target_);
    if (!target.has_value() || !target->IsCapture()) {
        return;
    }

    hover_path_ = bitboard::ReconstructCapturePath(position_,
                                                  *selected_square_,
                                                  *hovered_target_,
                                                  target->primary_variant);
}

void DevelopmentSession::RefreshDerivedState() {
    status_ = {};
    const auto [end_reason, winner] = EvaluateBitboardTerminal(position_);
    if (end_reason != SurakartaEndReason::NONE) {
        status_.terminal = true;
        status_.end_reason = end_reason;
        status_.winner = winner;
    }
}

void DevelopmentSession::ApplyMove(bitboard::Move move, bool preserve_completed_snapshot) {
    const auto generation = AdvanceGeneration();
    auto undo = bitboard::Undo{};
    bitboard::MakeMove(position_, move, undo);
    last_move_ = move;
    ClearSelection();
    if (preserve_completed_snapshot) {
        std::lock_guard<std::mutex> lock(search_mutex_);
        completed_search_.reset();
        ui_snapshot_.active = false;
        ui_snapshot_generation_ = generation;
    } else {
        ResetSearchArtifacts(generation);
    }
    RefreshDerivedState();
    StartEngineSearchIfNeeded();
}

void DevelopmentSession::ApplyCompletedSearchIfReady() {
    if (search_running_.load(std::memory_order_acquire) || !search_thread_.joinable()) {
        return;
    }

    search_thread_.join();

    auto completed = std::optional<CompletedSearch>{};
    {
        std::lock_guard<std::mutex> lock(search_mutex_);
        completed = completed_search_;
        completed_search_.reset();
    }

    if (!completed.has_value()) {
        return;
    }
    if (completed->generation != active_generation_.load(std::memory_order_acquire)) {
        return;
    }
    if (!completed->result.best_move.IsValid()) {
        return;
    }

    ApplyMove(completed->result.best_move, true);
}

std::optional<bitboard::LegalTargetInfo> DevelopmentSession::FindTarget(bitboard::Square destination) const {
    for (const auto& target : legal_targets_) {
        if (target.to == destination) {
            return target;
        }
    }
    return std::nullopt;
}

void DevelopmentSession::ResetSearchArtifacts(std::uint64_t generation, bool snapshot_active) {
    std::lock_guard<std::mutex> lock(search_mutex_);
    completed_search_.reset();
    ui_snapshot_ = {};
    ui_snapshot_.active = snapshot_active;
    ui_snapshot_generation_ = generation;
}

void DevelopmentSession::PublishSearchSnapshot(std::uint64_t generation, const bitboard::SearchSnapshot& snapshot) {
    std::lock_guard<std::mutex> lock(search_mutex_);
    if (generation != active_generation_.load(std::memory_order_acquire)) {
        return;
    }
    ui_snapshot_ = snapshot;
    ui_snapshot_generation_ = generation;
}

void DevelopmentSession::RefreshSearchSnapshot() {
    if (!SearchActive()) {
        return;
    }

    const auto snapshot = controller_.Snapshot();
    if (!snapshot.active) {
        return;
    }

    PublishSearchSnapshot(active_generation_.load(std::memory_order_acquire), snapshot);
}

}  // namespace surakarta::devtools
