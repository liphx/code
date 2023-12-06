#include <iomanip>
#include <iostream>

struct Date {
    int year;
    int month;
    int day;
};

std::ostream& operator<<(std::ostream& os, const Date& d) {
    os.fill('0');
    os << std::setw(4) << d.year << '-' << std::setw(2) << d.month << '-' << std::setw(2) << d.day;
    return os;
}

// output '\n', do not flush
std::ostream& nl(std::ostream& os) { return os << '\n'; }

int main() {
    Date d{2022, 8, 29};
    std::cout << d << nl;
}
