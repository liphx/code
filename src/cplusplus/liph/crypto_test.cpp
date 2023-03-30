#include "crypto.h"

#include <iostream>

#include "gtest/gtest.h"

using namespace liph::crypto;

TEST(crypto, md5) {
    EXPECT_EQ(md5(""), "D41D8CD98F00B204E9800998ECF8427E");
    EXPECT_EQ(md5("hello"), "5D41402ABC4B2A76B9719D911017C592");
    EXPECT_EQ(md5("1234567890123456789012345678901234567890"), "F5BF3E984432AE6F9F98840951E5CEF3");
}
