#include <string>
#include <utility>

// https://clang.llvm.org/extra/clang-tidy/checks/modernize/pass-by-value.html
// Pass-by-value in constructors

struct A {
    A(std::string str) : str_(std::move(str)) {}
    std::string str_;
};

struct B {
    B(const std::string& str) : str_(str) {}
    std::string str_;
};

int main() {
    std::string str = "hello";
    A a1(str);             // lvalue, copy
    A a2(std::move(str));  // rvalue, move

    std::string str2 = "hello";
    B b1(str2);             // lvalue, copy
    B b2(std::move(str2));  // rvalue, copy
}
