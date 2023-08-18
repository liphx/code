#include "liph/time.h"

#include <cinttypes>
#include <cstdio>

#include "gtest/gtest.h"
#include "liph/logging.h"

using namespace liph;

TEST(time, DISABLED_backwards) {
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

TEST(time, time_format) {
    LOG << "now: " << time_format();
    // EXPECT_EQ(time_format(0), "1970-01-01 08:00:00");
}
