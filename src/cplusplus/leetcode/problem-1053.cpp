// https://leetcode.cn/problems/previous-permutation-with-one-swap/

#include "leetcode.h"

class Solution {
public:
    int find(int *arr, int i, int j, int m) {
        int max = INT_MIN;
        int idx = -1;
        while (i < j) {
            if (arr[i] < m && arr[i] > max) {
                max = arr[i];
                idx = i;
            }
            i++;
        }
        return idx;
    }

    vector<int> prevPermOpt1(vector<int>& arr) {
        if (arr.size() <= 1) return arr;
        for (int i = arr.size() - 1; i >= 0; i--) {
            int j = find(arr.data(), i + 1, arr.size(), arr[i]);
            if (j != -1) {
                std::swap(arr[i], arr[j]);
                return arr;
            }
        }
        return arr;
    }
};

int main() {
    vector<int> arr{3, 2, 1};
    std::cout << Solution().prevPermOpt1(arr) << std::endl;

    arr = std::vector<int>{1, 1, 5};
    std::cout << Solution().prevPermOpt1(arr) << std::endl;

    arr = std::vector<int>{1, 9, 4, 6, 7};
    std::cout << Solution().prevPermOpt1(arr) << std::endl;

    arr = std::vector<int>{3, 1, 1, 3};
    std::cout << Solution().prevPermOpt1(arr) << std::endl;
    // 1 3 1 3

    arr = std::vector<int>{5, 3, 1, 1, 3};
    std::cout << Solution().prevPermOpt1(arr) << std::endl;
    // 5, 1, 3, 1, 3
}
