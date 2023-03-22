// https://leetcode.cn/problems/remove-duplicates-from-sorted-array/
// 给定一个排序数组，你需要在原地删除重复出现的元素，使得每个元素只出现一次，返回移除后数组的新长度。
// 不要使用额外的数组空间，你必须在原地修改输入数组并在使用O(1)额外空间的条件下完成。

#include "leetcode.h"

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.empty()) return 0;
        int n = nums.size();
        int pre = nums[0];
        int k = 1;
        for (int i = 1; i < n; ++i) {
            if (nums[i] != pre) {
                pre = nums[i];
                if (k == i)
                    ++k;
                else
                    nums[k++] = nums[i];
            }
        }
        return k;
    }
};

int main() {
    vector<int> nums{1, 1, 2};
    cout << Solution().removeDuplicates(nums) << endl;  // 2
}
