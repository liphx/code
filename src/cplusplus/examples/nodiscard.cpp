// https://en.cppreference.com/w/cpp/language/attributes/nodiscard
// use [[nodiscard]] if possible

// nodiscard with return value
[[nodiscard]] int get() { return 42; }

// nodiscard with type
struct [[nodiscard]] A {};

A getA() { return A{}; }

// nodiscard with constructor
struct B {
    B() {}
    [[nodiscard]] B(int) {}
};

// nodiscard with message
[[nodiscard("use this return value")]] bool empty() { return true; }

// nodiscard with enumeration
enum [[nodiscard]] error_type { ok, error };
error_type type() { return ok; }

#include <future>
#include <vector>

int main() {
    get();  // warning

    auto l = [] [[nodiscard]] () -> int { return 42; };  // C++23
    l();                                                 // warning

    getA();  // warning

    B();     // no warning
    B b(0);  // not discarded, no warning
    B(0);    // warning

    empty();  // warning with "use this return value"

    type();  // warning

    std::vector<int> vc;
    vc.empty();  // warning, container::empty is [[nodiscard]]

    std::async([]() {});
    // warning
}
