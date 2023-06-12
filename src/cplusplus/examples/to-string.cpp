#include <iostream>
#include <sstream>

void print(double f) {
    std::cout << "cout      : " << f << std::endl;
    std::cout << "to_string : " << std::to_string(f) << std::endl;

    std::ostringstream os;
    /* os.precision(7); */
    os << f;
    std::cout << "sstream   : " << os.str() << std::endl;
}

int main() {
    double f = 1e40;
    print(f);

    double f2 = std::strtod("1e40", nullptr);
    print(f2);

    double f3 = std::strtod("10000000000000000303786028427003666890752.000000", nullptr);
    print(f3);
}
