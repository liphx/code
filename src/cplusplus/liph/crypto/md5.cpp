// rfc1321.txt

#include "liph/crypto/md5.h"

// #define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
// #include <cryptopp/hex.h>
// #include <cryptopp/md5.h>

#include <algorithm>
#include <cstdint>
#include <vector>

namespace liph {

// std::string md5(const std::string& msg) {
//     using namespace CryptoPP;
//     Weak::MD5 hash;
//     std::string digest;
//     hash.Update((const uint8_t *)msg.data(), msg.size());
//     digest.resize(hash.DigestSize());
//     hash.Final((uint8_t *)&digest[0]);
//     std::string out;
//     StringSource ss(digest, true, new HexEncoder(new StringSink(out)));
//     return out;
// }

namespace {

using byte = uint8_t;
using word = uint32_t;

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

// input: message of arbitrary length
// output: 128-bit digest
void md5(const void *input, size_t length, byte digest[16]) {
    // assert(input);

    // Step 1. Append Padding Bits(Bytes)
    // 448 / 8 = 56, 512 / 8 = 64
    size_t mod = length % 64;
    size_t padding_size = mod < 56 ? 56 - mod : 64 - mod + 56;
    std::vector<byte> padding(length + padding_size + 8);
    std::copy(static_cast<const byte *>(input), static_cast<const byte *>(input) + length, padding.begin());
    padding[length] = 0x80;  // 1000 0000
    std::fill_n(padding.begin() + length + 1, padding_size - 1, 0);

    // Step 2. Append Length
    uint64_t input_size = length;
    std::copy(reinterpret_cast<byte *>(&input_size), reinterpret_cast<byte *>(&input_size) + 8,
            padding.begin() + padding_size);
    // assert(padding.size() % 64 == 0);

    // Step 3. Initialize MD Buffer
    word state[4] = {0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210};

    // Step 4. Process Message in 16-Word Blocks
    for (size_t i = 0; i < padding.size() / 64; ++i) {
        word A = state[0];
        word B = state[1];
        word C = state[2];
        word D = state[3];
        word X[16];
        std::copy_n(padding.data() + i * 64, 64, reinterpret_cast<byte *>(&X[0]));
        word T[65] = {0, 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
                0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
                0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
                0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
                0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
                0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
                0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
                0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define FF(a, b, c, d, x, s, i)                \
    {                                          \
        (a) += F((b), (c), (d)) + X[x] + T[i]; \
        (a) = ROTATE_LEFT((a), (s));           \
        (a) += (b);                            \
    }
#define GG(a, b, c, d, x, s, i)                \
    {                                          \
        (a) += G((b), (c), (d)) + X[x] + T[i]; \
        (a) = ROTATE_LEFT((a), (s));           \
        (a) += (b);                            \
    }
#define HH(a, b, c, d, x, s, i)                \
    {                                          \
        (a) += H((b), (c), (d)) + X[x] + T[i]; \
        (a) = ROTATE_LEFT((a), (s));           \
        (a) += (b);                            \
    }
#define II(a, b, c, d, x, s, i)                \
    {                                          \
        (a) += I((b), (c), (d)) + X[x] + T[i]; \
        (a) = ROTATE_LEFT((a), (s));           \
        (a) += (b);                            \
    }
        FF(A, B, C, D, 0, 7, 1);
        FF(D, A, B, C, 1, 12, 2);
        FF(C, D, A, B, 2, 17, 3);
        FF(B, C, D, A, 3, 22, 4);
        FF(A, B, C, D, 4, 7, 5);
        FF(D, A, B, C, 5, 12, 6);
        FF(C, D, A, B, 6, 17, 7);
        FF(B, C, D, A, 7, 22, 8);
        FF(A, B, C, D, 8, 7, 9);
        FF(D, A, B, C, 9, 12, 10);
        FF(C, D, A, B, 10, 17, 11);
        FF(B, C, D, A, 11, 22, 12);
        FF(A, B, C, D, 12, 7, 13);
        FF(D, A, B, C, 13, 12, 14);
        FF(C, D, A, B, 14, 17, 15);
        FF(B, C, D, A, 15, 22, 16);

        GG(A, B, C, D, 1, 5, 17);
        GG(D, A, B, C, 6, 9, 18);
        GG(C, D, A, B, 11, 14, 19);
        GG(B, C, D, A, 0, 20, 20);
        GG(A, B, C, D, 5, 5, 21);
        GG(D, A, B, C, 10, 9, 22);
        GG(C, D, A, B, 15, 14, 23);
        GG(B, C, D, A, 4, 20, 24);
        GG(A, B, C, D, 9, 5, 25);
        GG(D, A, B, C, 14, 9, 26);
        GG(C, D, A, B, 3, 14, 27);
        GG(B, C, D, A, 8, 20, 28);
        GG(A, B, C, D, 13, 5, 29);
        GG(D, A, B, C, 2, 9, 30);
        GG(C, D, A, B, 7, 14, 31);
        GG(B, C, D, A, 12, 20, 32);

        HH(A, B, C, D, 5, 4, 33);
        HH(D, A, B, C, 8, 11, 34);
        HH(C, D, A, B, 11, 16, 35);
        HH(B, C, D, A, 14, 23, 36);
        HH(A, B, C, D, 1, 4, 37);
        HH(D, A, B, C, 4, 11, 38);
        HH(C, D, A, B, 7, 16, 39);
        HH(B, C, D, A, 10, 23, 40);
        HH(A, B, C, D, 13, 4, 41);
        HH(D, A, B, C, 0, 11, 42);
        HH(C, D, A, B, 3, 16, 43);
        HH(B, C, D, A, 6, 23, 44);
        HH(A, B, C, D, 9, 4, 45);
        HH(D, A, B, C, 12, 11, 46);
        HH(C, D, A, B, 15, 16, 47);
        HH(B, C, D, A, 2, 23, 48);

        II(A, B, C, D, 0, 6, 49);
        II(D, A, B, C, 7, 10, 50);
        II(C, D, A, B, 14, 15, 51);
        II(B, C, D, A, 5, 21, 52);
        II(A, B, C, D, 12, 6, 53);
        II(D, A, B, C, 3, 10, 54);
        II(C, D, A, B, 10, 15, 55);
        II(B, C, D, A, 1, 21, 56);
        II(A, B, C, D, 8, 6, 57);
        II(D, A, B, C, 15, 10, 58);
        II(C, D, A, B, 6, 15, 59);
        II(B, C, D, A, 13, 21, 60);
        II(A, B, C, D, 4, 6, 61);
        II(D, A, B, C, 11, 10, 62);
        II(C, D, A, B, 2, 15, 63);
        II(B, C, D, A, 9, 21, 64);

        state[0] += A;
        state[1] += B;
        state[2] += C;
        state[3] += D;
    }
    *reinterpret_cast<uint32_t *>(&digest[0]) = state[0];
    *(reinterpret_cast<uint32_t *>(&digest[0]) + 1) = state[1];
    *(reinterpret_cast<uint32_t *>(&digest[0]) + 2) = state[2];
    *(reinterpret_cast<uint32_t *>(&digest[0]) + 1) = state[3];
}

inline char hex0(byte b, char ch) {
    byte c = b & 0xf;
    return c < 10 ? c + '0' : c + ch - 10;
}
inline char hex1(byte b, char ch) {
    byte c = b & 0xf0;
    c /= 0x10;
    return c < 10 ? c + '0' : c + ch - 10;
}

}  // namespace

std::string MD5(const std::string& msg) {
    byte digest[16];
    md5(msg.data(), msg.length(), digest);
    std::string ret(32, '0');
    for (size_t i = 0; i < 16; ++i) {
        ret[i * 2] = hex0(digest[i], 'A');
        ret[i * 2 + 1] = hex1(digest[i], 'A');
    }
    return ret;
}

std::string md5(const std::string& msg) {
    byte digest[16];
    md5(msg.data(), msg.length(), digest);
    std::string ret(32, '0');
    for (size_t i = 0; i < 16; ++i) {
        ret[i * 2] = (digest[i] & 0xf) + 'a';
        ret[i * 2 + 1] = (digest[i] & 0xf0) + 'a';
    }
    return ret;
}

}  // namespace liph
