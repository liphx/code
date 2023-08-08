#include "liph/concurrency/threadpool.h"

#include "gtest/gtest.h"
#include "liph/logging.h"

namespace liph {
namespace test {

TEST(threadpool, DISABLED_all) {
    auto *pool = new ThreadPool;
    LOG << "thread pool size = " << pool->Size();
    pool->Start();
    int i = 0;
    auto entry = [](int *n) { *n = 100; };
    pool->AddTask(entry, &i);
    sleep(1);  // wait
    delete pool;
    EXPECT_EQ(i, 100);
}

}  // namespace test
}  // namespace liph
