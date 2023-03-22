#include <iomanip>
#include <iostream>

struct Date {
    int year;
    int month;
    int day;
};

/*
 * 为自己的类定义插入符 << 和提取符 >>
 * 1. 第一个参数定义成流的非const引用
 * 2. 执行向/从流中插入/提取数据的操作
 * 3. 返回流的引用
 */
std::ostream& operator<<(std::ostream& os, const Date& d) {
    os.fill('0');
    os << std::setw(4) << d.year << '-' << std::setw(2) << d.month << '-' << std::setw(2) << d.day;
    return os;
}

// 仅输出'\n', 不刷新流
std::ostream& nl(std::ostream& os) { return os << '\n'; }

int main() {
    Date d{2022, 8, 29};
    std::cout << d << std::endl;
}
