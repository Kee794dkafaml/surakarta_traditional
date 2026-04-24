#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <ostream>
#include <vector>

#include "surakarta_common.h"

namespace surakarta::bitboard {

constexpr int kBoardSize = BOARD_SIZE;
constexpr int kSquareCount = BOARD_SIZE * BOARD_SIZE;
constexpr int kMoveListCapacity = 512;
constexpr int kMaxCaptureVariants = 4;
constexpr int kMaxPly = 64;
constexpr std::uint8_t kInvalidSquare = 0xFF;

using Bitboard = std::uint64_t;
using Square = std::uint8_t;

enum class Color : std::uint8_t {
    Black = 0,
    White = 1,
    None = 2,
};

enum class StraightDir : std::uint8_t {
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

enum MoveFlags : std::uint8_t {
    MoveNone = 0,
    MoveQuiet = 1 << 0,
    MoveCapture = 1 << 1,
    MoveNull = 1 << 2,
};

constexpr Square MakeSquare(int x, int y) {
    return static_cast<Square>(y * kBoardSize + x);
}

constexpr int SquareX(Square square) {
    return static_cast<int>(square % kBoardSize);
}

constexpr int SquareY(Square square) {
    return static_cast<int>(square / kBoardSize);
}

constexpr Bitboard Bit(Square square) {
    return Bitboard{1} << square;
}

constexpr bool IsValidSquare(Square square) {
    return square < kSquareCount;
}

constexpr Color ToBitboardColor(PieceColor color) {
    return color == PieceColor::BLACK
               ? Color::Black
           : color == PieceColor::WHITE
               ? Color::White
               : Color::None;
}

constexpr PieceColor ToLegacyColor(Color color) {
    return color == Color::Black
               ? PieceColor::BLACK
           : color == Color::White
               ? PieceColor::WHITE
               : PieceColor::NONE;
}

constexpr std::uint8_t ColorIndex(Color color) {
    return color == Color::White ? 1u : 0u;
}

struct Board {
    Bitboard pieces[2]{};

    constexpr Bitboard Occupied() const {
        return pieces[0] | pieces[1];
    }

    constexpr Bitboard Empty() const {
        return ~Occupied() & ((Bitboard{1} << kSquareCount) - 1);
    }

    constexpr Bitboard Pieces(Color color) const {
        return color == Color::Black ? pieces[0] : pieces[1];
    }

    constexpr bool HasPiece(Color color, Square square) const {
        return (Pieces(color) & Bit(square)) != 0;
    }

    constexpr Color ColorAt(Square square) const {
        if ((pieces[0] & Bit(square)) != 0) {
            return Color::Black;
        }
        if ((pieces[1] & Bit(square)) != 0) {
            return Color::White;
        }
        return Color::None;
    }

    void SetPiece(Color color, Square square);
    void ClearSquare(Square square);
    int Count(Color color) const;
};

struct Move {
    Square from{kInvalidSquare};
    Square to{kInvalidSquare};
    std::uint8_t flags{MoveNone};
    std::uint8_t aux{0};

    constexpr bool IsValid() const {
        return from != kInvalidSquare && to != kInvalidSquare;
    }

    constexpr bool IsCapture() const {
        return (flags & MoveCapture) != 0;
    }

    constexpr bool IsQuiet() const {
        return (flags & MoveQuiet) != 0;
    }

    constexpr bool IsNull() const {
        return (flags & MoveNull) != 0;
    }

    static constexpr Move Null() {
        return Move{kInvalidSquare, kInvalidSquare, MoveNull, 0};
    }
};

struct MoveList {
    std::array<Move, kMoveListCapacity> moves{};
    std::array<int, kMoveListCapacity> scores{};
    int size{0};

    void Clear();
    void Push(Move move, int score = 0);
};

struct LegalTargetInfo {
    Square from{kInvalidSquare};
    Square to{kInvalidSquare};
    std::uint8_t flags{MoveNone};
    std::uint8_t variant_count{0};
    std::uint8_t primary_variant{0};

    constexpr bool IsCapture() const {
        return (flags & MoveCapture) != 0;
    }

    constexpr bool IsQuiet() const {
        return (flags & MoveQuiet) != 0;
    }
};

struct Undo {
    std::uint64_t previous_key{0};
    int previous_eval_cache{0};
    std::uint16_t previous_no_capture_ply{0};
    std::uint16_t previous_ply{0};
    std::uint16_t previous_max_no_capture_round{0};
    std::uint8_t previous_side_to_move{0};
    std::uint8_t captured_square{kInvalidSquare};
    std::uint8_t captured_color{static_cast<std::uint8_t>(Color::None)};
};

struct alignas(64) Position {
    Board board{};
    std::uint64_t zobrist_key{0};
    int eval_cache{0};
    std::uint16_t no_capture_ply{0};
    std::uint16_t ply{0};
    std::uint16_t max_no_capture_round{MAX_NO_CAPTURE_ROUND};
    std::uint8_t side_to_move{static_cast<std::uint8_t>(Color::Black)};
    std::uint8_t reserved[5]{};

    constexpr Color SideToMove() const {
        return static_cast<Color>(side_to_move);
    }
};

struct BitboardTables {
    std::array<Bitboard, kSquareCount> quiet_move_mask{};
    std::array<Bitboard, kSquareCount> capture_target_mask{};
    std::array<std::array<std::uint8_t, kSquareCount>, kSquareCount> capture_variant_count{};
    std::array<std::array<std::array<Bitboard, kMaxCaptureVariants>, kSquareCount>, kSquareCount> capture_clear_mask{};
    std::array<std::array<std::array<std::uint8_t, kMaxCaptureVariants>, kSquareCount>, kSquareCount> capture_start_dir{};
};

class BitboardGenerator {
   public:
    BitboardTables Build() const;
};

const BitboardTables& GetBitboardTables();

class PositionBuilder {
   public:
    static Position FromLegacy(const SurakartaBoard& board, const SurakartaGameInfo& game_info);
};

class PositionAdapter {
   public:
    static Move FromLegacyMove(const SurakartaMove& move, const Position& position);
    static SurakartaMove ToLegacyMove(Move move, PieceColor player);
    static Square FromLegacyPosition(SurakartaPosition position);
    static SurakartaPosition ToLegacyPosition(Square square);
};

std::uint64_t ComputeZobrist(const Position& position);
bool IsCaptureLegal(const Position& position, Square from, Square to, std::uint8_t* variant_index = nullptr);
Bitboard GetLegalTargetMask(const Position& position, Square from);
Bitboard GetLegalTargetMask(const Position& position, Color mover, Square from);
std::vector<LegalTargetInfo> DescribeLegalTargets(const Position& position, Square from);
std::vector<LegalTargetInfo> DescribeLegalTargets(const Position& position, Color mover, Square from);
std::vector<SurakartaMovePathFragment> ReconstructCapturePath(const Position& position, Square from, Square to, std::uint8_t variant_index);
void GenerateMoves(const Position& position, MoveList& move_list, bool captures_only = false);
bool HasAnyLegalMove(const Position& position);
bool HasAnyQuietMove(const Position& position);
void MakeMove(Position& position, Move move, Undo& undo);
void UnmakeMove(Position& position, Move move, const Undo& undo);
void MakeNullMove(Position& position, Undo& undo);
void UnmakeNullMove(Position& position, const Undo& undo);
int MaterialBalance(const Position& position);
std::ostream& operator<<(std::ostream& os, Move move);

}  // namespace surakarta::bitboard
