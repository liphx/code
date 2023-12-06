#include <iostream>
#include <string>
#include <string_view>

void f(bool) { std::cout << "bool" << std::endl; }
void f(const std::string&) { std::cout << "string" << std::endl; }
void f(std::string_view) { std::cout << "string_view" << std::endl; }

struct A {
    int x;
    A(int x2 = 0) : x(x2) { std::cout << "A(" << x << ")" << std::endl; }

    operator int() const {
        std::cout << "A::int()" << std::endl;
        return x;
    }
};

void f(int x) { std::cout << "f(): " << x << std::endl; }

void g(A a) { std::cout << "g(): " << a.x << std::endl; }

int main() {
    f("hello");                    // bool
    f(std::string("hello"));       // string
    f(std::string_view("hello"));  // string_view

    g(10);
    f(A(20));
}
