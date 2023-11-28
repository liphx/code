#include <iostream>
#include <string_view>
using std::string_view;

std::string_view get() { return "view"; }
std::string_view get(const std::string& str) { return str; }
// std::string_view get(std::string str) { return str; }

int main() {
    {
        auto s = get();
        std::cout << s << std::endl;
        std::cout << (void *)s.data() << " " << s.size() << std::endl;
    }

    {
        std::string str = "hello";
        auto s = get(str);
        std::cout << (void *)str.data() << std::endl;
        std::cout << s << std::endl;
        std::cout << (void *)s.data() << " " << s.size() << std::endl;
    }
}
