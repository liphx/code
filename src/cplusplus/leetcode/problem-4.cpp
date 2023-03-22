// https://leetcode.cn/problems/median-of-two-sorted-arrays/
// 寻找两个正序数组的中位数

#include "leetcode.h"

class Solution {
public:
    // O(log(size))
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int i = 0, j = 0, m = nums1.size(), n = nums2.size();
        while (m - i > 1 && n - j > 1) {
            int min1 = nums1[(i + m - 1) / 2];
            int min2 = nums2[(j + n - 1) / 2];
            int max1 = nums1[(i + m) / 2];
            int max2 = nums2[(j + n) / 2];
            if (min1 <= min2 && max1 >= max2) {
                return findMedianSortedArrays(nums2.data() + j, n - j);
            } else if (min1 >= min2 && max1 <= max2) {
                return findMedianSortedArrays(nums1.data() + i, m - i);
            }
            int k = min((m - i) / 2, (n - j) / 2);
            if (min1 < min2) {
                i += k;
                n -= k;
            } else {
                j += k;
                m -= k;
            }
        }

        if (i == m)
            return findMedianSortedArrays(nums2.data() + j, n - j);
        else if (j == n)
            return findMedianSortedArrays(nums1.data() + i, m - i);
        else if (i == m - 1)
            return findMedianSortedArrays(nums1[i], nums2.data() + j, n - j);
        else  // j == n - 1
            return findMedianSortedArrays(nums2[j], nums1.data() + i, m - i);
    }

private:
    double findMedianSortedArrays(int nums[], int n) {  // n > 0
        if (n % 2 != 0)
            return nums[n / 2];
        else
            return (nums[n / 2 - 1] + nums[n / 2] + 0.0) / 2;
    }

    double findMedianSortedArrays(int num, int nums[], int n) {  // n >= 0
        if (n == 0) return num;
        if (n == 1) return (num + nums[0] + 0.0) / 2;
        vector<int> tmp{num};
        if (n % 2 == 0) {
            tmp.emplace_back(nums[n / 2 - 1]);
            tmp.emplace_back(nums[n / 2]);
        } else {
            tmp.emplace_back(nums[n / 2 - 1]);
            tmp.emplace_back(nums[n / 2]);
            tmp.emplace_back(nums[n / 2 + 1]);
        }
        sort(tmp.begin(), tmp.end());
        return findMedianSortedArrays(tmp.data(), tmp.size());
    }

    // O(size)
    double findMedianSortedArrays2(vector<int>& nums1, vector<int>& nums2) {
        vector<int> nums;
        int i = 0, j = 0;
        for (; i < nums1.size() && j < nums2.size();) {
            if (nums1[i] < nums2[j])
                nums.emplace_back(nums1[i++]);
            else
                nums.emplace_back(nums2[j++]);
        }
        for (; i < nums1.size();) nums.emplace_back(nums1[i++]);
        for (; j < nums2.size();) nums.emplace_back(nums2[j++]);
        return findMedianSortedArrays(nums.data(), nums.size());
    }
};

int main() {
    {
        vector<int> nums1{1, 3};
        vector<int> nums2{2};
        cout << Solution().findMedianSortedArrays(nums1, nums2) << endl;  // 2
    }
    {
        vector<int> nums1{1, 2, 3};
        vector<int> nums2{1, 2, 2};
        cout << Solution().findMedianSortedArrays(nums1, nums2) << endl;  // 2
    }
    {
        vector<int> nums1{1, 7, 11, 13, 29, 52};
        vector<int> nums2{2, 7, 7, 12, 67, 102};
        cout << Solution().findMedianSortedArrays(nums1, nums2) << endl;  // 11.5
    }
    {
        vector<int> nums1{0, 0, 0, 0, 0};
        vector<int> nums2{-1, 0, 0, 0, 0, 0, 1};
        cout << Solution().findMedianSortedArrays(nums1, nums2) << endl;  // 0
    }
    {
        vector<int> nums1{2};
        vector<int> nums2{1, 3, 4};
        cout << Solution().findMedianSortedArrays(nums1, nums2) << endl;  // 2.5
    }
}
