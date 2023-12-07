#include "liph/encoding/base64.h"

#include "gtest/gtest.h"

namespace liph {

TEST(base64, encode) {
    EXPECT_EQ(base64::encode(""), "");
    EXPECT_EQ(base64::encode("hello"), "aGVsbG8=");
    EXPECT_EQ(base64::encode("light w"), "bGlnaHQgdw==");
    EXPECT_EQ(base64::encode("light wo"), "bGlnaHQgd28=");
    EXPECT_EQ(base64::encode("light wor"), "bGlnaHQgd29y");
    EXPECT_EQ(base64::encode("liph"), "bGlwaA==");

    int32_t x = 42;
    std::string_view sv(reinterpret_cast<const char *>(&x), sizeof(x));
    EXPECT_EQ(base64::encode(sv), "KgAAAA==");
}

TEST(base64, decode) {
    EXPECT_EQ(base64::decode(""), "");
    EXPECT_EQ(base64::decode("---"), "");
    EXPECT_EQ(base64::decode("aGVsbG8="), "hello");
    EXPECT_EQ(base64::decode("bGlnaHQgdw=="), "light w");
    EXPECT_EQ(base64::decode("bGlnaHQgd28="), "light wo");
    EXPECT_EQ(base64::decode("bGlnaHQgd29y"), "light wor");
    EXPECT_EQ(base64::decode("bGlwaA=="), "liph");

    std::string s = base64::decode("KgAAAA==");
    EXPECT_EQ(*reinterpret_cast<const int32_t *>(s.data()), 42);
}

};  // namespace liph
