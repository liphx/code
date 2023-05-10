// https://leetcode.cn/problems/subarray-sum-equals-k/

#include "leetcode.h"

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<int, int> count{{0, 1}};
        int ret = 0, sum = 0;
        for (int num : nums) {
            sum += num;
            ret += count[sum - k];
            ++count[sum];
        }
        return ret;
    }
};

int main() {
    vector<int> num{1, 1, 1};
    cout << Solution().subarraySum(num, 2) << endl;  // 2

    num = vector<int>{1, 2, 3};
    cout << Solution().subarraySum(num, 3) << endl;  // 2

    num = vector<int>{1, -1, 0};
    cout << Solution().subarraySum(num, 0) << endl;  // 3

    num = vector<int>{1, 2, 1, 2, 1};
    cout << Solution().subarraySum(num, 3) << endl;  // 4

    num = vector<int>{-1, -1, 1};
    cout << Solution().subarraySum(num, 1) << endl;  // 1
}
