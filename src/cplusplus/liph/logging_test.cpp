#include "liph/logging.h"

#include "gtest/gtest.h"

namespace liph {

TEST(logging, logging) {
    std::string str = "world";
    LOG << "hello " << str;
    LOG << format("The answer is {}", 42);
}

TEST(logging, logtostderr) {
    singleton<logger>::instance().set_logtostderr(true);
    std::string str = "world";
    LOG << "hello " << str;
    LOG << format("The answer is {}", 42);
}

}  // namespace liph
