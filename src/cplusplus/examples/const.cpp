#include <iostream>
#include <string>

class Text {
public:
    Text(std::string s = "") : str(s) {}

    const char& operator[](std::size_t pos) const {
        std::cout << "const operator[] called." << std::endl;
        return str[pos];
    }

    // non-const 版本调用 const 版本代码实现
    char& operator[](std::size_t pos) {
        std::cout << "non-const operator[] called." << std::endl;
        return const_cast<char&>(static_cast<const Text&>(*this)[pos]);
    }

private:
    std::string str;
};

int main() {
    Text t("hello");
    t[0] = 's';

    const Text t2("world");
    t2[0];
}
