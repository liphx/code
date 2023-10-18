#include <iostream>
#include <string>
#include <string_view>

void f(bool) { std::cout << "bool" << std::endl; }
void f(const std::string&) { std::cout << "string" << std::endl; }
void f(std::string_view) { std::cout << "string_view" << std::endl; }

int main() {
    f("hello");                    // bool
    f(std::string("hello"));       // string
    f(std::string_view("hello"));  // string_view
}
