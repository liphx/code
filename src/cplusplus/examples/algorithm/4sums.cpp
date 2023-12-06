#include <algorithm>
#include <iostream>
#include <vector>

// select 4 of n numbers (repeatable)
// and determine whether there is a possibility that the sum is m
// O(n^2logn)
bool solve(const std::vector<int>& nums, int m) {
    if (nums.empty()) return false;
    std::vector<int> tmp(nums.size() * nums.size());
    size_t k = 0;
    for (size_t i = 0; i < nums.size(); i++) {
        for (size_t j = 0; j < nums.size(); j++) {
            tmp[k++] = nums[i] + nums[j];
        }
    }
    std::sort(tmp.begin(), tmp.end());
    for (size_t i = 0; i < nums.size(); i++) {
        for (size_t j = 0; j < nums.size(); j++) {
            int target = m - nums[i] - nums[j];
            if (std::binary_search(tmp.begin(), tmp.end(), target)) return true;
        }
    }
    return false;
}

int main() {
    std::vector nums{-9, 8, -3, 3, 7, 1};
    std::cout << std::boolalpha << solve(nums, 8) << std::endl;   // true
    std::cout << std::boolalpha << solve(nums, 13) << std::endl;  // true
    std::cout << std::boolalpha << solve(nums, 33) << std::endl;  // false
}
