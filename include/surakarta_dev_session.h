#pragma once

#include <chrono>
#include <atomic>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "surakarta_bitboard.h"
#include "surakarta_reason.h"
#include "surakarta_search.h"

namespace surakarta::devtools {

struct SessionStatus {
    bool terminal{false};
    SurakartaEndReason end_reason{SurakartaEndReason::NONE};
    SurakartaPlayer winner{SurakartaPlayer::NONE};
};

class DevelopmentSession {
   public:
    DevelopmentSession();
    ~DevelopmentSession();

    void Reset();
    bool LoadFromFile(const std::string& file_name, std::string* error = nullptr);

    void SetHumanColor(bitboard::Color color);
    void SetSearchLimits(const bitboard::SearchLimits& limits);

    bool SelectSquare(bitboard::Square square);
    bool ApplyHumanMove(bitboard::Square destination);
    void ClearSelection();

    void SetHoveredTarget(bitboard::Square square);
    void ClearHoveredTarget();

    void Update();
    bool WaitForIdle(std::chrono::milliseconds timeout);

    const bitboard::Position& Position() const { return position_; }
    bitboard::Color HumanColor() const { return human_color_; }
    std::optional<bitboard::Square> SelectedSquare() const { return selected_square_; }
    const std::vector<bitboard::LegalTargetInfo>& LegalTargets() const { return legal_targets_; }
    const std::vector<SurakartaMovePathFragment>& HoverPath() const { return hover_path_; }
    bitboard::SearchSnapshot SearchSnapshot() const;
    bool SearchActive() const;
    bool IsEngineTurn() const;
    int EvalScore() const;
    SessionStatus Status() const { return status_; }
    std::optional<bitboard::Move> LastMove() const { return last_move_; }
    const std::string& LoadedFile() const { return loaded_file_; }

   private:
    struct CompletedSearch {
        std::uint64_t generation{0};
        bitboard::SearchResult result{};
    };

    bool LoadPosition(const std::string& file_name, std::string* error);
    std::uint64_t AdvanceGeneration();
    void StartEngineSearchIfNeeded();
    void CancelSearch();
    void RebuildSelection();
    void RebuildHoverPath();
    void RefreshDerivedState();
    void ApplyMove(bitboard::Move move, bool preserve_completed_snapshot = false);
    void ApplyCompletedSearchIfReady();
    void ResetSearchArtifacts(std::uint64_t generation, bool snapshot_active = false);
    void PublishSearchSnapshot(std::uint64_t generation, const bitboard::SearchSnapshot& snapshot);
    void RefreshSearchSnapshot();
    std::optional<bitboard::LegalTargetInfo> FindTarget(bitboard::Square destination) const;

    bitboard::Position position_{};
    bitboard::Color human_color_{bitboard::Color::Black};
    bitboard::SearchLimits limits_{};
    bitboard::SearchController controller_{};
    SessionStatus status_{};
    std::string loaded_file_;
    std::optional<bitboard::Square> selected_square_;
    std::optional<bitboard::Square> hovered_target_;
    std::optional<bitboard::Move> last_move_;
    std::vector<bitboard::LegalTargetInfo> legal_targets_;
    std::vector<SurakartaMovePathFragment> hover_path_;

    std::thread search_thread_;
    std::atomic<bool> search_running_{false};
    mutable std::mutex search_mutex_;
    std::optional<CompletedSearch> completed_search_;
    bitboard::SearchSnapshot ui_snapshot_{};
    std::uint64_t ui_snapshot_generation_{0};
    std::uint64_t generation_counter_{0};
    std::atomic<std::uint64_t> active_generation_{0};
};

}  // namespace surakarta::devtools
