#ifndef LIPH_TIME_H_
#define LIPH_TIME_H_

#include <sys/time.h>

#include <cstdint>
#include <cstdio>
#include <ctime>
#include <string>

namespace liph {

inline int64_t gettimeofday_us() {
    struct timeval now;
    if (gettimeofday(&now, NULL) != 0) {
        perror("gettimeofday");
    }
    return now.tv_sec * 1000000L + now.tv_usec;
}

inline int64_t gettimeofday_ms() { return gettimeofday_us() / 1000L; }

inline int64_t gettimeofday_s() { return gettimeofday_us() / 1000000L; }

inline std::string time_format(time_t t = time(nullptr)) {
    struct tm tm;
    localtime_r(&t, &tm);
    char str[32];
    if (strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &tm) == 0) {
        return "";
    }
    return str;
}

inline int julianday(int year, int month, int day) {
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    return day + (153 * m + 2) / 5 + y * 365 + y / 4 - y / 100 + y / 400 - 32045;
}

// Tomohiko Sakamoto’s Algorithm
inline int day_of_the_week(int y, int m, int d) {
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};  // array with leading number of days values
    y -= m < 3;                                             // if month is less than 3 reduce year by 1
    return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

#if 0
inline int day_of_the_week(int y, int m, int d) {
    y -= m < 3;
    return (y + y / 4 - y / 100 + y / 400 + "-bed=pen+mad."[m] + d) % 7;
}
#endif

};  // namespace liph

#endif  // LIPH_TIME_H_
