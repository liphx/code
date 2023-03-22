#include <stdio.h>

int getJulianDayNumber(int year, int month, int day) {
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    return day + (153 * m + 2) / 5 + y * 365 + y / 4 - y / 100 + y / 400 - 32045;
}

int main() { printf("%d days\n", getJulianDayNumber(2018, 3, 27) - getJulianDayNumber(2000, 1, 1)); }
