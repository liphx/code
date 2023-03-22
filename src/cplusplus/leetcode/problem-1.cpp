// https://leetcode.cn/problems/two-sum/

#include "leetcode.h"

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> data;
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            int t = target - nums[i];
            if (data.find(t) != data.end()) {
                return vector<int>{data[t], i};
            }
            data[nums[i]] = i;
        }
        return vector<int>();
    }
};

int main() {
    vector<int> data{2, 7, 11, 5};
    vector<int> ans = Solution().twoSum(data, 9);
    cout << ans << endl;  // [0, 1]
}
