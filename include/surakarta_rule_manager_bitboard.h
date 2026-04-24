#pragma once

#include "surakarta_bitboard.h"
#include "surakarta_rule_manager.h"

std::pair<SurakartaEndReason, SurakartaPlayer> EvaluateBitboardTerminal(const surakarta::bitboard::Position& position);

class SurakartaRuleManagerBitboard : public SurakartaRuleManager {
   public:
    SurakartaRuleManagerBitboard() = default;

    SurakartaRuleManagerBitboard(std::shared_ptr<SurakartaBoard> board,
                                 std::shared_ptr<SurakartaGameInfo> game_info)
        : SurakartaRuleManager(board, game_info) {}

    void OnUpdateBoard() override;
    SurakartaIllegalMoveReason JudgeMove(const SurakartaMove& move) override;
    std::pair<SurakartaEndReason, SurakartaPlayer> JudgeEnd(const SurakartaIllegalMoveReason reason) override;
    std::unique_ptr<std::vector<SurakartaPosition>> GetAllLegalTarget(const SurakartaPosition postion) override;
};
