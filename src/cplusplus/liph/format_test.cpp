#include "liph/format.h"

#include "gtest/gtest.h"
#include "liph/logging.h"

TEST(format, format) {
    LOG << liph::format("The answer is {}.", 42);
    // 忽略多余参数
    LOG << liph::format("{} {}", 1, 2, 3);
}
