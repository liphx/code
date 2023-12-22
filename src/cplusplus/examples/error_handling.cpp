#include <cassert>
#include <exception>
#include <iostream>
#include <string>

enum class error_action { ignore, throwing, terminating, logging };
constexpr error_action default_error_action = error_action::throwing;

enum class error_code { range_error, length_error };
std::string error_code_name[]{"range error", "length error"};

template <error_action action = default_error_action, class C>
constexpr void expect(C cond, error_code x) {
    if constexpr (action == error_action::logging)
        if (!cond()) std::cerr << "expect() failure: " << int(x) << ' ' << error_code_name[int(x)] << '\n';
    if constexpr (action == error_action::throwing)
        if (!cond()) throw x;
    if constexpr (action == error_action::terminating)
        if (!cond()) std::terminate();
    // or no action
}

double& foo(double *ptr, size_t size, size_t i) {
    expect([size, i]() { return i < size; }, error_code::range_error);
    return ptr[i];
}

int main() {
    double data[]{0, 1.2, -3};
    foo(data, std::size(data), 0);
    foo(data, std::size(data), 3);
}
