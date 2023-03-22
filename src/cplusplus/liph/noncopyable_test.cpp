#include "noncopyable.h"

#include <iostream>

#include "gtest/gtest.h"

class A : liph::noncopyable {};

class B : liph::noncopyable {
    int64_t data_;
};

TEST(noncopyable, sizeof) {
    std::cout << sizeof(liph::noncopyable) << std::endl;
    std::cout << sizeof(A) << std::endl;
    std::cout << sizeof(B) << std::endl;
}
