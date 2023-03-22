#include "format.h"

#include "gtest/gtest.h"

namespace liph {
namespace test {

TEST(format, format) {
    std::cout << format("The answer is {}.", 42) << std::endl;
    // 忽略多余参数
    std::cout << format("{} {}", 1, 2, 3) << std::endl;
}

}  // namespace test
}  // namespace liph
