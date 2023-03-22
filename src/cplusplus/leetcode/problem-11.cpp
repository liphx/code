// https://leetcode.cn/problems/container-with-most-water/

#include "leetcode.h"

class Solution {
public:
    int maxArea(vector<int>& height) {
        int i = 0, j = height.size() - 1;
        int ret = 0;
        while (i < j) {
            if (height[i] > height[j]) {
                ret = max(ret, (j - i) * height[j]);
                j--;
            } else {
                ret = max(ret, (j - i) * height[i]);
                i++;
            }
        }
        return ret;
    }
};

int main() {
    vector<int> height{1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << Solution().maxArea(height) << endl;  // 49
}
