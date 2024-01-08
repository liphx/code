#include "liph/lang/finally.h"

#include "gtest/gtest.h"

namespace liph {

TEST(finally, finally) {
    int *p = new int;
    auto action = finally([p]() { delete p; });
}

}  // namespace liph
