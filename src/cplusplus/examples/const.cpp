#include <iostream>
#include <string>

class Text {
public:
    Text(std::string s = "") : str(std::move(s)) {}

    const char& operator[](std::size_t pos) const {
        std::cout << "const operator[] called." << std::endl;
        return str[pos];
    }

    // non-const version calls const version
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
