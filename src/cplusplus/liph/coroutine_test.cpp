#include "liph/coroutine.h"

#include <iostream>
#include <memory>

#include "gtest/gtest.h"

namespace liph {
namespace test {

processor p;

void foo(void *num) {
    for (int i = 0; i < 5; i++) {
        std::cout << (*(int *)num)++ << std::endl;
        p.yield();
    }
}

TEST(coroutine, coroutine) {
    int num1 = 0, num2 = 10;
    int co1 = p.add(std::make_shared<coroutine>(foo, &num1));
    int co2 = p.add(std::make_shared<coroutine>(foo, &num2));
    std::cout << "main begin" << std::endl;
    while (p.status(co1) && p.status(co2)) {
        p.resume(co1);
        p.resume(co2);
    }
    std::cout << "main end" << std::endl;
}

}  // namespace test
}  // namespace liph
