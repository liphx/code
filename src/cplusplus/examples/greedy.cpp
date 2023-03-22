#include <vector>
/*
 * 给你一个整数数组 nums, nums.size() > 0
 * 每一次操作中，你可以选择数组中一个元素，并将它增加 1
 * 返回使 nums 严格递增 的 最少 操作次数。
 */
int minOperations(std::vector<int>& nums) {
    int ans = 0, prev = nums[0];
    for (int i = 1; i < nums.size(); i++) {
        if (nums[i] > prev)
            prev = nums[i];
        else {
            ans += prev - nums[i] + 1;
            prev = prev + 1;
        }
    }
    return ans;
}

int main(int argc, char **argv) {}
