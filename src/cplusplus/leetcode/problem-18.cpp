// https://leetcode.cn/problems/4sum/
// 给定一个包含 n 个整数的数组 nums 和一个目标值 target，判断 nums 中是否存在四个元素 a，b，c 和 d ，
// 使得 a + b + c + d 的值与 target 相等？ 找出所有满足条件且不重复的四元组。答案中不可以包含重复的四元组。

#include "leetcode.h"

class Solution {
public:
    // 构造两数之和的数组，转化成twoSum问题，leetcode no.1
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        set<vector<int>> ans;
        int size = nums.size();
        vector<pair<int, pair<int, int>>> num1;
        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                num1.emplace_back(make_pair((nums[i] + nums[j]), make_pair(i, j)));  // i != j
            }
        }
        unordered_multimap<int, pair<int, int>> num2{num1.begin(), num1.end()};
        size = num1.size();
        for (int i = 0; i < size; i++) {
            int tmp = target - num1[i].first;
            int x1 = num1[i].second.first;
            int y1 = num1[i].second.second;
            auto pr = num2.equal_range(tmp);
            if (pr.first != num2.end()) {
                for (auto iter = pr.first; iter != pr.second; iter++) {
                    int x2 = iter->second.first;
                    int y2 = iter->second.second;
                    if (x1 != x2 && x1 != y2 && y1 != x2 && y1 != y2) {
                        vector<int> tmp2{nums[x1], nums[y1], nums[x2], nums[y2]};
                        sort(tmp2.begin(), tmp2.end());
                        ans.insert(tmp2);
                    }
                }
            }
        }
        return vector<vector<int>>(ans.begin(), ans.end());
    }
};

int main() {
    vector<int> nums{1, 0, -1, 0, -2, 2};
    auto ans = Solution().fourSum(nums, 0);
    cout << ans << endl;  // [[-2, -1, 1, 2], [-2, 0, 0, 2], [-1, 0, 0, 1]]
}
