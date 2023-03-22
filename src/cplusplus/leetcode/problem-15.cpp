// https://leetcode.cn/problems/3sum/
// 给定一个包含 n 个整数的数组 nums，判断 nums 中是否存在三个元素 a，b，c ，使得 a + b + c = 0 ？
// 找出所有满足条件且不重复的三元组。 注意：答案中不可以包含重复的三元组。

#include "leetcode.h"

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        set<vector<int>> ret;
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int pre = INT_MAX;  // 针对重复用例的优化
        for (int i = 0; i < n - 2; ++i) {
            int j = i + 1, k = n - 1;
            int target = -nums[i];
            if (target == pre) continue;
            pre = target;
            while (j < k) {
                if ((nums[j] + nums[k]) == target) {
                    ret.insert(vector<int>{nums[i], nums[j], nums[k]});
                    ++j;
                    --k;
                } else if ((nums[j] + nums[k]) > target) {
                    --k;
                } else {
                    ++j;
                }
            }
        }
        return vector<vector<int>>(ret.begin(), ret.end());
    }
};

int main() {
    vector<int> nums{-1, 0, 1, 2, -1, -4};
    auto ans = Solution().threeSum(nums);
    cout << ans << endl;  // [[-1, -1, 2], [-1, 0, 1]]
}
