#include "logger.h"

#include "gtest/gtest.h"

namespace liph {
namespace test {

TEST(logger, logging) {
    Singleton<logger>::GetInstance("std.log")->start();
    logging("hello", 42, 1.0, true);
}

}  // namespace test
}  // namespace liph
