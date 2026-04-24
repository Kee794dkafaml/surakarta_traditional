#pragma once

#include <array>
#include <cstdint>

#include "surakarta_bitboard.h"

namespace surakarta::bitboard {

inline constexpr BitboardTables kStaticBitboardTables = [] {
    auto tables = BitboardTables{};
    tables.quiet_move_mask = std::array<Bitboard, 36>{
        0xC2ull,
        0x1C5ull,
        0x38Aull,
        0x714ull,
        0xE28ull,
        0xC10ull,
        0x3083ull,
        0x7147ull,
        0xE28Eull,
        0x1C51Cull,
        0x38A38ull,
        0x30430ull,
        0xC20C0ull,
        0x1C51C0ull,
        0x38A380ull,
        0x714700ull,
        0xE28E00ull,
        0xC10C00ull,
        0x3083000ull,
        0x7147000ull,
        0xE28E000ull,
        0x1C51C000ull,
        0x38A38000ull,
        0x30430000ull,
        0xC20C0000ull,
        0x1C51C0000ull,
        0x38A380000ull,
        0x714700000ull,
        0xE28E00000ull,
        0xC10C00000ull,
        0x83000000ull,
        0x147000000ull,
        0x28E000000ull,
        0x51C000000ull,
        0xA38000000ull,
        0x430000000ull
    };
    tables.capture_target_mask = std::array<Bitboard, 36>{
        0x0ull,
        0x4BF492FD0ull,
        0x30CFFF308ull,
        0x30CFFF304ull,
        0x4BF492FC2ull,
        0x0ull,
        0x4BF492F92ull,
        0x4BF492F52ull,
        0x7BFFFFEDEull,
        0x7BFFFFDDEull,
        0x4BF492BD2ull,
        0x4BF4927D2ull,
        0x30CFFE30Cull,
        0x7BFFFDFDEull,
        0x30CFFB30Cull,
        0x30CFF730Cull,
        0x7BFFEFFDEull,
        0x30CFDF30Cull,
        0x30CFBF30Cull,
        0x7BFF7FFDEull,
        0x30CEFF30Cull,
        0x30CDFF30Cull,
        0x7BFBFFFDEull,
        0x30C7FF30Cull,
        0x4BE492FD2ull,
        0x4BD492FD2ull,
        0x7BBFFFFDEull,
        0x7B7FFFFDEull,
        0x4AF492FD2ull,
        0x49F492FD2ull,
        0x0ull,
        0x43F492FD2ull,
        0x20CFFF30Cull,
        0x10CFFF30Cull,
        0xBF492FD2ull,
        0x0ull
    };
    tables.capture_variant_count = std::array<std::array<std::uint8_t, 36>, 36>{
        std::array<std::uint8_t, 36>{
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            0,
            0,
            2,
            0,
            2,
            3,
            2,
            2,
            4,
            2,
            0,
            1,
            0,
            0,
            2,
            0,
            0,
            1,
            0,
            0,
            2,
            0,
            2,
            3,
            2,
            2,
            4,
            2,
            0,
            1,
            0,
            0,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            0,
            2,
            0,
            0,
            0,
            0,
            1,
            2,
            0,
            0,
            2,
            2,
            3,
            4,
            2,
            2,
            2,
            2,
            3,
            4,
            2,
            2,
            0,
            0,
            1,
            2,
            0,
            0,
            0,
            0,
            1,
            2,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            2,
            0,
            0,
            0,
            0,
            0,
            2,
            1,
            0,
            0,
            2,
            2,
            4,
            3,
            2,
            2,
            2,
            2,
            4,
            3,
            2,
            2,
            0,
            0,
            2,
            1,
            0,
            0,
            0,
            0,
            2,
            1,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            0,
            0,
            0,
            0,
            2,
            4,
            2,
            2,
            3,
            2,
            0,
            2,
            0,
            0,
            1,
            0,
            0,
            2,
            0,
            0,
            1,
            0,
            2,
            4,
            2,
            2,
            3,
            2,
            0,
            2,
            0,
            0,
            1,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            0,
            0,
            2,
            0,
            0,
            3,
            1,
            1,
            3,
            1,
            0,
            2,
            0,
            0,
            2,
            0,
            0,
            2,
            0,
            0,
            2,
            0,
            2,
            4,
            2,
            2,
            4,
            2,
            0,
            2,
            0,
            0,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            3,
            0,
            0,
            4,
            0,
            3,
            0,
            3,
            3,
            4,
            3,
            0,
            3,
            0,
            0,
            4,
            0,
            0,
            3,
            0,
            0,
            4,
            0,
            4,
            4,
            4,
            4,
            4,
            4,
            0,
            3,
            0,
            0,
            4,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            1,
            2,
            2,
            0,
            1,
            3,
            0,
            3,
            3,
            1,
            2,
            4,
            3,
            4,
            4,
            2,
            2,
            4,
            3,
            4,
            4,
            2,
            2,
            4,
            3,
            4,
            4,
            2,
            0,
            2,
            1,
            2,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            2,
            1,
            2,
            0,
            1,
            3,
            3,
            0,
            3,
            1,
            2,
            4,
            4,
            3,
            4,
            2,
            2,
            4,
            4,
            3,
            4,
            2,
            2,
            4,
            4,
            3,
            4,
            2,
            0,
            2,
            2,
            1,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            4,
            0,
            0,
            3,
            0,
            3,
            4,
            3,
            3,
            0,
            3,
            0,
            4,
            0,
            0,
            3,
            0,
            0,
            4,
            0,
            0,
            3,
            0,
            4,
            4,
            4,
            4,
            4,
            4,
            0,
            4,
            0,
            0,
            3,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            0,
            0,
            2,
            0,
            1,
            3,
            1,
            1,
            3,
            0,
            0,
            2,
            0,
            0,
            2,
            0,
            0,
            2,
            0,
            0,
            2,
            0,
            2,
            4,
            2,
            2,
            4,
            2,
            0,
            2,
            0,
            0,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            2,
            2,
            0,
            0,
            0,
            0,
            2,
            2,
            0,
            0,
            0,
            1,
            3,
            3,
            1,
            1,
            2,
            2,
            4,
            4,
            2,
            2,
            0,
            0,
            2,
            2,
            0,
            0,
            0,
            0,
            2,
            2,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            1,
            2,
            2,
            2,
            0,
            2,
            3,
            4,
            4,
            4,
            2,
            1,
            0,
            3,
            3,
            3,
            1,
            2,
            3,
            4,
            4,
            4,
            2,
            2,
            3,
            4,
            4,
            4,
            2,
            0,
            1,
            2,
            2,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            3,
            4,
            0,
            0,
            0,
            0,
            3,
            4,
            0,
            0,
            3,
            3,
            0,
            4,
            3,
            3,
            4,
            4,
            4,
            4,
            4,
            4,
            0,
            0,
            3,
            4,
            0,
            0,
            0,
            0,
            3,
            4,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            4,
            3,
            0,
            0,
            0,
            0,
            4,
            3,
            0,
            0,
            3,
            3,
            4,
            0,
            3,
            3,
            4,
            4,
            4,
            4,
            4,
            4,
            0,
            0,
            4,
            3,
            0,
            0,
            0,
            0,
            4,
            3,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            2,
            2,
            1,
            0,
            2,
            4,
            4,
            4,
            3,
            2,
            1,
            3,
            3,
            3,
            0,
            1,
            2,
            4,
            4,
            4,
            3,
            2,
            2,
            4,
            4,
            4,
            3,
            2,
            0,
            2,
            2,
            2,
            1,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            2,
            2,
            0,
            0,
            0,
            0,
            2,
            2,
            0,
            0,
            1,
            1,
            3,
            3,
            1,
            0,
            2,
            2,
            4,
            4,
            2,
            2,
            0,
            0,
            2,
            2,
            0,
            0,
            0,
            0,
            2,
            2,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            2,
            2,
            0,
            0,
            0,
            0,
            2,
            2,
            0,
            0,
            2,
            2,
            4,
            4,
            2,
            2,
            0,
            1,
            3,
            3,
            1,
            1,
            0,
            0,
            2,
            2,
            0,
            0,
            0,
            0,
            2,
            2,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            1,
            2,
            2,
            2,
            0,
            2,
            3,
            4,
            4,
            4,
            2,
            2,
            3,
            4,
            4,
            4,
            2,
            1,
            0,
            3,
            3,
            3,
            1,
            2,
            3,
            4,
            4,
            4,
            2,
            0,
            1,
            2,
            2,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            3,
            4,
            0,
            0,
            0,
            0,
            3,
            4,
            0,
            0,
            4,
            4,
            4,
            4,
            4,
            4,
            3,
            3,
            0,
            4,
            3,
            3,
            0,
            0,
            3,
            4,
            0,
            0,
            0,
            0,
            3,
            4,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            4,
            3,
            0,
            0,
            0,
            0,
            4,
            3,
            0,
            0,
            4,
            4,
            4,
            4,
            4,
            4,
            3,
            3,
            4,
            0,
            3,
            3,
            0,
            0,
            4,
            3,
            0,
            0,
            0,
            0,
            4,
            3,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            2,
            2,
            1,
            0,
            2,
            4,
            4,
            4,
            3,
            2,
            2,
            4,
            4,
            4,
            3,
            2,
            1,
            3,
            3,
            3,
            0,
            1,
            2,
            4,
            4,
            4,
            3,
            2,
            0,
            2,
            2,
            2,
            1,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            2,
            2,
            0,
            0,
            0,
            0,
            2,
            2,
            0,
            0,
            2,
            2,
            4,
            4,
            2,
            2,
            1,
            1,
            3,
            3,
            1,
            0,
            0,
            0,
            2,
            2,
            0,
            0,
            0,
            0,
            2,
            2,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            0,
            0,
            2,
            0,
            2,
            4,
            2,
            2,
            4,
            2,
            0,
            2,
            0,
            0,
            2,
            0,
            0,
            2,
            0,
            0,
            2,
            0,
            0,
            3,
            1,
            1,
            3,
            1,
            0,
            2,
            0,
            0,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            3,
            0,
            0,
            4,
            0,
            4,
            4,
            4,
            4,
            4,
            4,
            0,
            3,
            0,
            0,
            4,
            0,
            0,
            3,
            0,
            0,
            4,
            0,
            3,
            0,
            3,
            3,
            4,
            3,
            0,
            3,
            0,
            0,
            4,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            1,
            2,
            2,
            0,
            2,
            4,
            3,
            4,
            4,
            2,
            2,
            4,
            3,
            4,
            4,
            2,
            2,
            4,
            3,
            4,
            4,
            2,
            1,
            3,
            0,
            3,
            3,
            1,
            0,
            2,
            1,
            2,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            2,
            1,
            2,
            0,
            2,
            4,
            4,
            3,
            4,
            2,
            2,
            4,
            4,
            3,
            4,
            2,
            2,
            4,
            4,
            3,
            4,
            2,
            1,
            3,
            3,
            0,
            3,
            1,
            0,
            2,
            2,
            1,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            4,
            0,
            0,
            3,
            0,
            4,
            4,
            4,
            4,
            4,
            4,
            0,
            4,
            0,
            0,
            3,
            0,
            0,
            4,
            0,
            0,
            3,
            0,
            3,
            4,
            3,
            3,
            0,
            3,
            0,
            4,
            0,
            0,
            3,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            0,
            0,
            2,
            0,
            2,
            4,
            2,
            2,
            4,
            2,
            0,
            2,
            0,
            0,
            2,
            0,
            0,
            2,
            0,
            0,
            2,
            0,
            1,
            3,
            1,
            1,
            3,
            0,
            0,
            2,
            0,
            0,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            1,
            0,
            0,
            2,
            0,
            2,
            3,
            2,
            2,
            4,
            2,
            0,
            1,
            0,
            0,
            2,
            0,
            0,
            1,
            0,
            0,
            2,
            0,
            2,
            3,
            2,
            2,
            4,
            2,
            0,
            0,
            0,
            0,
            2,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            1,
            2,
            0,
            0,
            0,
            0,
            1,
            2,
            0,
            0,
            2,
            2,
            3,
            4,
            2,
            2,
            2,
            2,
            3,
            4,
            2,
            2,
            0,
            0,
            1,
            2,
            0,
            0,
            0,
            0,
            0,
            2,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            2,
            1,
            0,
            0,
            0,
            0,
            2,
            1,
            0,
            0,
            2,
            2,
            4,
            3,
            2,
            2,
            2,
            2,
            4,
            3,
            2,
            2,
            0,
            0,
            2,
            1,
            0,
            0,
            0,
            0,
            2,
            0,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            2,
            0,
            0,
            1,
            0,
            2,
            4,
            2,
            2,
            3,
            2,
            0,
            2,
            0,
            0,
            1,
            0,
            0,
            2,
            0,
            0,
            1,
            0,
            2,
            4,
            2,
            2,
            3,
            2,
            0,
            2,
            0,
            0,
            0,
            0
        },
        std::array<std::uint8_t, 36>{
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        }
    };
    tables.capture_clear_mask = std::array<std::array<std::array<Bitboard, 4>, 36>, 36>{
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xFC0ull,
                0x4BF492480ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x4BF492F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x40ull,
                0x4BF492FD0ull,
                0x4BF492F90ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xC0ull,
                0x4BF492E90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1C0ull,
                0x4BF492C90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3C0ull,
                0xFD0ull,
                0x4BF492080ull,
                0x4BF492C90ull
            },
            std::array<Bitboard, 4>{
                0x7C0ull,
                0x4BF492490ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF490FD0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xFD0ull,
                0x4BF482080ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF410FD0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10FD0ull,
                0x4BF082080ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43E410FD0ull,
                0x82082080ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43C410FD0ull,
                0x4BF410FD0ull,
                0x83082080ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x438410FD0ull,
                0x83082080ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x430410FD0ull,
                0x87082080ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x410FD0ull,
                0x430410FD0ull,
                0x8F082080ull,
                0x4BF082080ull
            },
            std::array<Bitboard, 4>{
                0x410410FD0ull,
                0x9F082080ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43F410FD0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410FD0ull,
                0xBF082080ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F000ull,
                0x30CFCC300ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFFF208ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F008ull,
                0x30CFCC100ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x30CFFE308ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1000ull,
                0x30CFFC308ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3000ull,
                0x30CFFF208ull,
                0x30CFFC308ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x7000ull,
                0x3F208ull,
                0x30CFC4100ull,
                0x30CFFC308ull
            },
            std::array<Bitboard, 4>{
                0xF000ull,
                0x30CFEC308ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1F000ull,
                0x30CFCC308ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208FBF208ull,
                0x104104100ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208F3F208ull,
                0x104144100ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208E3F208ull,
                0x30CFFF208ull,
                0x1041C4100ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F208ull,
                0x208E3F208ull,
                0x1041C4100ull,
                0x30CFC4100ull
            },
            std::array<Bitboard, 4>{
                0x208A3F208ull,
                0x1043C4100ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20823F208ull,
                0x1047C4100ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x308FFF208ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x23F208ull,
                0x304FC4100ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208FFF208ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x823F208ull,
                0x104FC4100ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F000ull,
                0x30CFCC300ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F004ull,
                0x30CFCC200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFFF104ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3E000ull,
                0x30CFCC304ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3C000ull,
                0x30CFCD304ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x38000ull,
                0x3F104ull,
                0x30CFC8200ull,
                0x30CFCF304ull
            },
            std::array<Bitboard, 4>{
                0x30000ull,
                0x30CFFF104ull,
                0x30CFCF304ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20000ull,
                0x30CFCF304ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x30CFDF304ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10413F104ull,
                0x208F88200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10417F104ull,
                0x208F08200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F104ull,
                0x1041FF104ull,
                0x208E08200ull,
                0x30CFC8200ull
            },
            std::array<Bitboard, 4>{
                0x1041FF104ull,
                0x30CFFF104ull,
                0x208E08200ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1043FF104ull,
                0x208A08200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047FF104ull,
                0x208208200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x13F104ull,
                0x308FC8200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x304FFF104ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x413F104ull,
                0x208FC8200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104FFF104ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xFC0ull,
                0x4BF492480ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xF80ull,
                0x4BF492482ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xF00ull,
                0xFC2ull,
                0x4BF492400ull,
                0x4BF4924C2ull
            },
            std::array<Bitboard, 4>{
                0xE00ull,
                0x4BF4924C2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xC00ull,
                0x4BF4925C2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x800ull,
                0x4BF492FC2ull,
                0x4BF4927C2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x4BF4927C2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xFC2ull,
                0x4BF490400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF482FC2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2FC2ull,
                0x4BF410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF082FC2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82082FC2ull,
                0x43E410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82FC2ull,
                0x83082FC2ull,
                0x43C410400ull,
                0x4BF410400ull
            },
            std::array<Bitboard, 4>{
                0x83082FC2ull,
                0x438410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x87082FC2ull,
                0x430410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x8F082FC2ull,
                0x4BF082FC2ull,
                0x430410400ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F082FC2ull,
                0x410410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2082FC2ull,
                0x43F410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBF082FC2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x4BF492F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF492482ull,
                0xF80ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2ull,
                0x4BF492F92ull,
                0x4BF492F90ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF492E92ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF492C92ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF492082ull,
                0x4BF492C92ull,
                0xF90ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF492492ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82ull,
                0x4BF490F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF482082ull,
                0xF90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2082ull,
                0x4BF410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF082082ull,
                0x10F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82082082ull,
                0x43E410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82082ull,
                0x83082082ull,
                0x43C410F90ull,
                0x4BF410F90ull
            },
            std::array<Bitboard, 4>{
                0x83082082ull,
                0x438410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x87082082ull,
                0x430410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x8F082082ull,
                0x4BF082082ull,
                0x410F90ull,
                0x430410F90ull
            },
            std::array<Bitboard, 4>{
                0x9F082082ull,
                0x410410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2082082ull,
                0x43F410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBF082082ull,
                0x10410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF492F50ull,
                0x40ull,
                0x4BF492F10ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xF42ull,
                0x4BF492400ull,
                0x4BF492442ull,
                0xF00ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2ull,
                0x4BF492F10ull,
                0x4BF492F12ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x42ull,
                0x4BF492E10ull,
                0x4BF492E52ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x142ull,
                0x4BF492C10ull,
                0x4BF492C52ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x342ull,
                0xF52ull,
                0x4BF492000ull,
                0x4BF492C10ull
            },
            std::array<Bitboard, 4>{
                0x742ull,
                0x4BF492410ull,
                0x4BF492452ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF490F52ull,
                0x42ull,
                0x4BF490F10ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xF52ull,
                0x4BF482000ull,
                0x4BF482042ull,
                0xF10ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF410F52ull,
                0x2042ull,
                0x4BF410F10ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10F52ull,
                0x4BF082000ull,
                0x4BF082042ull,
                0x10F10ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43E410F52ull,
                0x82082000ull,
                0x82082042ull,
                0x43E410F10ull
            },
            std::array<Bitboard, 4>{
                0x43C410F52ull,
                0x4BF410F52ull,
                0x83082000ull,
                0x82042ull
            },
            std::array<Bitboard, 4>{
                0x438410F52ull,
                0x83082000ull,
                0x83082042ull,
                0x438410F10ull
            },
            std::array<Bitboard, 4>{
                0x430410F52ull,
                0x87082000ull,
                0x87082042ull,
                0x430410F10ull
            },
            std::array<Bitboard, 4>{
                0x410F52ull,
                0x430410F52ull,
                0x8F082000ull,
                0x4BF082000ull
            },
            std::array<Bitboard, 4>{
                0x410410F52ull,
                0x9F082000ull,
                0x9F082042ull,
                0x410410F10ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43F410F52ull,
                0x2082042ull,
                0x43F410F10ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410F52ull,
                0xBF082000ull,
                0xBF082042ull,
                0x10410F10ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xC0ull,
                0x4BF492E90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFFF208ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F004ull,
                0x30CFCC200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF4924C2ull,
                0xE00ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF492E92ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xC2ull,
                0x4BF492E10ull,
                0x4BF492ED2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F00Cull,
                0x30CFCC000ull,
                0x4BF492CD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF4920C2ull,
                0x4BF492CD2ull,
                0xE10ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF4924D2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4ull,
                0x30CFFE208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1004ull,
                0x30CFFC208ull,
                0xC2ull,
                0x4BF490E10ull
            },
            std::array<Bitboard, 4>{
                0x3004ull,
                0x30CFFF20Cull,
                0x30CFFC208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x7004ull,
                0x3F20Cull,
                0x30CFC4000ull,
                0x30CFFC208ull
            },
            std::array<Bitboard, 4>{
                0xF004ull,
                0x30CFEC208ull,
                0x4BF4820C2ull,
                0xE10ull
            },
            std::array<Bitboard, 4>{
                0x1F004ull,
                0x30CFCC208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208FBF20Cull,
                0x104104000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208F3F20Cull,
                0x104144000ull,
                0x20C2ull,
                0x4BF410E10ull
            },
            std::array<Bitboard, 4>{
                0x208E3F20Cull,
                0x30CFFF20Cull,
                0x1041C4000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F20Cull,
                0x208E3F20Cull,
                0x1041C4000ull,
                0x30CFC4000ull
            },
            std::array<Bitboard, 4>{
                0x208A3F20Cull,
                0x1043C4000ull,
                0x4BF0820C2ull,
                0x10E10ull
            },
            std::array<Bitboard, 4>{
                0x20823F20Cull,
                0x1047C4000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x820820C2ull,
                0x43E410E10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x820C2ull,
                0x830820C2ull,
                0x43C410E10ull,
                0x4BF410E10ull
            },
            std::array<Bitboard, 4>{
                0x308FFF20Cull,
                0x830820C2ull,
                0x438410E10ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x23F20Cull,
                0x304FC4000ull,
                0x870820C2ull,
                0x430410E10ull
            },
            std::array<Bitboard, 4>{
                0x8F0820C2ull,
                0x4BF0820C2ull,
                0x410E10ull,
                0x430410E10ull
            },
            std::array<Bitboard, 4>{
                0x9F0820C2ull,
                0x410410E10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20820C2ull,
                0x43F410E10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208FFF20Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x823F20Cull,
                0x104FC4000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBF0820C2ull,
                0x10410E10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1C0ull,
                0x4BF492C90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F008ull,
                0x30CFCC100ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFFF104ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF4925C2ull,
                0xC00ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF492C92ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1C2ull,
                0x4BF492C10ull,
                0x4BF492CD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F00Cull,
                0x30CFCC000ull,
                0x4BF492CD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF4921C2ull,
                0x4BF492DD2ull,
                0xC10ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF4925D2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3E008ull,
                0x30CFCC104ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3C008ull,
                0x30CFCD104ull,
                0x1C2ull,
                0x4BF490C10ull
            },
            std::array<Bitboard, 4>{
                0x38008ull,
                0x3F10Cull,
                0x30CFC8000ull,
                0x30CFCF104ull
            },
            std::array<Bitboard, 4>{
                0x30008ull,
                0x30CFFF10Cull,
                0x30CFCF104ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20008ull,
                0x30CFCF104ull,
                0x4BF4821C2ull,
                0xC10ull
            },
            std::array<Bitboard, 4>{
                0x8ull,
                0x30CFDF104ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10413F10Cull,
                0x208F88000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10417F10Cull,
                0x208F08000ull,
                0x21C2ull,
                0x4BF410C10ull
            },
            std::array<Bitboard, 4>{
                0x3F10Cull,
                0x1041FF10Cull,
                0x208E08000ull,
                0x30CFC8000ull
            },
            std::array<Bitboard, 4>{
                0x1041FF10Cull,
                0x30CFFF10Cull,
                0x208E08000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1043FF10Cull,
                0x208A08000ull,
                0x4BF0821C2ull,
                0x10C10ull
            },
            std::array<Bitboard, 4>{
                0x1047FF10Cull,
                0x208208000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x820821C2ull,
                0x43E410C10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x821C2ull,
                0x830821C2ull,
                0x43C410C10ull,
                0x4BF410C10ull
            },
            std::array<Bitboard, 4>{
                0x13F10Cull,
                0x308FC8000ull,
                0x830821C2ull,
                0x438410C10ull
            },
            std::array<Bitboard, 4>{
                0x304FFF10Cull,
                0x870821C2ull,
                0x430410C10ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x8F0821C2ull,
                0x4BF0821C2ull,
                0x410C10ull,
                0x430410C10ull
            },
            std::array<Bitboard, 4>{
                0x9F0821C2ull,
                0x410410C10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20821C2ull,
                0x43F410C10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x413F10Cull,
                0x208FC8000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104FFF10Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBF0821C2ull,
                0x10410C10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBD0ull,
                0x4BF492080ull,
                0x3C0ull,
                0x4BF492890ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF492BC2ull,
                0x4BF4923C2ull,
                0x800ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xB90ull,
                0x4BF492082ull,
                0x4BF492892ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xB10ull,
                0xBD2ull,
                0x4BF492000ull,
                0x4BF4920C2ull
            },
            std::array<Bitboard, 4>{
                0xA10ull,
                0x4BF4920C2ull,
                0x4BF4928D2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x810ull,
                0x4BF4921C2ull,
                0x4BF4929D2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10ull,
                0x4BF4923C2ull,
                0x4BF4923D2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBD2ull,
                0x4BF490000ull,
                0x3C2ull,
                0x4BF490810ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF482BD2ull,
                0x4BF4823C2ull,
                0x810ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2BD2ull,
                0x4BF410000ull,
                0x23C2ull,
                0x4BF410810ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF082BD2ull,
                0x4BF0823C2ull,
                0x10810ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82082BD2ull,
                0x43E410000ull,
                0x820823C2ull,
                0x43E410810ull
            },
            std::array<Bitboard, 4>{
                0x82BD2ull,
                0x83082BD2ull,
                0x43C410000ull,
                0x4BF410000ull
            },
            std::array<Bitboard, 4>{
                0x83082BD2ull,
                0x438410000ull,
                0x830823C2ull,
                0x438410810ull
            },
            std::array<Bitboard, 4>{
                0x87082BD2ull,
                0x430410000ull,
                0x870823C2ull,
                0x430410810ull
            },
            std::array<Bitboard, 4>{
                0x8F082BD2ull,
                0x4BF082BD2ull,
                0x430410000ull,
                0x8F0823C2ull
            },
            std::array<Bitboard, 4>{
                0x9F082BD2ull,
                0x410410000ull,
                0x9F0823C2ull,
                0x410410810ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2082BD2ull,
                0x43F410000ull,
                0x20823C2ull,
                0x43F410810ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBF082BD2ull,
                0xBF0823C2ull,
                0x10410810ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x7C0ull,
                0x4BF492490ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF4927C2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF492492ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x7C2ull,
                0x4BF492410ull,
                0x4BF4924D2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF4924D2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF4925D2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF4927C2ull,
                0x10ull,
                0x4BF4927D2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x7C2ull,
                0x4BF490410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF4827C2ull,
                0x410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x27C2ull,
                0x4BF410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF0827C2ull,
                0x10410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x820827C2ull,
                0x43E410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x827C2ull,
                0x830827C2ull,
                0x43C410410ull,
                0x4BF410410ull
            },
            std::array<Bitboard, 4>{
                0x830827C2ull,
                0x438410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x870827C2ull,
                0x430410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x8F0827C2ull,
                0x4BF0827C2ull,
                0x410410ull,
                0x430410410ull
            },
            std::array<Bitboard, 4>{
                0x9F0827C2ull,
                0x410410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20827C2ull,
                0x43F410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBF0827C2ull,
                0x10410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x30CFFE308ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFCC304ull,
                0x3E000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4ull,
                0x30CFFE208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFCC104ull,
                0x3E008ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFFC30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104ull,
                0x30CFFC30Cull,
                0x30CFFE208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFC4104ull,
                0x30CFFC30Cull,
                0x3E208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFEC30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFCC30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104104104ull,
                0x208FBE208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104144104ull,
                0x208F3E208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4104ull,
                0x1041C4104ull,
                0x208E3E208ull,
                0x30CFFE208ull
            },
            std::array<Bitboard, 4>{
                0x1041C4104ull,
                0x30CFC4104ull,
                0x3E208ull,
                0x208E3E208ull
            },
            std::array<Bitboard, 4>{
                0x1043C4104ull,
                0x208A3E208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047C4104ull,
                0x20823E208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104104ull,
                0x308FFE208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x304FC4104ull,
                0x23E208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4104104ull,
                0x208FFE208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104FC4104ull,
                0x823E208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF490FD0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1000ull,
                0x30CFFC308ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFCD304ull,
                0x3C000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xFC2ull,
                0x4BF490400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82ull,
                0x4BF490F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xC2ull,
                0x4BF490F10ull,
                0x4BF490FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xC2ull,
                0x4BF490E10ull,
                0x1004ull,
                0x30CFFC208ull
            },
            std::array<Bitboard, 4>{
                0x1C2ull,
                0x4BF490C10ull,
                0x30CFCD104ull,
                0x3C008ull
            },
            std::array<Bitboard, 4>{
                0x3C2ull,
                0xFD2ull,
                0x4BF490000ull,
                0x4BF490C10ull
            },
            std::array<Bitboard, 4>{
                0x7C2ull,
                0x4BF490410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFFC30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1104ull,
                0x30CFFD30Cull,
                0x30CFFC208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFC5104ull,
                0x30CFFD30Cull,
                0x3C208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xFD2ull,
                0x4BF480000ull,
                0x30CFED30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFCD30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104105104ull,
                0x208FBC208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF410FD2ull,
                0x104145104ull,
                0x208F3C208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x5104ull,
                0x1041C5104ull,
                0x208E3C208ull,
                0x30CFFC208ull
            },
            std::array<Bitboard, 4>{
                0x1041C5104ull,
                0x30CFC5104ull,
                0x3C208ull,
                0x208E3C208ull
            },
            std::array<Bitboard, 4>{
                0x10FD2ull,
                0x4BF080000ull,
                0x1043C5104ull,
                0x208A3C208ull
            },
            std::array<Bitboard, 4>{
                0x1047C5104ull,
                0x20823C208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43E410FD2ull,
                0x82080000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43C410FD2ull,
                0x4BF410FD2ull,
                0x83080000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x438410FD2ull,
                0x83080000ull,
                0x105104ull,
                0x308FFC208ull
            },
            std::array<Bitboard, 4>{
                0x430410FD2ull,
                0x87080000ull,
                0x304FC5104ull,
                0x23C208ull
            },
            std::array<Bitboard, 4>{
                0x410FD2ull,
                0x430410FD2ull,
                0x8F080000ull,
                0x4BF080000ull
            },
            std::array<Bitboard, 4>{
                0x410410FD2ull,
                0x9F080000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43F410FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4105104ull,
                0x208FFC208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104FC5104ull,
                0x823C208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410FD2ull,
                0xBF080000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFFB208ull,
                0x3000ull,
                0x30CFF8308ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3B104ull,
                0x30CFC8200ull,
                0x30CFCB304ull,
                0x38000ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFFB20Cull,
                0x3004ull,
                0x30CFF8208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3B10Cull,
                0x30CFC8000ull,
                0x30CFCB104ull,
                0x38008ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104ull,
                0x30CFFA208ull,
                0x30CFF830Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1104ull,
                0x30CFF8208ull,
                0x30CFF930Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3104ull,
                0x3B30Cull,
                0x30CFC0000ull,
                0x30CFF8208ull
            },
            std::array<Bitboard, 4>{
                0xB104ull,
                0x30CFE8208ull,
                0x30CFEB30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1B104ull,
                0x30CFC8208ull,
                0x30CFCB30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208FBB30Cull,
                0x104100000ull,
                0x104103104ull,
                0x208FB8208ull
            },
            std::array<Bitboard, 4>{
                0x208F3B30Cull,
                0x104140000ull,
                0x104143104ull,
                0x208F38208ull
            },
            std::array<Bitboard, 4>{
                0x208E3B30Cull,
                0x30CFFB30Cull,
                0x1041C0000ull,
                0x3104ull
            },
            std::array<Bitboard, 4>{
                0x3B30Cull,
                0x208E3B30Cull,
                0x1041C0000ull,
                0x30CFC0000ull
            },
            std::array<Bitboard, 4>{
                0x208A3B30Cull,
                0x1043C0000ull,
                0x1043C3104ull,
                0x208A38208ull
            },
            std::array<Bitboard, 4>{
                0x20823B30Cull,
                0x1047C0000ull,
                0x1047C3104ull,
                0x208238208ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x308FFB30Cull,
                0x103104ull,
                0x308FF8208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x23B30Cull,
                0x304FC0000ull,
                0x304FC3104ull,
                0x238208ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208FFB30Cull,
                0x4103104ull,
                0x208FF8208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x823B30Cull,
                0x104FC0000ull,
                0x104FC3104ull,
                0x8238208ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x37208ull,
                0x30CFC4100ull,
                0x7000ull,
                0x30CFF4308ull
            },
            std::array<Bitboard, 4>{
                0x30CFF7104ull,
                0x30CFC7304ull,
                0x30000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3720Cull,
                0x30CFC4000ull,
                0x7004ull,
                0x30CFF4208ull
            },
            std::array<Bitboard, 4>{
                0x30CFF710Cull,
                0x30CFC7104ull,
                0x30008ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x36208ull,
                0x30CFC4104ull,
                0x30CFF430Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x34208ull,
                0x30CFC5104ull,
                0x30CFF530Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30208ull,
                0x3730Cull,
                0x30CFC0000ull,
                0x30CFC7104ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20208ull,
                0x30CFC7104ull,
                0x30CFE730Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208ull,
                0x30CFD7104ull,
                0x30CFC730Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10413730Cull,
                0x208F80000ull,
                0x104107104ull,
                0x208FB0208ull
            },
            std::array<Bitboard, 4>{
                0x10417730Cull,
                0x208F00000ull,
                0x104147104ull,
                0x208F30208ull
            },
            std::array<Bitboard, 4>{
                0x3730Cull,
                0x1041F730Cull,
                0x208E00000ull,
                0x30CFC0000ull
            },
            std::array<Bitboard, 4>{
                0x1041F730Cull,
                0x30CFF730Cull,
                0x208E00000ull,
                0x1041C7104ull
            },
            std::array<Bitboard, 4>{
                0x1043F730Cull,
                0x208A00000ull,
                0x1043C7104ull,
                0x208A30208ull
            },
            std::array<Bitboard, 4>{
                0x1047F730Cull,
                0x208200000ull,
                0x1047C7104ull,
                0x208230208ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x13730Cull,
                0x308FC0000ull,
                0x107104ull,
                0x308FF0208ull
            },
            std::array<Bitboard, 4>{
                0x304FF730Cull,
                0x304FC7104ull,
                0x230208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x413730Cull,
                0x208FC0000ull,
                0x4107104ull,
                0x208FF0208ull
            },
            std::array<Bitboard, 4>{
                0x104FF730Cull,
                0x104FC7104ull,
                0x8230208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xFD0ull,
                0x4BF482080ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xF000ull,
                0x30CFEC308ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFCF304ull,
                0x20000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF482FC2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xF90ull,
                0x4BF482082ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xF10ull,
                0xFD2ull,
                0x4BF482000ull,
                0x4BF4820C2ull
            },
            std::array<Bitboard, 4>{
                0xE10ull,
                0x4BF4820C2ull,
                0xF004ull,
                0x30CFEC208ull
            },
            std::array<Bitboard, 4>{
                0xC10ull,
                0x4BF4821C2ull,
                0x30CFCF104ull,
                0x20008ull
            },
            std::array<Bitboard, 4>{
                0xC10ull,
                0x4BF4823C2ull,
                0x4BF482FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x410ull,
                0x4BF4827C2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFEC30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xFD2ull,
                0x4BF480000ull,
                0x30CFED30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xF104ull,
                0x30CFE8208ull,
                0x30CFEF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFCF104ull,
                0x20208ull,
                0x30CFEF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFCF30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410F104ull,
                0x208FA8208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2FD2ull,
                0x4BF400000ull,
                0x10414F104ull,
                0x208F28208ull
            },
            std::array<Bitboard, 4>{
                0xF104ull,
                0x1041CF104ull,
                0x208E28208ull,
                0x30CFE8208ull
            },
            std::array<Bitboard, 4>{
                0x1041CF104ull,
                0x30CFCF104ull,
                0x28208ull,
                0x208E28208ull
            },
            std::array<Bitboard, 4>{
                0x4BF082FD2ull,
                0x1043CF104ull,
                0x208A28208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047CF104ull,
                0x208228208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82082FD2ull,
                0x43E400000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82FD2ull,
                0x83082FD2ull,
                0x43C400000ull,
                0x4BF400000ull
            },
            std::array<Bitboard, 4>{
                0x83082FD2ull,
                0x438400000ull,
                0x10F104ull,
                0x308FE8208ull
            },
            std::array<Bitboard, 4>{
                0x87082FD2ull,
                0x430400000ull,
                0x304FCF104ull,
                0x228208ull
            },
            std::array<Bitboard, 4>{
                0x8F082FD2ull,
                0x4BF082FD2ull,
                0x430400000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F082FD2ull,
                0x410400000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2082FD2ull,
                0x43F400000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x410F104ull,
                0x208FE8208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104FCF104ull,
                0x8228208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBF082FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1F000ull,
                0x30CFCC308ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFDF304ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1F004ull,
                0x30CFCC208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFDF104ull,
                0x8ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFCC30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFCD30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1F104ull,
                0x30CFC8208ull,
                0x30CFCF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFDF104ull,
                0x208ull,
                0x30CFCF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CFCF30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10411F104ull,
                0x208F88208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10415F104ull,
                0x208F08208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1F104ull,
                0x1041DF104ull,
                0x208E08208ull,
                0x30CFC8208ull
            },
            std::array<Bitboard, 4>{
                0x1041DF104ull,
                0x30CFDF104ull,
                0x8208ull,
                0x208E08208ull
            },
            std::array<Bitboard, 4>{
                0x1043DF104ull,
                0x208A08208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047DF104ull,
                0x208208208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x11F104ull,
                0x308FC8208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x304FDF104ull,
                0x208208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x411F104ull,
                0x208FC8208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104FDF104ull,
                0x8208208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104104100ull,
                0x208FBF208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10413F104ull,
                0x208F88200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104104000ull,
                0x208FBF20Cull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10413F10Cull,
                0x208F88000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104104104ull,
                0x208FBE208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104105104ull,
                0x208FBC208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104100000ull,
                0x104107104ull,
                0x208FB8208ull,
                0x208FBF30Cull
            },
            std::array<Bitboard, 4>{
                0x104107104ull,
                0x10413F30Cull,
                0x208F80000ull,
                0x208FB8208ull
            },
            std::array<Bitboard, 4>{
                0x10410F104ull,
                0x208FA8208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10411F104ull,
                0x208F88208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CF3F30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104000000ull,
                0x30CF3F30Cull,
                0x208FBF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10413F30Cull,
                0x30CF3F30Cull,
                0x208F80000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CB3F30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30C33F30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x100000000ull,
                0x208FBF30Cull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10433F30Cull,
                0x200F80000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x20CFBF30Cull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10C33F30Cull,
                0xF80000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF410FD0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104144100ull,
                0x208F3F208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10417F104ull,
                0x208F08200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2FC2ull,
                0x4BF410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2082ull,
                0x4BF410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20C2ull,
                0x4BF410F10ull,
                0x4BF410FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20C2ull,
                0x4BF410E10ull,
                0x104144000ull,
                0x208F3F20Cull
            },
            std::array<Bitboard, 4>{
                0x21C2ull,
                0x4BF410C10ull,
                0x10417F10Cull,
                0x208F08000ull
            },
            std::array<Bitboard, 4>{
                0x23C2ull,
                0x2FD2ull,
                0x4BF410000ull,
                0x4BF410C10ull
            },
            std::array<Bitboard, 4>{
                0x27C2ull,
                0x4BF410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104144104ull,
                0x208F3E208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF410FD2ull,
                0x104145104ull,
                0x208F3C208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104140000ull,
                0x104147104ull,
                0x208F38208ull,
                0x208F3F30Cull
            },
            std::array<Bitboard, 4>{
                0x104147104ull,
                0x10417F30Cull,
                0x208F00000ull,
                0x208F38208ull
            },
            std::array<Bitboard, 4>{
                0x2FD2ull,
                0x4BF400000ull,
                0x10414F104ull,
                0x208F28208ull
            },
            std::array<Bitboard, 4>{
                0x10415F104ull,
                0x208F08208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CF3F30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104040000ull,
                0x30CF7F30Cull,
                0x208F3F30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10417F30Cull,
                0x30CF7F30Cull,
                0x208F00000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x12FD2ull,
                0x4BF000000ull,
                0x30CB7F30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30C37F30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43E412FD2ull,
                0x82000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43C412FD2ull,
                0x4BF412FD2ull,
                0x83000000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x438412FD2ull,
                0x83000000ull,
                0x100040000ull,
                0x208F3F30Cull
            },
            std::array<Bitboard, 4>{
                0x430412FD2ull,
                0x87000000ull,
                0x10437F30Cull,
                0x200F00000ull
            },
            std::array<Bitboard, 4>{
                0x412FD2ull,
                0x430412FD2ull,
                0x8F000000ull,
                0x4BF000000ull
            },
            std::array<Bitboard, 4>{
                0x410412FD2ull,
                0x9F000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43F412FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x40000ull,
                0x20CF3F30Cull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10C37F30Cull,
                0xF00000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10412FD2ull,
                0xBF000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CEFF208ull,
                0x1040C4100ull,
                0x208E3F208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F104ull,
                0x30CEC8200ull,
                0x1040FF104ull,
                0x208E08200ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CEFF20Cull,
                0x1040C4000ull,
                0x208E3F20Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F10Cull,
                0x30CEC8000ull,
                0x1040FF10Cull,
                0x208E08000ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4104ull,
                0x30CEFE208ull,
                0x1040C4104ull,
                0x208E3E208ull
            },
            std::array<Bitboard, 4>{
                0x5104ull,
                0x30CEFC208ull,
                0x1040C5104ull,
                0x208E3C208ull
            },
            std::array<Bitboard, 4>{
                0x7104ull,
                0x30CEF8208ull,
                0x30CEFF30Cull,
                0x1040C0000ull
            },
            std::array<Bitboard, 4>{
                0x7104ull,
                0x3F30Cull,
                0x30CEC0000ull,
                0x30CEF8208ull
            },
            std::array<Bitboard, 4>{
                0xF104ull,
                0x30CEE8208ull,
                0x1040CF104ull,
                0x208E28208ull
            },
            std::array<Bitboard, 4>{
                0x1F104ull,
                0x30CEC8208ull,
                0x1040DF104ull,
                0x208E08208ull
            },
            std::array<Bitboard, 4>{
                0x208EBF30Cull,
                0x104000000ull,
                0x30CE3F30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208E3F30Cull,
                0x104040000ull,
                0x30CE7F30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F30Cull,
                0x208E3F30Cull,
                0x1040C0000ull,
                0x30CEC0000ull
            },
            std::array<Bitboard, 4>{
                0x208A3F30Cull,
                0x1042C0000ull,
                0x30CAFF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20823F30Cull,
                0x1046C0000ull,
                0x30C2FF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x308EFF30Cull,
                0x1000C0000ull,
                0x208E3F30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x23F30Cull,
                0x304EC0000ull,
                0x1042FF30Cull,
                0x200E00000ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208EFF30Cull,
                0xC0000ull,
                0x20CE3F30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x823F30Cull,
                0x104EC0000ull,
                0x10C2FF30Cull,
                0xE00000ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F208ull,
                0x30CDC4100ull,
                0x1041C4100ull,
                0x208C3F208ull
            },
            std::array<Bitboard, 4>{
                0x30CDFF104ull,
                0x1041FF104ull,
                0x208C08200ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F20Cull,
                0x30CDC4000ull,
                0x1041C4000ull,
                0x208C3F20Cull
            },
            std::array<Bitboard, 4>{
                0x30CDFF10Cull,
                0x1041FF10Cull,
                0x208C08000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3E208ull,
                0x30CDC4104ull,
                0x1041C4104ull,
                0x208C3E208ull
            },
            std::array<Bitboard, 4>{
                0x3C208ull,
                0x30CDC5104ull,
                0x1041C5104ull,
                0x208C3C208ull
            },
            std::array<Bitboard, 4>{
                0x38208ull,
                0x3F30Cull,
                0x30CDC0000ull,
                0x30CDC7104ull
            },
            std::array<Bitboard, 4>{
                0x38208ull,
                0x30CDC7104ull,
                0x30CDFF30Cull,
                0x1041C7104ull
            },
            std::array<Bitboard, 4>{
                0x28208ull,
                0x30CDCF104ull,
                0x1041CF104ull,
                0x208C28208ull
            },
            std::array<Bitboard, 4>{
                0x8208ull,
                0x30CDDF104ull,
                0x1041DF104ull,
                0x208C08208ull
            },
            std::array<Bitboard, 4>{
                0x10413F30Cull,
                0x208D80000ull,
                0x30CD3F30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10417F30Cull,
                0x208D00000ull,
                0x30CD7F30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3F30Cull,
                0x1041FF30Cull,
                0x208C00000ull,
                0x30CDC0000ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1041FF30Cull,
                0x208800000ull,
                0x30C9FF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1045FF30Cull,
                0x208000000ull,
                0x30C1FF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x13F30Cull,
                0x308DC0000ull,
                0x1001C0000ull,
                0x208D3F30Cull
            },
            std::array<Bitboard, 4>{
                0x304DFF30Cull,
                0x1041FF30Cull,
                0x200C00000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x413F30Cull,
                0x208DC0000ull,
                0x1C0000ull,
                0x20CD3F30Cull
            },
            std::array<Bitboard, 4>{
                0x104DFF30Cull,
                0x10C1FF30Cull,
                0xC00000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10FD0ull,
                0x4BF082080ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1043C4100ull,
                0x208A3F208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1043FF104ull,
                0x208A08200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BF082FC2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10F90ull,
                0x4BF082082ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10F10ull,
                0x10FD2ull,
                0x4BF082000ull,
                0x4BF0820C2ull
            },
            std::array<Bitboard, 4>{
                0x10E10ull,
                0x4BF0820C2ull,
                0x1043C4000ull,
                0x208A3F20Cull
            },
            std::array<Bitboard, 4>{
                0x10C10ull,
                0x4BF0821C2ull,
                0x1043FF10Cull,
                0x208A08000ull
            },
            std::array<Bitboard, 4>{
                0x10C10ull,
                0x4BF0823C2ull,
                0x4BF082FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410ull,
                0x4BF0827C2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1043C4104ull,
                0x208A3E208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10FD2ull,
                0x4BF080000ull,
                0x1043C5104ull,
                0x208A3C208ull
            },
            std::array<Bitboard, 4>{
                0x1043C0000ull,
                0x1043C7104ull,
                0x208A38208ull,
                0x208A3F30Cull
            },
            std::array<Bitboard, 4>{
                0x1043C7104ull,
                0x1043FF30Cull,
                0x208A00000ull,
                0x208A38208ull
            },
            std::array<Bitboard, 4>{
                0x4BF082FD2ull,
                0x1043CF104ull,
                0x208A28208ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1043DF104ull,
                0x208A08208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30CB3F30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x12FD2ull,
                0x4BF000000ull,
                0x30CB7F30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1043C0000ull,
                0x208A3F30Cull,
                0x30CBFF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1043FF30Cull,
                0x208800000ull,
                0x30CBFF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30C3FF30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82092FD2ull,
                0x43E000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x92FD2ull,
                0x83092FD2ull,
                0x43C000000ull,
                0x4BF000000ull
            },
            std::array<Bitboard, 4>{
                0x83092FD2ull,
                0x438000000ull,
                0x1003C0000ull,
                0x208B3F30Cull
            },
            std::array<Bitboard, 4>{
                0x87092FD2ull,
                0x430000000ull,
                0x1043FF30Cull,
                0x200800000ull
            },
            std::array<Bitboard, 4>{
                0x8F092FD2ull,
                0x4BF092FD2ull,
                0x430000000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F092FD2ull,
                0x410000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2092FD2ull,
                0x43F000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3C0000ull,
                0x20CB3F30Cull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10C3FF30Cull,
                0x800000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBF092FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047C4100ull,
                0x20823F208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047FF104ull,
                0x208208200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047C4000ull,
                0x20823F20Cull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047FF10Cull,
                0x208208000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047C4104ull,
                0x20823E208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047C5104ull,
                0x20823C208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047C0000ull,
                0x1047C7104ull,
                0x208238208ull,
                0x20823F30Cull
            },
            std::array<Bitboard, 4>{
                0x1047C7104ull,
                0x1047FF30Cull,
                0x208200000ull,
                0x208238208ull
            },
            std::array<Bitboard, 4>{
                0x1047CF104ull,
                0x208228208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047DF104ull,
                0x208208208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30C33F30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30C37F30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047C0000ull,
                0x20823F30Cull,
                0x30C3FF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047FF30Cull,
                0x208000000ull,
                0x30C3FF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x30C3FF30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1007C0000ull,
                0x20833F30Cull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1047FF30Cull,
                0x200000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x7C0000ull,
                0x20C33F30Cull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10C7FF30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82082080ull,
                0x43E410FD0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82082FC2ull,
                0x43E410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82082082ull,
                0x43E410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82082000ull,
                0x820820C2ull,
                0x43E410F10ull,
                0x43E410FD2ull
            },
            std::array<Bitboard, 4>{
                0x820820C2ull,
                0x43E410E10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x820821C2ull,
                0x43E410C10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x820823C2ull,
                0x82082FD2ull,
                0x43E410000ull,
                0x43E410C10ull
            },
            std::array<Bitboard, 4>{
                0x820827C2ull,
                0x43E410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82080000ull,
                0x43E410FD2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82082FD2ull,
                0x43E400000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82000000ull,
                0x43E412FD2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82092FD2ull,
                0x43E000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x80000000ull,
                0x4BE492FD2ull,
                0x43E492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BA492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4B2492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82492FD2ull,
                0x4B2492FD2ull,
                0x43E000000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x492492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x43E492FD2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x92492FD2ull,
                0x3E000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BD410FD0ull,
                0x81082080ull,
                0x43C410FD0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82FC2ull,
                0x4BD410400ull,
                0x81082FC2ull,
                0x43C410400ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82082ull,
                0x4BD410F90ull,
                0x81082082ull,
                0x43C410F90ull
            },
            std::array<Bitboard, 4>{
                0x820C2ull,
                0x4BD410F10ull,
                0x4BD410FD2ull,
                0x81082000ull
            },
            std::array<Bitboard, 4>{
                0x820C2ull,
                0x4BD410E10ull,
                0x810820C2ull,
                0x43C410E10ull
            },
            std::array<Bitboard, 4>{
                0x821C2ull,
                0x4BD410C10ull,
                0x810821C2ull,
                0x43C410C10ull
            },
            std::array<Bitboard, 4>{
                0x823C2ull,
                0x82FD2ull,
                0x4BD410000ull,
                0x4BD410C10ull
            },
            std::array<Bitboard, 4>{
                0x827C2ull,
                0x4BD410410ull,
                0x810827C2ull,
                0x43C410410ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BD410FD2ull,
                0x81080000ull,
                0x43C410FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82FD2ull,
                0x4BD400000ull,
                0x81082FD2ull,
                0x43C400000ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BD412FD2ull,
                0x81000000ull,
                0x43C412FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x92FD2ull,
                0x4BD000000ull,
                0x81092FD2ull,
                0x43C000000ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43C492FD2ull,
                0x80000000ull,
                0x4BC492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x438492FD2ull,
                0x81000000ull,
                0x4B9492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x430492FD2ull,
                0x85000000ull,
                0x4B1492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x492FD2ull,
                0x430492FD2ull,
                0x8D000000ull,
                0x4BD000000ull
            },
            std::array<Bitboard, 4>{
                0x410492FD2ull,
                0x9D000000ull,
                0x491492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43D492FD2ull,
                0x1000000ull,
                0x43C492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10492FD2ull,
                0xBD000000ull,
                0x91492FD2ull,
                0x3C000000ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x83082080ull,
                0x438410FD0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x308FFF208ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x13F104ull,
                0x308FC8200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x83082FC2ull,
                0x438410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x83082082ull,
                0x438410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x83082000ull,
                0x830820C2ull,
                0x438410F10ull,
                0x438410FD2ull
            },
            std::array<Bitboard, 4>{
                0x308FFF20Cull,
                0x830820C2ull,
                0x438410E10ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x13F10Cull,
                0x308FC8000ull,
                0x830821C2ull,
                0x438410C10ull
            },
            std::array<Bitboard, 4>{
                0x830823C2ull,
                0x83082FD2ull,
                0x438410000ull,
                0x438410C10ull
            },
            std::array<Bitboard, 4>{
                0x830827C2ull,
                0x438410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104104ull,
                0x308FFE208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x105104ull,
                0x308FFC208ull,
                0x83080000ull,
                0x438410FD2ull
            },
            std::array<Bitboard, 4>{
                0x107104ull,
                0x308FF8208ull,
                0x308FFF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x107104ull,
                0x13F30Cull,
                0x308FC0000ull,
                0x308FF8208ull
            },
            std::array<Bitboard, 4>{
                0x10F104ull,
                0x308FE8208ull,
                0x83082FD2ull,
                0x438400000ull
            },
            std::array<Bitboard, 4>{
                0x11F104ull,
                0x308FC8208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208FBF30Cull,
                0x100000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208F3F30Cull,
                0x100040000ull,
                0x83000000ull,
                0x438412FD2ull
            },
            std::array<Bitboard, 4>{
                0x208F3F30Cull,
                0x1000C0000ull,
                0x308FFF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x13F30Cull,
                0x208F3F30Cull,
                0x1001C0000ull,
                0x308FC0000ull
            },
            std::array<Bitboard, 4>{
                0x208B3F30Cull,
                0x1003C0000ull,
                0x83092FD2ull,
                0x438000000ull
            },
            std::array<Bitboard, 4>{
                0x20833F30Cull,
                0x1007C0000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4BA492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x83000000ull,
                0x438492FD2ull,
                0x4BB492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x33F30Cull,
                0x300FC0000ull,
                0x4B3492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x83492FD2ull,
                0x4B3492FD2ull,
                0x438000000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x493492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x3000000ull,
                0x43A492FD2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208FFF30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x833F30Cull,
                0x100FC0000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x93492FD2ull,
                0x38000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x87082080ull,
                0x430410FD0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x23F208ull,
                0x304FC4100ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x304FFF104ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x87082FC2ull,
                0x430410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x87082082ull,
                0x430410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x87082000ull,
                0x870820C2ull,
                0x430410F10ull,
                0x430410FD2ull
            },
            std::array<Bitboard, 4>{
                0x23F20Cull,
                0x304FC4000ull,
                0x870820C2ull,
                0x430410E10ull
            },
            std::array<Bitboard, 4>{
                0x304FFF10Cull,
                0x870821C2ull,
                0x430410C10ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x870823C2ull,
                0x87082FD2ull,
                0x430410000ull,
                0x430410C10ull
            },
            std::array<Bitboard, 4>{
                0x870827C2ull,
                0x430410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x23E208ull,
                0x304FC4104ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x23C208ull,
                0x304FC5104ull,
                0x87080000ull,
                0x430410FD2ull
            },
            std::array<Bitboard, 4>{
                0x238208ull,
                0x23F30Cull,
                0x304FC0000ull,
                0x304FC7104ull
            },
            std::array<Bitboard, 4>{
                0x238208ull,
                0x304FC7104ull,
                0x304FFF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x228208ull,
                0x304FCF104ull,
                0x87082FD2ull,
                0x430400000ull
            },
            std::array<Bitboard, 4>{
                0x208208ull,
                0x304FDF104ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10433F30Cull,
                0x200F80000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10437F30Cull,
                0x200F00000ull,
                0x87000000ull,
                0x430412FD2ull
            },
            std::array<Bitboard, 4>{
                0x23F30Cull,
                0x1043FF30Cull,
                0x200E00000ull,
                0x304FC0000ull
            },
            std::array<Bitboard, 4>{
                0x1043FF30Cull,
                0x200C00000ull,
                0x304FFF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1043FF30Cull,
                0x200800000ull,
                0x87092FD2ull,
                0x430000000ull
            },
            std::array<Bitboard, 4>{
                0x1047FF30Cull,
                0x200000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4B2492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x87000000ull,
                0x430492FD2ull,
                0x4B3492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x33F30Cull,
                0x300FC0000ull,
                0x4B3492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x87492FD2ull,
                0x4B7492FD2ull,
                0x430000000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x497492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x7000000ull,
                0x432492FD2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x433F30Cull,
                0x200FC0000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104FFF30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x97492FD2ull,
                0x30000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x410FD0ull,
                0x4AF082080ull,
                0x8F082080ull,
                0x420410FD0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4AF082FC2ull,
                0x8F082FC2ull,
                0x420410400ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x410F90ull,
                0x4AF082082ull,
                0x8F082082ull,
                0x420410F90ull
            },
            std::array<Bitboard, 4>{
                0x410F10ull,
                0x410FD2ull,
                0x4AF082000ull,
                0x4AF0820C2ull
            },
            std::array<Bitboard, 4>{
                0x410E10ull,
                0x4AF0820C2ull,
                0x8F0820C2ull,
                0x420410E10ull
            },
            std::array<Bitboard, 4>{
                0x410C10ull,
                0x4AF0821C2ull,
                0x8F0821C2ull,
                0x420410C10ull
            },
            std::array<Bitboard, 4>{
                0x410C10ull,
                0x4AF0823C2ull,
                0x4AF082FD2ull,
                0x8F0823C2ull
            },
            std::array<Bitboard, 4>{
                0x410410ull,
                0x4AF0827C2ull,
                0x8F0827C2ull,
                0x420410410ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x410FD2ull,
                0x4AF080000ull,
                0x8F080000ull,
                0x420410FD2ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4AF082FD2ull,
                0x8F082FD2ull,
                0x420400000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x412FD2ull,
                0x4AF000000ull,
                0x8F000000ull,
                0x420412FD2ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4AF092FD2ull,
                0x8F092FD2ull,
                0x420000000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x82492FD2ull,
                0x42E000000ull,
                0x4A2492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x492FD2ull,
                0x83492FD2ull,
                0x42C000000ull,
                0x4AF000000ull
            },
            std::array<Bitboard, 4>{
                0x83492FD2ull,
                0x428000000ull,
                0x4A3492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x87492FD2ull,
                0x420000000ull,
                0x4A7492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x8F492FD2ull,
                0x400000000ull,
                0x48F492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2492FD2ull,
                0x42F000000ull,
                0xF000000ull,
                0x422492FD2ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xAF492FD2ull,
                0x8F492FD2ull,
                0x20000000ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F082080ull,
                0x410410FD0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F082FC2ull,
                0x410410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F082082ull,
                0x410410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F082000ull,
                0x9F0820C2ull,
                0x410410F10ull,
                0x410410FD2ull
            },
            std::array<Bitboard, 4>{
                0x9F0820C2ull,
                0x410410E10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F0821C2ull,
                0x410410C10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F0823C2ull,
                0x9F082FD2ull,
                0x410410000ull,
                0x410410C10ull
            },
            std::array<Bitboard, 4>{
                0x9F0827C2ull,
                0x410410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F080000ull,
                0x410410FD2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F082FD2ull,
                0x410400000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F000000ull,
                0x410412FD2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F092FD2ull,
                0x410000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x492492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F000000ull,
                0x410492FD2ull,
                0x493492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x493492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x497492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F492FD2ull,
                0x400000000ull,
                0x49F492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x1F000000ull,
                0x412492FD2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43F410FD0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2082FC2ull,
                0x43F410400ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2082082ull,
                0x43F410F90ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20820C2ull,
                0x43F410F10ull,
                0x43F410FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20820C2ull,
                0x43F410E10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20821C2ull,
                0x43F410C10ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20823C2ull,
                0x2082FD2ull,
                0x43F410000ull,
                0x43F410C10ull
            },
            std::array<Bitboard, 4>{
                0x20827C2ull,
                0x43F410410ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43F410FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2082FD2ull,
                0x43F400000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43F412FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2092FD2ull,
                0x43F000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43E492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43E492FD2ull,
                0x1000000ull,
                0x43F492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x43A492FD2ull,
                0x3000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x432492FD2ull,
                0x7000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x2492FD2ull,
                0x432492FD2ull,
                0xF000000ull,
                0x43F000000ull
            },
            std::array<Bitboard, 4>{
                0x412492FD2ull,
                0x1F000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x12492FD2ull,
                0x3F000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208FFF208ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x413F104ull,
                0x208FC8200ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208FFF20Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x413F10Cull,
                0x208FC8000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4104104ull,
                0x208FFE208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4105104ull,
                0x208FFC208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4107104ull,
                0x208FF8208ull,
                0x208FFF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x4107104ull,
                0x413F30Cull,
                0x208FC0000ull,
                0x208FF8208ull
            },
            std::array<Bitboard, 4>{
                0x410F104ull,
                0x208FE8208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x411F104ull,
                0x208FC8208ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20CFBF30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20CF3F30Cull,
                0x40000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20CF3F30Cull,
                0xC0000ull,
                0x208FFF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x413F30Cull,
                0x20CF3F30Cull,
                0x1C0000ull,
                0x208FC0000ull
            },
            std::array<Bitboard, 4>{
                0x20CB3F30Cull,
                0x3C0000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x20C33F30Cull,
                0x7C0000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x208FFF30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x433F30Cull,
                0x200FC0000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xC33F30Cull,
                0xFC0000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x823F208ull,
                0x104FC4100ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104FFF104ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x823F20Cull,
                0x104FC4000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104FFF10Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x823E208ull,
                0x104FC4104ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x823C208ull,
                0x104FC5104ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x8238208ull,
                0x823F30Cull,
                0x104FC0000ull,
                0x104FC7104ull
            },
            std::array<Bitboard, 4>{
                0x8238208ull,
                0x104FC7104ull,
                0x104FFF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x8228208ull,
                0x104FCF104ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x8208208ull,
                0x104FDF104ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10C33F30Cull,
                0xF80000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10C37F30Cull,
                0xF00000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x823F30Cull,
                0x10C3FF30Cull,
                0xE00000ull,
                0x104FC0000ull
            },
            std::array<Bitboard, 4>{
                0x10C3FF30Cull,
                0xC00000ull,
                0x104FFF30Cull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10C3FF30Cull,
                0x800000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10C7FF30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x833F30Cull,
                0x100FC0000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x104FFF30Cull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xC33F30Cull,
                0xFC0000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410FD0ull,
                0xBF082080ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBF082FC2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410F90ull,
                0xBF082082ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410F10ull,
                0x10410FD2ull,
                0xBF082000ull,
                0xBF0820C2ull
            },
            std::array<Bitboard, 4>{
                0x10410E10ull,
                0xBF0820C2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410C10ull,
                0xBF0821C2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410C10ull,
                0xBF0823C2ull,
                0xBF082FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410410ull,
                0xBF0827C2ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10410FD2ull,
                0xBF080000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBF082FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10412FD2ull,
                0xBF000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0xBF092FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x92492FD2ull,
                0x3E000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x10492FD2ull,
                0x93492FD2ull,
                0x3C000000ull,
                0xBF000000ull
            },
            std::array<Bitboard, 4>{
                0x93492FD2ull,
                0x38000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x97492FD2ull,
                0x30000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F492FD2ull,
                0x20000000ull,
                0xBF492FD2ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x9F492FD2ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x12492FD2ull,
                0x3F000000ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        },
        std::array<std::array<Bitboard, 4>, 36>{
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            },
            std::array<Bitboard, 4>{
                0x0ull,
                0x0ull,
                0x0ull,
                0x0ull
            }
        }
    };
    tables.capture_start_dir = std::array<std::array<std::array<std::uint8_t, 4>, 36>, 36>{
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                2
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                2
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                2
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                2
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                2
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                2
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                2
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                2
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                2,
                3
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                2,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                2,
                3,
                3
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                3,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                3,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                2,
                3,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                1,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                1,
                1
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                1,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                1,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        },
        std::array<std::array<std::uint8_t, 4>, 36>{
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            },
            std::array<std::uint8_t, 4>{
                0,
                0,
                0,
                0
            }
        }
    };
    return tables;
}();

}  // namespace surakarta::bitboard
