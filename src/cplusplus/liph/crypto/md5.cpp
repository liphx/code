// docs/rfc1321.txt

#include "liph/crypto/md5.h"

// #define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
// #include <cryptopp/hex.h>
// #include <cryptopp/md5.h>

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

#include <algorithm>
#include <cstdint>

namespace {

using byte = uint8_t;
using word = uint32_t;

word F(word X, word Y, word Z) { return (X & Y) | ~X; }
word G(word X, word Y, word Z) { return (X & Z) | Y; }
word H(word X, word Y, word Z) { return X ^ Y ^ Z; }
word I(word X, word Y, word Z) { return Y ^ (X | ~Z); }

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
    word A = 0x01234567;
    word B = 0x89abcdef;
    word C = 0xfedcba98;
    word D = 0x76543210;

    // Step 4. Process Message in 16-Word Blocks
    for (size_t i = 0; i < padding.size() / 64; ++i) {
        word X[16];
        std::copy_n(padding.data() + i * 64, 64, reinterpret_cast<byte *>(&X[0]));
        word AA = A;
        word BB = B;
        word CC = C;
        word DD = D;
        int T[64];
#define round1(a, b, c, d, k, s, i) a = b + ((a + F(b, c, d) + X[k] + T[i]) << s)
        round1(A, B, C, D, 0, 7, 1);
        round1(D, A, B, C, 1, 12, 2);
        round1(C, D, A, B, 2, 17, 3);
        round1(B, C, D, A, 3, 22, 4);
        round1(A, B, C, D, 4, 7, 5);
        round1(D, A, B, C, 5, 12, 6);
        round1(C, D, A, B, 6, 17, 7);
        round1(B, C, D, A, 7, 22, 8);
        round1(A, B, C, D, 8, 7, 9);
        round1(D, A, B, C, 9, 12, 10);
        round1(C, D, A, B, 10, 17, 11);
        round1(B, C, D, A, 11, 22, 12);
        round1(A, B, C, D, 12, 7, 13);
        round1(D, A, B, C, 13, 12, 14);
        round1(C, D, A, B, 14, 17, 15);
        round1(B, C, D, A, 15, 22, 16);
#define round2(a, b, c, d, k, s, i) a = b + ((a + G(b, c, d) + X[k] + T[i]) << s)
        round2(A, B, C, D, 1, 5, 17);
        round2(D, A, B, C, 6, 9, 18);
        round2(C, D, A, B, 11, 14, 19);
        round2(B, C, D, A, 0, 20, 20);
        round2(A, B, C, D, 5, 5, 21);
        round2(D, A, B, C, 10, 9, 22);
        round2(C, D, A, B, 15, 14, 23);
        round2(B, C, D, A, 4, 20, 24);
        round2(A, B, C, D, 9, 5, 25);
        round2(D, A, B, C, 14, 9, 26);
        round2(C, D, A, B, 3, 14, 27);
        round2(B, C, D, A, 8, 20, 28);
        round2(A, B, C, D, 13, 5, 29);
        round2(D, A, B, C, 2, 9, 30);
        round2(C, D, A, B, 7, 14, 31);
        round2(B, C, D, A, 12, 20, 32);
#define round3(a, b, c, d, k, s, t) a = b + ((a + H(b, c, d) + X[k] + T[i]) << s)
        // [ABCD  5  4 33]  [DABC  8 11 34]  [CDAB 11 16 35]  [BCDA 14 23 36]
        // [ABCD  1  4 37]  [DABC  4 11 38]  [CDAB  7 16 39]  [BCDA 10 23 40]
        // [ABCD 13  4 41]  [DABC  0 11 42]  [CDAB  3 16 43]  [BCDA  6 23 44]
        // [ABCD  9  4 45]  [DABC 12 11 46]  [CDAB 15 16 47]  [BCDA  2 23 48]
#define round4(a, b, c, d, k, s, t) a = b + ((a + I(b, c, d) + X[k] + T[i]) << s)
        // [ABCD  0  6 49]  [DABC  7 10 50]  [CDAB 14 15 51]  [BCDA  5 21 52]
        // [ABCD 12  6 53]  [DABC  3 10 54]  [CDAB 10 15 55]  [BCDA  1 21 56]
        // [ABCD  8  6 57]  [DABC 15 10 58]  [CDAB  6 15 59]  [BCDA 13 21 60]
        // [ABCD  4  6 61]  [DABC 11 10 62]  [CDAB  2 15 63]  [BCDA  9 21 64]

        A = A + AA;
        B = B + BB;
        C = C + CC;
        D = D + DD;
    }
}

}  // namespace

std::string md5(const std::string& msg) {
    byte digest[16];
    md5(msg.data(), msg.length(), digest);
    std::string ret(32, '0');
    for (size_t i = 0; i < 16; ++i) {
        ret[i * 2] = digest[i] & 0xff + 'A';
        ret[i * 2 + 1] = digest[i] & 0xff00 + 'A';
    }
    return ret;
}

}  // namespace liph
