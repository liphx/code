// https://en.cppreference.com/w/cpp/language/function#Deleted_functions

#include <string_view>

void f(const auto&) = delete;
void f(std::string_view) {}

int main() {
    // f("hello");
    // error: call to deleted function 'f'
    f(std::string_view("hello"));
    // ok
}
