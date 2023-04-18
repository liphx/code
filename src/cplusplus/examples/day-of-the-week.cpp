// Tomohiko Sakamoto’s Algorithm
int day_of_the_week(int y, int m, int d) {
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};  // array with leading number of days values
    y -= m < 3;                                             // if month is less than 3 reduce year by 1
    return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

int day_of_the_week2(int y, int m, int d) {
    y -= m < 3;
    return (y + y / 4 - y / 100 + y / 400 + "-bed=pen+mad."[m] + d) % 7;
}

#include <iostream>

int main() {
    std::cout << day_of_the_week(2023, 4, 18) << std::endl;   // 2
    std::cout << day_of_the_week2(2023, 4, 18) << std::endl;  // 2
    std::cout << day_of_the_week2(1998, 1, 1) << std::endl;   // 4
    std::cout << day_of_the_week2(2004, 2, 29) << std::endl;  // 0
    std::cout << day_of_the_week2(2023, 4, 2) << std::endl;   // 0
}
