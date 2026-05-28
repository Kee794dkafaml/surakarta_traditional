#include "surakarta_rule_manager_impl.h"
#include "surakarta_utils.h"

void SurakartaRuleManagerImpl::OnUpdateBoard() {
    // TODO:
    // Every time the board and game_info is updated to the next round version, this function will be called.
    // You don't need to implement this function if you don't need it.
    // A more delicate way is to use Qt's signal and slot mechanism, but that's the advanced part.
}

SurakartaIllegalMoveReason SurakartaRuleManagerImpl::JudgeMove(const SurakartaMove& move) {
    // Range check:
    if (board_->IsInside(move.from) == false || board_->IsInside(move.to) == false)
        return SurakartaIllegalMoveReason::OUT_OF_BOARD;

    // Some useful values:
    const auto piece_from = (*board_)[move.from.x][move.from.y];
    const auto piece_to = (*board_)[move.to.x][move.to.y];
    const auto color_from = piece_from->GetColor();
    const auto color_to = piece_to->GetColor();
    const auto curr_player = game_info_->current_player_;
    const auto oppo_player = ReverseColor(curr_player);

    // Ensure color is correct:
    if (color_from == PieceColor::NONE)
        return SurakartaIllegalMoveReason::NOT_PIECE;
    if (move.player != curr_player)
        return SurakartaIllegalMoveReason::NOT_PLAYER_TURN;
    if (color_from != curr_player)
        return SurakartaIllegalMoveReason::NOT_PLAYER_PIECE;
    if (color_to != oppo_player && color_to != PieceColor::NONE) {
        return SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
    }

    if (move.from.x == move.to.x && move.from.y == move.to.y)
        return SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
    if (move.player != game_info_->current_player_)
        return SurakartaIllegalMoveReason::NOT_PLAYER_TURN;

    const auto util = SurakartaMovablityUtil(board_);
    if (piece_to->GetColor() == PieceColor::NONE) {  // Non-capture case:
        if (util.IsMovableToNoneCapture(*piece_from, *piece_to))
            return SurakartaIllegalMoveReason::LEGAL_NON_CAPTURE_MOVE;
        else
            return SurakartaIllegalMoveReason::ILLIGAL_NON_CAPTURE_MOVE;
    } else {  // Capture case:
        if (util.IsMovableToCapture(*piece_from, *piece_to))
            return SurakartaIllegalMoveReason::LEGAL_CAPTURE_MOVE;
        else
            return SurakartaIllegalMoveReason::ILLIGAL_CAPTURE_MOVE;
    }
}

std::pair<SurakartaEndReason, SurakartaPlayer> SurakartaRuleManagerImpl::JudgeEnd(const SurakartaIllegalMoveReason reason) {
    const auto curr_colour = game_info_->current_player_;
    const auto oppo_colour = ReverseColor(curr_colour);

    if (IsLegalMoveReason(reason) == false)
        return std::pair(SurakartaEndReason::ILLIGAL_MOVE, oppo_colour);

    int black_remain = 0;
    int white_remain = 0;
    for (const auto& column : *board_) {
        for (const auto& piece : column) {
            if (piece->GetColor() == PieceColor::BLACK)
                black_remain++;
            if (piece->GetColor() == PieceColor::WHITE)
                white_remain++;
        }
    }

    if (black_remain == 0)
        return std::pair(SurakartaEndReason::CHECKMATE, PieceColor::WHITE);
    if (white_remain == 0)
        return std::pair(SurakartaEndReason::CHECKMATE, PieceColor::BLACK);

    const bool material_has_changed = black_remain < BOARD_SIZE * 2 || white_remain < BOARD_SIZE * 2;
    const auto capture_util = SurakartaPieceCanCaptureUtil(board_);
    if (material_has_changed &&
        !capture_util.CanCaptureOpponentPiece(PieceColor::BLACK) &&
        !capture_util.CanCaptureOpponentPiece(PieceColor::WHITE)) {
        if (black_remain > white_remain)
            return std::pair(SurakartaEndReason::STALEMATE, PieceColor::BLACK);
        else if (black_remain < white_remain)
            return std::pair(SurakartaEndReason::STALEMATE, PieceColor::WHITE);
        else
            return std::pair(SurakartaEndReason::STALEMATE, SurakartaPlayer::NONE);
    }

    return std::pair(SurakartaEndReason::NONE, PieceColor::NONE);
}

std::unique_ptr<std::vector<SurakartaPosition>> SurakartaRuleManagerImpl::GetAllLegalTarget(const SurakartaPosition postion) {
    if (board_->IsInside(postion) == false || board_->IsInside(postion) == false)
        return std::make_unique<std::vector<SurakartaPosition>>();
    if ((*board_)[postion.x][postion.y]->GetColor() == PieceColor::NONE)
        return std::make_unique<std::vector<SurakartaPosition>>();
    const auto piece_ptr = (*board_)[postion.x][postion.y];
    const auto util = SurakartaGetAllLegalTargetUtil(board_);
    return util.GetAllLegalTargets(*piece_ptr);
}
