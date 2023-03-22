// https://leetcode.cn/problems/3sum-closest/
// 给定一个包括 n 个整数的数组 nums 和 一个目标值 target。
// 找出 nums 中的三个整数，使得它们的和与 target 最接近。
// 返回这三个数的和。假定每组输入只存在唯一答案。

#include "leetcode.h"

class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int ans = INT_MAX;
        for (int i = 0; i < n; i++) {
            int tmp = target - nums[i];
            int j = i + 1, k = n - 1;
            while (j < k) {
                int tmp2 = tmp - (nums[j] + nums[k]);
                if (tmp2 == 0) return target;
                if (abs(tmp2) < abs(ans)) ans = tmp2;
                if (tmp2 < 0)  // 注意这里是小于0
                    k--;
                else
                    j++;
            }
        }
        return target - ans;
    }
};

int main() {
    vector<int> nums{0, 2, 1, -3};
    cout << Solution().threeSumClosest(nums, 1) << endl;  // 0
}
