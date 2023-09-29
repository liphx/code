#include "liph/numeric/random.h"

#include "gtest/gtest.h"
#include "liph/logging.h"
#include "liph/string.h"

TEST(random, random_string) {
    auto str = liph::random_string(8);
    LOG << str;
    str = liph::ltrim(str, "0");
    if (str.empty()) {
        str = "0";
    }
    LOG << str;
}
