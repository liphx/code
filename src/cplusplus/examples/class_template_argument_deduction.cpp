// Class template argument deduction (CTAD) (since C++17)
// https://en.cppreference.com/w/cpp/language/class_template_argument_deduction

#include <array>
#include <mutex>
#include <vector>

int main() {
    std::vector vc{0, 1, 2};                     // vector<int>
    std::vector vc2{vc.begin(), vc.end()};       // vector<iterator>
    std::vector<int> vc3{vc.begin(), vc.end()};  // vector<int>

    std::vector vc4{0, 1.0};  // vector<double>

    // std::array arr{0, 1.0};
    // error: types differ

    std::mutex m;
    std::lock_guard guard(m);
}
