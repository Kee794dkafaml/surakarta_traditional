#include "surakarta_bitboard.h"

#ifndef SURAKARTA_DYNAMIC_MASKS
#include "surakarta_masks.h"
#endif

#include <algorithm>
#include <bit>
#include <optional>
#include <stdexcept>
#include <string>

namespace surakarta::bitboard {

namespace {

constexpr std::array<std::pair<int, int>, 8> kQuietOffsets{{
    {-1, -1},
    {0, -1},
    {1, -1},
    {-1, 0},
    {1, 0},
    {-1, 1},
    {0, 1},
    {1, 1},
}};

constexpr std::array<StraightDir, 4> kStraightDirs{{
    StraightDir::Up,
    StraightDir::Down,
    StraightDir::Left,
    StraightDir::Right,
}};

struct NextResult {
    bool valid{false};
    Square square{kInvalidSquare};
    StraightDir direction{StraightDir::Up};
};

constexpr std::pair<int, int> Delta(StraightDir direction) {
    switch (direction) {
        case StraightDir::Up:
            return {0, -1};
        case StraightDir::Down:
            return {0, 1};
        case StraightDir::Left:
            return {-1, 0};
        case StraightDir::Right:
            return {1, 0};
    }
    return {0, 0};
}

NextResult NextCaptureStep(Square square, StraightDir direction) {
    const int x = SquareX(square);
    const int y = SquareY(square);
    const auto [dx, dy] = Delta(direction);
    const int next_x = x + dx;
    const int next_y = y + dy;

    if (next_x >= 0 && next_x < kBoardSize && next_y >= 0 && next_y < kBoardSize) {
        return {true, MakeSquare(next_x, next_y), direction};
    }

    if (next_x >= 1 && next_x < kBoardSize / 2 && next_y == -1) {
        return {true, MakeSquare(0, next_x), StraightDir::Right};
    }
    if (next_x >= kBoardSize / 2 && next_x < kBoardSize - 1 && next_y == -1) {
        return {true, MakeSquare(kBoardSize - 1, kBoardSize - next_x - 1), StraightDir::Left};
    }
    if (next_x >= 1 && next_x < kBoardSize / 2 && next_y == kBoardSize) {
        return {true, MakeSquare(0, kBoardSize - next_x - 1), StraightDir::Right};
    }
    if (next_x >= kBoardSize / 2 && next_x < kBoardSize - 1 && next_y == kBoardSize) {
        return {true, MakeSquare(kBoardSize - 1, next_x), StraightDir::Left};
    }
    if (next_x == -1 && next_y >= 1 && next_y < kBoardSize / 2) {
        return {true, MakeSquare(next_y, 0), StraightDir::Down};
    }
    if (next_x == -1 && next_y >= kBoardSize / 2 && next_y < kBoardSize - 1) {
        return {true, MakeSquare(kBoardSize - next_y - 1, kBoardSize - 1), StraightDir::Up};
    }
    if (next_x == kBoardSize && next_y >= 1 && next_y < kBoardSize / 2) {
        return {true, MakeSquare(kBoardSize - next_y - 1, 0), StraightDir::Down};
    }
    if (next_x == kBoardSize && next_y >= kBoardSize / 2 && next_y < kBoardSize - 1) {
        return {true, MakeSquare(next_y, kBoardSize - 1), StraightDir::Up};
    }

    return {};
}

std::uint64_t SplitMix64(std::uint64_t& state) {
    std::uint64_t result = (state += 0x9E3779B97F4A7C15ull);
    result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9ull;
    result = (result ^ (result >> 27)) * 0x94D049BB133111EBull;
    return result ^ (result >> 31);
}

const std::array<std::array<std::uint64_t, kSquareCount>, 2>& ZobristTable() {
    static const auto table = []() {
        auto result = std::array<std::array<std::uint64_t, kSquareCount>, 2>{};
        std::uint64_t seed = 0x534B544142424F44ull;
        for (auto& color_table : result) {
            for (auto& value : color_table) {
                value = SplitMix64(seed);
            }
        }
        return result;
    }();
    return table;
}

std::uint64_t SideZobrist() {
    static std::uint64_t side = []() {
        std::uint64_t seed = 0xB17B0A6D3E4F9981ull;
        return SplitMix64(seed);
    }();
    return side;
}

Square PopLsb(Bitboard& bitboard) {
    const auto index = static_cast<Square>(std::countr_zero(bitboard));
    bitboard &= (bitboard - 1);
    return index;
}

std::optional<SurakartaMovePathFragment> CapturePathFragment(Square square, StraightDir direction) {
    const int x = SquareX(square);
    const int y = SquareY(square);
    const auto [dx, dy] = Delta(direction);
    const int next_x = x + dx;
    const int next_y = y + dy;

    if (next_x >= 0 && next_x < kBoardSize && next_y >= 0 && next_y < kBoardSize) {
        return SurakartaMovePathFragment(x, y, next_x, next_y);
    }

    if (next_x >= 1 && next_x < kBoardSize / 2 && next_y == -1) {
        return SurakartaMovePathFragment(0, 0, next_x, 0, 1, true);
    }
    if (next_x >= kBoardSize / 2 && next_x < kBoardSize - 1 && next_y == -1) {
        return SurakartaMovePathFragment(kBoardSize - 1, 0, kBoardSize - next_x - 1, 2, 1, false);
    }
    if (next_x >= 1 && next_x < kBoardSize / 2 && next_y == kBoardSize) {
        return SurakartaMovePathFragment(0, kBoardSize - 1, next_x, 0, 3, false);
    }
    if (next_x >= kBoardSize / 2 && next_x < kBoardSize - 1 && next_y == kBoardSize) {
        return SurakartaMovePathFragment(kBoardSize - 1, kBoardSize - 1, kBoardSize - next_x - 1, 2, 3, true);
    }
    if (next_x == -1 && next_y >= 1 && next_y < kBoardSize / 2) {
        return SurakartaMovePathFragment(0, 0, next_y, 1, 0, false);
    }
    if (next_x == -1 && next_y >= kBoardSize / 2 && next_y < kBoardSize - 1) {
        return SurakartaMovePathFragment(0, kBoardSize - 1, kBoardSize - next_y - 1, 3, 0, true);
    }
    if (next_x == kBoardSize && next_y >= 1 && next_y < kBoardSize / 2) {
        return SurakartaMovePathFragment(kBoardSize - 1, 0, next_y, 1, 2, true);
    }
    if (next_x == kBoardSize && next_y >= kBoardSize / 2 && next_y < kBoardSize - 1) {
        return SurakartaMovePathFragment(kBoardSize - 1, kBoardSize - 1, kBoardSize - next_y - 1, 3, 2, false);
    }

    return std::nullopt;
}

std::uint8_t CountCaptureVariants(const Position& position, Square from, Square to, std::uint8_t* first_variant = nullptr) {
    const auto& tables = GetBitboardTables();
    const auto occupied = position.board.Occupied();
    const auto variant_count = tables.capture_variant_count[from][to];
    std::uint8_t legal_count = 0;
    for (std::uint8_t variant = 0; variant < variant_count; ++variant) {
        if ((occupied & tables.capture_clear_mask[from][to][variant]) == 0) {
            if (first_variant != nullptr && legal_count == 0) {
                *first_variant = variant;
            }
            ++legal_count;
        }
    }
    return legal_count;
}

}  // namespace

void Board::SetPiece(Color color, Square square) {
    ClearSquare(square);
    if (color != Color::None) {
        pieces[ColorIndex(color)] |= Bit(square);
    }
}

void Board::ClearSquare(Square square) {
    const auto mask = ~Bit(square);
    pieces[0] &= mask;
    pieces[1] &= mask;
}

int Board::Count(Color color) const {
    return std::popcount(Pieces(color));
}

void MoveList::Clear() {
    size = 0;
}

void MoveList::Push(Move move, int score) {
    if (size >= kMoveListCapacity) {
        throw std::runtime_error("MoveList capacity exceeded");
    }
    moves[size] = move;
    scores[size] = score;
    ++size;
}

BitboardTables BitboardGenerator::Build() const {
    auto tables = BitboardTables{};

    for (int y = 0; y < kBoardSize; ++y) {
        for (int x = 0; x < kBoardSize; ++x) {
            const auto from = MakeSquare(x, y);
            Bitboard quiet_mask = 0;
            for (const auto [dx, dy] : kQuietOffsets) {
                const int nx = x + dx;
                const int ny = y + dy;
                if (nx >= 0 && nx < kBoardSize && ny >= 0 && ny < kBoardSize) {
                    quiet_mask |= Bit(MakeSquare(nx, ny));
                }
            }
            tables.quiet_move_mask[from] = quiet_mask;

            for (const auto start_dir : kStraightDirs) {
                Square current = from;
                StraightDir current_dir = start_dir;
                int passed_corner_count = 0;
                Bitboard clear_mask = 0;

                while (true) {
                    const auto next = NextCaptureStep(current, current_dir);
                    if (!next.valid) {
                        break;
                    }
                    current = next.square;
                    if (next.direction != current_dir) {
                        ++passed_corner_count;
                    }
                    current_dir = next.direction;
                    if (current == from && passed_corner_count == 4) {
                        break;
                    }
                    if (passed_corner_count > 0 && current != from) {
                        tables.capture_target_mask[from] |= Bit(current);
                        auto& variant_count = tables.capture_variant_count[from][current];
                        if (variant_count >= kMaxCaptureVariants) {
                            throw std::runtime_error(
                                "capture variant count exceeds kMaxCaptureVariants from (" +
                                std::to_string(SquareX(from)) + "," + std::to_string(SquareY(from)) + ") to (" +
                                std::to_string(SquareX(current)) + "," + std::to_string(SquareY(current)) + ")");
                        }
                        tables.capture_clear_mask[from][current][variant_count] = clear_mask;
                        tables.capture_start_dir[from][current][variant_count] = static_cast<std::uint8_t>(start_dir);
                        ++variant_count;
                    }
                    if (current != from) {
                        clear_mask |= Bit(current);
                    }
                }
            }
        }
    }

    return tables;
}

const BitboardTables& GetBitboardTables() {
#ifdef SURAKARTA_DYNAMIC_MASKS
    static const auto dynamic_tables = BitboardGenerator().Build();
    return dynamic_tables;
#else
    return kStaticBitboardTables;
#endif
}

Position PositionBuilder::FromLegacy(const SurakartaBoard& board, const SurakartaGameInfo& game_info) {
    auto position = Position{};
    for (int x = 0; x < board.board_size_; ++x) {
        for (int y = 0; y < board.board_size_; ++y) {
            const auto color = ToBitboardColor(board[x][y]->GetColor());
            if (color != Color::None) {
                position.board.SetPiece(color, MakeSquare(x, y));
            }
        }
    }
    position.side_to_move = static_cast<std::uint8_t>(ToBitboardColor(game_info.current_player_));
    position.no_capture_ply = static_cast<std::uint16_t>(std::max(0, game_info.num_round_ - game_info.last_captured_round_));
    position.ply = static_cast<std::uint16_t>(std::max(0, game_info.num_round_ - 1));
    position.max_no_capture_round = static_cast<std::uint16_t>(game_info.max_no_capture_round_);
    position.eval_cache = MaterialBalance(position);
    position.zobrist_key = ComputeZobrist(position);
    return position;
}

Move PositionAdapter::FromLegacyMove(const SurakartaMove& move, const Position& position) {
    const auto from = FromLegacyPosition(move.from);
    const auto to = FromLegacyPosition(move.to);
    const auto target_color = position.board.ColorAt(to);
    if (target_color == Color::None) {
        return Move{from, to, MoveQuiet, 0};
    }
    std::uint8_t variant_index = 0;
    if (IsCaptureLegal(position, from, to, &variant_index)) {
        return Move{from, to, MoveCapture, variant_index};
    }
    return Move{from, to, MoveCapture, 0};
}

SurakartaMove PositionAdapter::ToLegacyMove(Move move, PieceColor player) {
    return SurakartaMove(ToLegacyPosition(move.from), ToLegacyPosition(move.to), player);
}

Square PositionAdapter::FromLegacyPosition(SurakartaPosition position) {
    return MakeSquare(position.x, position.y);
}

SurakartaPosition PositionAdapter::ToLegacyPosition(Square square) {
    return SurakartaPosition(SquareX(square), SquareY(square));
}

std::uint64_t ComputeZobrist(const Position& position) {
    const auto& table = ZobristTable();
    std::uint64_t key = 0;
    for (std::uint8_t color = 0; color < 2; ++color) {
        Bitboard pieces = position.board.pieces[color];
        while (pieces != 0) {
            const auto square = PopLsb(pieces);
            key ^= table[color][square];
        }
    }
    if (position.SideToMove() == Color::White) {
        key ^= SideZobrist();
    }
    return key;
}

bool IsCaptureLegal(const Position& position, Square from, Square to, std::uint8_t* variant_index) {
    const auto& tables = GetBitboardTables();
    if ((tables.capture_target_mask[from] & Bit(to)) == 0) {
        return false;
    }
    const auto occupied = position.board.Occupied();
    const auto variant_count = tables.capture_variant_count[from][to];
    for (std::uint8_t variant = 0; variant < variant_count; ++variant) {
        if ((occupied & tables.capture_clear_mask[from][to][variant]) == 0) {
            if (variant_index != nullptr) {
                *variant_index = variant;
            }
            return true;
        }
    }
    return false;
}

Bitboard GetLegalTargetMask(const Position& position, Color mover, Square from) {
    if (!position.board.HasPiece(mover, from)) {
        return 0;
    }
    const auto& tables = GetBitboardTables();
    const auto occupied = position.board.Occupied();
    Bitboard result = tables.quiet_move_mask[from] & ~occupied;
    const auto enemy = position.board.Pieces(mover == Color::Black ? Color::White : Color::Black);
    Bitboard candidates = enemy & tables.capture_target_mask[from];
    while (candidates != 0) {
        const auto to = PopLsb(candidates);
        if (IsCaptureLegal(position, from, to)) {
            result |= Bit(to);
        }
    }
    return result;
}

Bitboard GetLegalTargetMask(const Position& position, Square from) {
    return GetLegalTargetMask(position, position.SideToMove(), from);
}

std::vector<LegalTargetInfo> DescribeLegalTargets(const Position& position, Color mover, Square from) {
    auto result = std::vector<LegalTargetInfo>{};
    if (!position.board.HasPiece(mover, from)) {
        return result;
    }

    const auto& tables = GetBitboardTables();
    const auto occupied = position.board.Occupied();
    Bitboard quiet_targets = tables.quiet_move_mask[from] & ~occupied;
    while (quiet_targets != 0) {
        const auto to = PopLsb(quiet_targets);
        result.push_back(LegalTargetInfo{from, to, MoveQuiet, 0, 0});
    }

    const auto enemy = position.board.Pieces(mover == Color::Black ? Color::White : Color::Black);
    Bitboard capture_targets = enemy & tables.capture_target_mask[from];
    while (capture_targets != 0) {
        const auto to = PopLsb(capture_targets);
        std::uint8_t first_variant = 0;
        const auto legal_variants = CountCaptureVariants(position, from, to, &first_variant);
        if (legal_variants > 0) {
            result.push_back(LegalTargetInfo{from, to, MoveCapture, legal_variants, first_variant});
        }
    }

    return result;
}

std::vector<LegalTargetInfo> DescribeLegalTargets(const Position& position, Square from) {
    return DescribeLegalTargets(position, position.SideToMove(), from);
}

std::vector<SurakartaMovePathFragment> ReconstructCapturePath(const Position& position,
                                                              Square from,
                                                              Square to,
                                                              std::uint8_t variant_index) {
    auto path = std::vector<SurakartaMovePathFragment>{};
    if (!IsValidSquare(from) || !IsValidSquare(to)) {
        return path;
    }

    const auto mover = position.board.ColorAt(from);
    const auto target = position.board.ColorAt(to);
    if (mover == Color::None || target == Color::None || mover == target) {
        return path;
    }

    const auto& tables = GetBitboardTables();
    const auto variant_count = tables.capture_variant_count[from][to];
    if (variant_index >= variant_count) {
        return path;
    }
    if ((position.board.Occupied() & tables.capture_clear_mask[from][to][variant_index]) != 0) {
        return path;
    }

    auto current = from;
    auto direction = static_cast<StraightDir>(tables.capture_start_dir[from][to][variant_index]);
    int passed_corner_count = 0;
    int steps = 0;
    while (true) {
        if (++steps > 64) {
            return {};
        }
        const auto fragment = CapturePathFragment(current, direction);
        if (!fragment.has_value()) {
            return {};
        }
        path.push_back(fragment.value());

        const auto next = NextCaptureStep(current, direction);
        if (!next.valid) {
            return {};
        }

        current = next.square;
        if (next.direction != direction) {
            ++passed_corner_count;
        }
        direction = next.direction;

        if (current == to) {
            if (passed_corner_count > 0) {
                return path;
            }
            return {};
        }
        if (current != from && position.board.ColorAt(current) != Color::None) {
            return {};
        }
        if (current == from && passed_corner_count == 4) {
            return {};
        }
    }
}

void GenerateMoves(const Position& position, MoveList& move_list, bool captures_only) {
    move_list.Clear();
    const auto side = position.SideToMove();
    const auto enemy = side == Color::Black ? Color::White : Color::Black;
    Bitboard pieces = position.board.Pieces(side);
    const auto occupied = position.board.Occupied();
    const auto& tables = GetBitboardTables();

    while (pieces != 0) {
        const auto from = PopLsb(pieces);
        if (!captures_only) {
            Bitboard quiet_targets = tables.quiet_move_mask[from] & ~occupied;
            while (quiet_targets != 0) {
                const auto to = PopLsb(quiet_targets);
                move_list.Push(Move{from, to, MoveQuiet, 0});
            }
        }

        Bitboard capture_targets = position.board.Pieces(enemy) & tables.capture_target_mask[from];
        while (capture_targets != 0) {
            const auto to = PopLsb(capture_targets);
            std::uint8_t variant_index = 0;
            if (IsCaptureLegal(position, from, to, &variant_index)) {
                move_list.Push(Move{from, to, MoveCapture, variant_index});
            }
        }
    }
}

bool HasAnyLegalMove(const Position& position) {
    auto move_list = MoveList{};
    GenerateMoves(position, move_list);
    return move_list.size > 0;
}

bool HasAnyQuietMove(const Position& position) {
    auto move_list = MoveList{};
    GenerateMoves(position, move_list, false);
    for (int i = 0; i < move_list.size; ++i) {
        if (move_list.moves[i].IsQuiet()) {
            return true;
        }
    }
    return false;
}

void MakeMove(Position& position, Move move, Undo& undo) {
    undo.previous_key = position.zobrist_key;
    undo.previous_eval_cache = position.eval_cache;
    undo.previous_no_capture_ply = position.no_capture_ply;
    undo.previous_ply = position.ply;
    undo.previous_max_no_capture_round = position.max_no_capture_round;
    undo.previous_side_to_move = position.side_to_move;
    undo.captured_square = kInvalidSquare;
    undo.captured_color = static_cast<std::uint8_t>(Color::None);

    const auto side = position.SideToMove();
    const auto enemy = side == Color::Black ? Color::White : Color::Black;

    position.board.ClearSquare(move.from);
    if (move.IsCapture()) {
        undo.captured_square = move.to;
        undo.captured_color = static_cast<std::uint8_t>(enemy);
        position.board.ClearSquare(move.to);
        position.no_capture_ply = 0;
    } else {
        position.no_capture_ply = static_cast<std::uint16_t>(position.no_capture_ply + 1);
    }
    position.board.SetPiece(side, move.to);
    position.side_to_move = static_cast<std::uint8_t>(enemy);
    position.ply = static_cast<std::uint16_t>(position.ply + 1);
    position.eval_cache = MaterialBalance(position);
    position.zobrist_key = ComputeZobrist(position);
}

void UnmakeMove(Position& position, Move move, const Undo& undo) {
    position.side_to_move = undo.previous_side_to_move;
    position.no_capture_ply = undo.previous_no_capture_ply;
    position.ply = undo.previous_ply;
    position.max_no_capture_round = undo.previous_max_no_capture_round;
    position.eval_cache = undo.previous_eval_cache;
    position.zobrist_key = undo.previous_key;

    const auto side = position.SideToMove();
    position.board.ClearSquare(move.to);
    position.board.SetPiece(side, move.from);
    if (undo.captured_square != kInvalidSquare) {
        position.board.SetPiece(static_cast<Color>(undo.captured_color), undo.captured_square);
    }
}

void MakeNullMove(Position& position, Undo& undo) {
    undo.previous_key = position.zobrist_key;
    undo.previous_eval_cache = position.eval_cache;
    undo.previous_no_capture_ply = position.no_capture_ply;
    undo.previous_ply = position.ply;
    undo.previous_max_no_capture_round = position.max_no_capture_round;
    undo.previous_side_to_move = position.side_to_move;
    undo.captured_square = kInvalidSquare;
    undo.captured_color = static_cast<std::uint8_t>(Color::None);

    position.side_to_move = static_cast<std::uint8_t>(position.SideToMove() == Color::Black ? Color::White : Color::Black);
    position.no_capture_ply = static_cast<std::uint16_t>(position.no_capture_ply + 1);
    position.ply = static_cast<std::uint16_t>(position.ply + 1);
    position.zobrist_key = ComputeZobrist(position);
}

void UnmakeNullMove(Position& position, const Undo& undo) {
    position.side_to_move = undo.previous_side_to_move;
    position.no_capture_ply = undo.previous_no_capture_ply;
    position.ply = undo.previous_ply;
    position.max_no_capture_round = undo.previous_max_no_capture_round;
    position.eval_cache = undo.previous_eval_cache;
    position.zobrist_key = undo.previous_key;
}

int MaterialBalance(const Position& position) {
    return (position.board.Count(Color::Black) - position.board.Count(Color::White)) * 100;
}

std::ostream& operator<<(std::ostream& os, Move move) {
    if (move.IsNull()) {
        os << "null";
        return os;
    }
    os << "(" << SquareX(move.from) << ", " << SquareY(move.from) << ") -> "
       << "(" << SquareX(move.to) << ", " << SquareY(move.to) << ")";
    if (move.IsCapture()) {
        os << " x";
    }
    return os;
}

}  // namespace surakarta::bitboard
