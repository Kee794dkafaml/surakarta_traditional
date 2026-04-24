#include "surakarta_rule_manager_bitboard.h"

#include <bit>

#include "surakarta_utils.h"

using surakarta::bitboard::Color;
using surakarta::bitboard::GetLegalTargetMask;
using surakarta::bitboard::PositionAdapter;
using surakarta::bitboard::PositionBuilder;
using surakarta::bitboard::ToLegacyColor;

namespace {

surakarta::bitboard::Color Opponent(surakarta::bitboard::Color color) {
    return color == surakarta::bitboard::Color::Black
               ? surakarta::bitboard::Color::White
               : surakarta::bitboard::Color::Black;
}

surakarta::bitboard::Position BuildJudgeEndPosition(const SurakartaBoard& board,
                                                    const SurakartaGameInfo& game_info,
                                                    const SurakartaIllegalMoveReason reason) {
    auto position = PositionBuilder::FromLegacy(board, game_info);
    const auto current = surakarta::bitboard::ToBitboardColor(game_info.current_player_);
    const auto enemy = Opponent(current);

    if (reason == SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE) {
        auto enemy_pieces = position.board.Pieces(enemy);
        if (enemy_pieces != 0) {
            const auto captured = static_cast<surakarta::bitboard::Square>(std::countr_zero(enemy_pieces));
            position.board.ClearSquare(captured);
        }
        position.no_capture_ply = 0;
    } else if (reason == SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE) {
        position.no_capture_ply = static_cast<std::uint16_t>(position.no_capture_ply + 1);
    }
    return position;
}

}  // namespace

std::pair<SurakartaEndReason, SurakartaPlayer> EvaluateBitboardTerminal(const surakarta::bitboard::Position& position) {
    const auto black_count = position.board.Count(surakarta::bitboard::Color::Black);
    const auto white_count = position.board.Count(surakarta::bitboard::Color::White);

    if (black_count == 0) {
        return {SurakartaEndReason::CHECKMATE, SurakartaPlayer::WHITE};
    }
    if (white_count == 0) {
        return {SurakartaEndReason::CHECKMATE, SurakartaPlayer::BLACK};
    }

    if (position.no_capture_ply > position.max_no_capture_round) {
        if (black_count > white_count) {
            return {SurakartaEndReason::STALEMATE, SurakartaPlayer::BLACK};
        }
        if (white_count > black_count) {
            return {SurakartaEndReason::STALEMATE, SurakartaPlayer::WHITE};
        }
        return {SurakartaEndReason::STALEMATE, SurakartaPlayer::NONE};
    }

    return {SurakartaEndReason::NONE, SurakartaPlayer::NONE};
}

void SurakartaRuleManagerBitboard::OnUpdateBoard() {}

SurakartaIllegalMoveReason SurakartaRuleManagerBitboard::JudgeMove(const SurakartaMove& move) {
    if (!board_->IsInside(move.from) || !board_->IsInside(move.to)) {
        return SurakartaIllegalMoveReason::OUT_OF_BOARD;
    }

    const auto piece_from = (*board_)[move.from.x][move.from.y];
    const auto piece_to = (*board_)[move.to.x][move.to.y];
    const auto color_from = piece_from->GetColor();
    const auto color_to = piece_to->GetColor();
    const auto curr_player = game_info_->current_player_;
    const auto oppo_player = ReverseColor(curr_player);

    if (color_from == PieceColor::NONE) {
        return SurakartaIllegalMoveReason::NOT_PIECE;
    }
    if (move.player != curr_player) {
        return SurakartaIllegalMoveReason::NOT_PLAYER_TURN;
    }
    if (color_from != curr_player) {
        return SurakartaIllegalMoveReason::NOT_PLAYER_PIECE;
    }
    if (color_to != oppo_player && color_to != PieceColor::NONE) {
        return SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
    }
    if (move.from == move.to) {
        return SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
    }

    const auto position = PositionBuilder::FromLegacy(*board_, *game_info_);
    const auto from = PositionAdapter::FromLegacyPosition(move.from);
    const auto to = PositionAdapter::FromLegacyPosition(move.to);
    const auto legal_targets = GetLegalTargetMask(position, from);

    if (color_to == PieceColor::NONE) {
        return (legal_targets & surakarta::bitboard::Bit(to)) != 0
                   ? SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE
                   : SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
    }

    if ((legal_targets & surakarta::bitboard::Bit(to)) == 0) {
        return SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE;
    }
    return SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE;
}

std::pair<SurakartaEndReason, SurakartaPlayer> SurakartaRuleManagerBitboard::JudgeEnd(const SurakartaIllegalMoveReason reason) {
    const auto curr_colour = game_info_->current_player_;
    const auto oppo_colour = ReverseColor(curr_colour);

    if (!IsLegalMoveReason(reason)) {
        return std::pair(SurakartaEndReason::ILLIGAL_MOVE, oppo_colour);
    }

    const auto position = BuildJudgeEndPosition(*board_, *game_info_, reason);
    return EvaluateBitboardTerminal(position);
}

std::unique_ptr<std::vector<SurakartaPosition>> SurakartaRuleManagerBitboard::GetAllLegalTarget(const SurakartaPosition postion) {
    auto result = std::make_unique<std::vector<SurakartaPosition>>();
    if (!board_->IsInside(postion)) {
        return result;
    }
    if ((*board_)[postion.x][postion.y]->GetColor() == PieceColor::NONE) {
        return result;
    }

    const auto position = PositionBuilder::FromLegacy(*board_, *game_info_);
    const auto from = PositionAdapter::FromLegacyPosition(postion);
    const auto mover = surakarta::bitboard::ToBitboardColor((*board_)[postion.x][postion.y]->GetColor());
    auto targets = GetLegalTargetMask(position, mover, from);
    while (targets != 0) {
        const auto square = static_cast<surakarta::bitboard::Square>(std::countr_zero(targets));
        targets &= (targets - 1);
        result->push_back(PositionAdapter::ToLegacyPosition(square));
    }
    return result;
}
