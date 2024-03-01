#include "liph/process.h"

#include "gtest/gtest.h"
#include "liph/concurrency/thread.h"
#include "liph/logging.h"

namespace liph {

TEST(process, single_proc_guard) {
    auto ret = single_proc("/tmp/single_proc_guard", true);
    LOG << "run here";
    EXPECT_TRUE(ret);
    // sleeps(5);
}

}  // namespace liph
