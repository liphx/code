#include "liph/crypto/md5.h"

#include <iostream>

#include "gtest/gtest.h"

TEST(md5, md5) {
    // EXPECT_EQ(liph::MD5(""), "D41D8CD98F00B204E9800998ECF8427E");
    // EXPECT_EQ(liph::MD5("hello"), "5D41402ABC4B2A76B9719D911017C592");
    // EXPECT_EQ(liph::MD5("1234567890123456789012345678901234567890"), "F5BF3E984432AE6F9F98840951E5CEF3");

    // EXPECT_EQ(liph::md5("a"), "0cc175b9c0f1b6a831c399e269772661");
    // EXPECT_EQ(liph::md5("abc"), "900150983cd24fb0d6963f7d28e17f72");
    // EXPECT_EQ(liph::md5("message digest"), "f96b697d7cb7938d525a2f31aaf161d0");
    // EXPECT_EQ(liph::md5("abcdefghijklmnopqrstuvwxyz"), "c3fcd3d76192e4007dfb496cca67e13b");
    // EXPECT_EQ(liph::md5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"),
    //         "d174ab98d277d9f5a5611c2c9f419d9f");
    // EXPECT_EQ(liph::md5("12345678901234567890123456789012345678901234567890123456789012345678901234567890"),
    //         "57edf4a22be3c955ac49da2e2107b67a");
}
