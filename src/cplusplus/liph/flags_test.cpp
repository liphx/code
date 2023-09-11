#include "liph/flags.h"

#include "gtest/gtest.h"
#include "liph/logging.h"

TEST(flags, flags) {
    std::vector<const char *> args{
            "a.out", "abc", "-msg", "hello", "--enable", "true", "--num", "90", "123", "-amount", "3.9"};
    liph::flags flags;
    flags.register_bool_flag("enable");
    flags.register_int32_flag("num");
    flags.register_string_flag("msg");
    flags.register_double_flag("amount");

    char **argv = (char **)args.data();
    EXPECT_TRUE(flags.parse_flags(args.size(), &argv));
    EXPECT_TRUE(flags.bool_ref("enable"));
    EXPECT_EQ(flags.int32_ref("num"), 90);
    EXPECT_EQ(flags.double_ref("amount"), 3.9);
    EXPECT_EQ(flags.string_ref("msg"), "hello");

    EXPECT_THROW(flags.string_ref("enable"), std::exception);
    EXPECT_THROW(flags.string_ref("notfound"), std::exception);
}

TEST(flags, help_message) {
    liph::flags flags;
    flags.register_bool_flag("enable", true, "aaa");
    flags.register_string_flag("name", "liph", "bbb");
    LOG << flags.help();
}
