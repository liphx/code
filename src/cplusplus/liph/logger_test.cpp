#include "logger.h"

#include "gtest/gtest.h"

namespace liph {
namespace test {

TEST(logger, logging) {
    singleton<logger>::instance("std.log").start();
    logging("hello", 42, 1.0, true);
}

}  // namespace test
}  // namespace liph
