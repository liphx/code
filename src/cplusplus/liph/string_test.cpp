#include "liph/string.h"

#include <list>

#include "gtest/gtest.h"
#include "liph/print.h"

namespace liph {

TEST(string, trim) {
    std::string str = "\f\r  hello  \n\t ";
    EXPECT_EQ(ltrim(str), "hello  \n\t ");
    EXPECT_EQ(rtrim(str), "\f\r  hello");
    EXPECT_EQ(trim(str), "hello");
}

TEST(string, trim_charset) {
    EXPECT_EQ(ltrim("0123", "0"), "123");
    EXPECT_EQ(ltrim("00123", "0"), "123");
    EXPECT_EQ(ltrim("00123", "01"), "23");
    EXPECT_EQ(ltrim("000", "0"), "");

    EXPECT_EQ(ltrim("000", ""), "000");
    EXPECT_EQ(ltrim(""), "");
}

TEST(string, startswith_endswith) {
    std::string str = "world";
    EXPECT_TRUE(startswith(str, "wor"));
    EXPECT_FALSE(startswith(str, "wr"));
    EXPECT_TRUE(endswith(str, "ld"));
    EXPECT_FALSE(endswith(str, "l"));
}

TEST(string, basename) {
    EXPECT_EQ(basename(""), "");
    EXPECT_EQ(basename("abc"), "abc");
    EXPECT_EQ(basename("/"), "/");
    EXPECT_EQ(basename("//////"), "/");
    EXPECT_EQ(basename("/usr/bin/ls"), "ls");
    EXPECT_EQ(basename("/usr/bin/"), "bin");
    EXPECT_EQ(basename("/usr/bin///"), "bin");
    EXPECT_EQ(basename("/usr/////bin/"), "bin");
    EXPECT_EQ(basename("/tmp/"), "tmp");
    EXPECT_EQ(basename("tmp/"), "tmp");
    EXPECT_EQ(basename("tmp/abc///"), "abc");
    EXPECT_EQ(basename("../.."), "..");
}

TEST(string, dirname) {
    EXPECT_EQ(dirname(""), ".");
    EXPECT_EQ(dirname("abc"), ".");
    EXPECT_EQ(dirname("/"), "/");
    EXPECT_EQ(dirname("//////"), "/");
    EXPECT_EQ(dirname("/usr/bin/ls"), "/usr/bin");
    EXPECT_EQ(dirname("/usr/bin/"), "/usr");
    EXPECT_EQ(dirname("/usr/bin///"), "/usr");
    EXPECT_EQ(dirname("/usr/////bin/"), "/usr");
    EXPECT_EQ(dirname("/tmp/"), "/");
    EXPECT_EQ(dirname("tmp/"), ".");
    EXPECT_EQ(dirname("tmp/abc///"), "tmp");
    EXPECT_EQ(dirname("/tmp////abc/a"), "/tmp////abc");
    EXPECT_EQ(dirname("../.."), "..");
}

TEST(string, to_int) {
    auto i = to_int("100");
    EXPECT_EQ(*i, 100);
    i = to_int("2147483647");
    EXPECT_EQ(*i, 2147483647);
    i = to_int("2147483648");
    EXPECT_FALSE(i);
    i = to_int("hello");
    EXPECT_FALSE(i);
}

TEST(string, replace) {
    EXPECT_EQ(replace("abcabc", "ab", "ba"), "bacbac");
    EXPECT_EQ(replace("aaaa", "aa", "a"), "aa");
    EXPECT_EQ(replace("aaa", "aa", "a"), "aa");
}

TEST(string, join) {
    EXPECT_EQ(join(std::vector{1, 2, 3}, ", "), "1, 2, 3");
    EXPECT_EQ(join(std::list{1, 2, 3}), "1 2 3");
    std::string arr[]{"hello", "world"};
    EXPECT_EQ(join(arr, ", "), "hello, world");
}

TEST(string, to) {
    EXPECT_EQ(to<std::string>(0), "0");
    EXPECT_EQ(to(3.14), "3.14");
    EXPECT_EQ(to<double>("3.14"), 3.14);
}

}  // namespace liph
