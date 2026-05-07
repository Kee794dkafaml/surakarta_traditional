#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "surakarta_bitboard.h"

namespace {

using surakarta::bitboard::Bitboard;
using surakarta::bitboard::BitboardGenerator;

template <typename T>
struct TypeName;

template <>
struct TypeName<Bitboard> {
    static std::string Get() {
        return "Bitboard";
    }
};

template <>
struct TypeName<std::uint8_t> {
    static std::string Get() {
        return "std::uint8_t";
    }
};

template <typename T, std::size_t N>
struct TypeName<std::array<T, N>> {
    static std::string Get() {
        return "std::array<" + TypeName<T>::Get() + ", " + std::to_string(N) + ">";
    }
};

std::string Indent(int depth) {
    return std::string(static_cast<std::size_t>(depth) * 4, ' ');
}

template <typename T>
std::string FormatValue(const T& value) {
    if constexpr (std::is_same_v<T, Bitboard>) {
        auto stream = std::ostringstream{};
        stream << "0x" << std::hex << std::uppercase << value << "ull";
        return stream.str();
    } else if constexpr (std::is_integral_v<T>) {
        return std::to_string(static_cast<unsigned long long>(value));
    } else {
        static_assert(std::is_same_v<T, void>, "Unsupported array value type");
    }
}

template <typename T, std::size_t N>
void WriteArray(std::ostream& os, const std::array<T, N>& values, int depth) {
    os << TypeName<std::array<T, N>>::Get() << "{";

    if constexpr (std::is_integral_v<T> || std::is_same_v<T, Bitboard>) {
        if constexpr (N > 0) {
            os << "\n";
            for (std::size_t i = 0; i < N; ++i) {
                os << Indent(depth + 1) << FormatValue(values[i]);
                if (i + 1 != N) {
                    os << ",";
                }
                os << "\n";
            }
            os << Indent(depth);
        }
    } else {
        if constexpr (N > 0) {
            os << "\n";
            for (std::size_t i = 0; i < N; ++i) {
                os << Indent(depth + 1);
                WriteArray(os, values[i], depth + 1);
                if (i + 1 != N) {
                    os << ",";
                }
                os << "\n";
            }
            os << Indent(depth);
        }
    }
    os << "}";
}

void WriteHeader(std::ostream& os) {
    const auto tables = BitboardGenerator().Build();

    os << "#pragma once\n\n";
    os << "#include <array>\n";
    os << "#include <cstdint>\n\n";
    os << "#include \"surakarta_bitboard.h\"\n\n";
    os << "namespace surakarta::bitboard {\n\n";
    os << "inline constexpr BitboardTables kStaticBitboardTables = [] {\n";
    os << "    auto tables = BitboardTables{};\n";
    os << "    tables.quiet_move_mask = ";
    WriteArray(os, tables.quiet_move_mask, 1);
    os << ";\n";
    os << "    tables.capture_target_mask = ";
    WriteArray(os, tables.capture_target_mask, 1);
    os << ";\n";
    os << "    tables.capture_variant_count = ";
    WriteArray(os, tables.capture_variant_count, 1);
    os << ";\n";
    os << "    tables.capture_clear_mask = ";
    WriteArray(os, tables.capture_clear_mask, 1);
    os << ";\n";
    os << "    tables.capture_start_dir = ";
    WriteArray(os, tables.capture_start_dir, 1);
    os << ";\n";
    os << "    return tables;\n";
    os << "}();\n\n";
    os << "}  // namespace surakarta::bitboard\n";
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <output-header>" << std::endl;
        return 1;
    }

    auto output = std::ofstream(argv[1], std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        std::cerr << "Failed to open output file: " << argv[1] << std::endl;
        return 1;
    }

    try {
        WriteHeader(output);
    } catch (const std::exception& ex) {
        std::cerr << "Failed to generate bitboard masks: " << ex.what() << std::endl;
        return 1;
    }
    return output.good() ? 0 : 1;
}
