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

TEST(time, julianday) { EXPECT_EQ(julianday(2018, 3, 27) - julianday(2000, 1, 1), 6660); }

TEST(time, day_of_the_week) {
    EXPECT_EQ(day_of_the_week(2023, 4, 18), 2);
    EXPECT_EQ(day_of_the_week(2023, 4, 18), 2);
    EXPECT_EQ(day_of_the_week(1998, 1, 1), 4);
    EXPECT_EQ(day_of_the_week(2004, 2, 29), 0);
    EXPECT_EQ(day_of_the_week(2023, 4, 2), 0);
}
