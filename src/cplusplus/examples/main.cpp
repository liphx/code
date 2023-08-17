#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "liph/liph.h"
using namespace liph;
int main(int argc, char **argv) {
    std::string str;
    read_file(argv[1], str);
    for (auto ch : str) {
        std::cout << (unsigned int)((unsigned char)ch) << "-";
    }
    std::cout << std::endl;
}

#if 0
bool solve(int n, int *ptr) {
    int count[3]{0};
    for (int i = 0; i < n; i++) {
        if (ptr[i] == 0)
            count[0]++;
        else if (ptr[i] < 0)
            count[1]++;
        else
            count[2]++;
    }
    return std::abs(count[1] - count[2]) <= count[0] + 1;
}

int main() {
    int N;
    std::cin >> N;
    while (N-- > 0) {
        int n;
        std::cin >> n;
        std::vector<int> nums(n);
        for (int i = 0; i < n; i++) {
            std::cin >> nums[i];
        }
        std::cout << (solve(n, nums.data()) ? "Yes" : "No") << std::endl;
    }
}
#endif

#if 0
int main() {
    std::cout << __builtin_clz(1) << std::endl;        // 31
    std::cout << __builtin_clz(1 << 16) << std::endl;  // 15
    // 00001000  4
    // 00001000  1 << (7 - 4)
    // 00000111
    char a = 0b00001000;
    std::cout << __builtin_clz(a) << std::endl;
    char b = 1 << (31 - __builtin_clz(a));
    char c = b - 1;
    std::cout << (int)a << std::endl;
    std::cout << (int)b << std::endl;
    std::cout << (int)c << std::endl;
}
#endif
