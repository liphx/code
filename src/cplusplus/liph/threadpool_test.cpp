#include "threadpool.h"

#include "gtest/gtest.h"

namespace liph {
namespace test {

TEST(threadpool, all) {
    auto *pool = new ThreadPool;
    std::cout << "thread pool size = " << pool->Size() << std::endl;
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
