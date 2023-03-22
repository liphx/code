// https://leetcode.cn/problems/remove-element/
// 给你一个数组 nums 和一个值 val，你需要 原地 移除所有数值等于 val 的元素，并返回移除后数组的新长度。
// 不要使用额外的数组空间，你必须仅使用 O(1) 额外空间并 原地 修改输入数组。
// 元素的顺序可以改变。你不需要考虑数组中超出新长度后面的元素。

#include "leetcode.h"

class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int n = nums.size();
        int i = 0, j = n - 1;
        while (i <= j) {
            if (nums[j] == val) {
                --j;
                continue;
            }
            if (nums[i] == val) {
                if (i != j)
                    nums[i++] = nums[j--];
                else {
                    ++i;
                    break;
                }
            } else
                ++i;
        }
        return i;
    }
};

int main() {
    vector<int> nums{3, 2, 2, 3};
    cout << Solution().removeElement(nums, 3) << endl;  // 2
}
