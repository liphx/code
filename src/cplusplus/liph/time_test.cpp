#include "liph/time.h"

#include <cinttypes>
#include <cstdio>

#include "gtest/gtest.h"

using namespace liph;

TEST(time, backwards) {
    int64_t pre = gettimeofday_us();
    while (true) {
        int64_t now = gettimeofday_us();
        int64_t diff = now - pre;
        printf("%" PRId64 "\n", diff);
        if (diff < 0) {
            fprintf(stderr, "backwards: pre=%" PRId64 ", now=%" PRId64 "\n", pre, now);
            break;
        }
        pre = now;
    }
}
