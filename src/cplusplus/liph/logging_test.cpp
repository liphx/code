#include "liph/logging.h"

#include "gtest/gtest.h"
#include "liph/format.h"

TEST(logging, logging) {
    std::string str = "world";
    LOG << "hello " << str;
    LOG << liph::format("The answer is {}", 42);
}
