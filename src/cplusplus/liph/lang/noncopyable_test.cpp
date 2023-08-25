#include "liph/lang/noncopyable.h"

#include <iostream>

#include "gtest/gtest.h"
#include "liph/logging.h"

struct A : liph::noncopyable {};

struct B : liph::noncopyable {
    int64_t data_;
};

TEST(noncopyable, sizeof) {
    LOG << sizeof(liph::noncopyable);
    LOG << sizeof(A);
    LOG << sizeof(B);
}
