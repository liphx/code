#include "liph/string.h"

#include "gtest/gtest.h"
#include "liph/print.h"

TEST(string, trim) {
    std::string str = "\f\r  hello  \n\t ";
    EXPECT_EQ(liph::ltrim(str), "hello  \n\t ");
    EXPECT_EQ(liph::rtrim(str), "\f\r  hello");
    EXPECT_EQ(liph::trim(str), "hello");
}

TEST(string, trim_charset) {
    EXPECT_EQ(liph::ltrim("0123", "0"), "123");
    EXPECT_EQ(liph::ltrim("00123", "0"), "123");
    EXPECT_EQ(liph::ltrim("00123", "01"), "23");
    EXPECT_EQ(liph::ltrim("000", "0"), "");

    EXPECT_EQ(liph::ltrim("000", ""), "000");
    EXPECT_EQ(liph::ltrim(""), "");
}

TEST(string, startswith_endswith) {
    std::string str = "world";
    EXPECT_TRUE(liph::startswith(str, "wor"));
    EXPECT_FALSE(liph::startswith(str, "wr"));
    EXPECT_TRUE(liph::endswith(str, "ld"));
    EXPECT_FALSE(liph::endswith(str, "l"));
}

TEST(string, basename) {
    EXPECT_EQ(liph::basename(""), "");
    EXPECT_EQ(liph::basename("abc"), "abc");
    EXPECT_EQ(liph::basename("/"), "/");
    EXPECT_EQ(liph::basename("//////"), "/");
    EXPECT_EQ(liph::basename("/usr/bin/ls"), "ls");
    EXPECT_EQ(liph::basename("/usr/bin/"), "bin");
    EXPECT_EQ(liph::basename("/usr/bin///"), "bin");
    EXPECT_EQ(liph::basename("/usr/////bin/"), "bin");
    EXPECT_EQ(liph::basename("/tmp/"), "tmp");
    EXPECT_EQ(liph::basename("tmp/"), "tmp");
    EXPECT_EQ(liph::basename("tmp/abc///"), "abc");
    EXPECT_EQ(liph::basename("../.."), "..");
}

TEST(string, dirname) {
    EXPECT_EQ(liph::dirname(""), ".");
    EXPECT_EQ(liph::dirname("abc"), ".");
    EXPECT_EQ(liph::dirname("/"), "/");
    EXPECT_EQ(liph::dirname("//////"), "/");
    EXPECT_EQ(liph::dirname("/usr/bin/ls"), "/usr/bin");
    EXPECT_EQ(liph::dirname("/usr/bin/"), "/usr");
    EXPECT_EQ(liph::dirname("/usr/bin///"), "/usr");
    EXPECT_EQ(liph::dirname("/usr/////bin/"), "/usr");
    EXPECT_EQ(liph::dirname("/tmp/"), "/");
    EXPECT_EQ(liph::dirname("tmp/"), ".");
    EXPECT_EQ(liph::dirname("tmp/abc///"), "tmp");
    EXPECT_EQ(liph::dirname("/tmp////abc/a"), "/tmp////abc");
    EXPECT_EQ(liph::dirname("../.."), "..");
}
