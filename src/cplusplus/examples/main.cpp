#include <iostream>
#include <vector>

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
