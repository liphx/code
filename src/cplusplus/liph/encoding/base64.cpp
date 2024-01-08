#include "liph/encoding/base64.h"

#include <cstdint>

namespace liph {

namespace {

constexpr char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
constexpr char padding = '=';

uint8_t index(char ch) {
    if (ch >= 'A' && ch <= 'Z') return ch - 'A';
    if (ch >= 'a' && ch <= 'z') return ch - 'a' + 26;
    if (ch >= '0' && ch <= '9') return ch - '0' + 52;
    return ch == '+' ? 62 : ch == '/' ? 63 : -1;
}

constexpr inline size_t encode_len(size_t n) { return (n + 2) / 3 * 4; }

constexpr inline size_t decode_len(size_t n) { return n / 4 * 3; }

}  // namespace

std::string base64::encode(std::string_view sv) {
    std::string ret;
    ret.reserve(encode_len(sv.size()));
    const uint8_t *ptr = reinterpret_cast<const uint8_t *>(sv.data());
    for (size_t i = 0; i < sv.size(); i += 3) {
        uint32_t n = static_cast<uint32_t>(ptr[i]) << 16;
        if (i + 1 < sv.size()) n |= static_cast<uint32_t>(ptr[i + 1]) << 8;
        if (i + 2 < sv.size()) n |= static_cast<uint32_t>(ptr[i + 2]);
        size_t padding_size = (i + 2 < sv.size()) ? 0 : (i + 1 < sv.size()) ? 1 : 2;
        for (size_t j = 0; j < 4 - padding_size; ++j) ret += chars[(n >> (18 - 6 * j)) & 0x3f];
        ret.append(std::string(padding_size, padding));
    }
    return ret;
}

std::string base64::decode(std::string_view sv) {
    if (sv.size() % 4 != 0) return "";

    std::string ret;
    ret.reserve(decode_len(sv.size()));
    for (size_t i = 0, padding_size = 0; i < sv.size(); i += 4) {
        if (padding_size) return "";
        if (sv[i + 2] == padding && sv[i + 3] != padding) return "";
        if (sv[i + 2] == padding || sv[i + 3] == padding) padding_size = (sv[i + 2] == padding) ? 2 : 1;

        uint32_t n = 0;

        // clang-format off

        #define func(V) {           \
            uint8_t v = V;          \
            if (v == -1) return ""; \
            n = (n | v) << 6;       \
        }
        // clang-format on

        func(index(sv[i]));
        func(index(sv[i + 1]));
        func((sv[i + 2] == padding) ? 0 : index(sv[i + 2]));
        func((sv[i + 3] == padding) ? 0 : index(sv[i + 3]));

        for (size_t j = 0; j < 3 - padding_size; ++j) ret += static_cast<char>(n >> (22 - 8 * j));
    }
    return ret;
}

}  // namespace liph
