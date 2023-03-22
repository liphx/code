#include "liph/file.h"

#include <filesystem>

#include "gtest/gtest.h"

TEST(file, read_write) {
    const std::string str = "hello, world.";
    const std::string pathname = "./.file_test.tmp";
    EXPECT_TRUE(liph::write_file(pathname, str));

    std::string str2;
    EXPECT_TRUE(liph::read_file(pathname, str2));
    EXPECT_EQ(str, str2);
    EXPECT_TRUE(std::filesystem::remove(pathname));
    // system(("rm -f " + pathname).c_str());
}
