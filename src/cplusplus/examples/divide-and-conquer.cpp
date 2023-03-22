#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
 * 在递增序列 A[i, j) 中寻找 num
 */
bool _binary_search(int A[], int i, int j, int num) {
    if (i >= j) return false;
    int mid = i + (j - i) / 2;
    if (A[mid] == num)
        return true;
    else if (A[mid] > num)
        return _binary_search(A, i, mid, num);
    else
        return _binary_search(A, mid + 1, j, num);
}

/* 二分搜索，递归版 */
bool binary_search(int A[], int n, int num) { return _binary_search(A, 0, n, num); }

/* 二分搜索，循环版 */
bool binary_search2(int A[], int n, int num) {
    int i = 0, j = n, mid;
    while (i < j) {
        mid = i + (j - i) / 2;
        if (A[mid] == num)
            return true;
        else if (A[mid] > num)
            j = mid;
        else
            i = mid + 1;
    }
    return false;
}

/* 分治法数组求和 */
int sum(int A[], int n) { return n == 1 ? A[0] : A[n - 1] + sum(A, n - 1); }

template <typename T>
T gcd(T x, T y) {
    return x == 0 ? y : y == 0 ? x : gcd(y, x % y);
}

template <typename T>
T lcm(T x, T y) {
    return (x != 0 && y != 0) ? (x / gcd(x, y)) * y : 0;
}

int merge(vector<int>& nums, int left, int mid, int right, vector<int>& tmp) {
    int ans = 0;
    int i = left, j = mid, index = 0;
    while (i < mid && j < right) {
        if (nums[i] <= nums[j]) {
            tmp[index++] = nums[i++];
        } else {
            tmp[index++] = nums[j++];
            ans += mid - i;
        }
    }
    while (i < mid) {
        tmp[index++] = nums[i++];
    }
    while (j < right) {
        tmp[index++] = nums[j++];
    }
    copy(tmp.begin(), tmp.begin() + right - left, nums.begin() + left);
    return ans;
}

int merge_sort(vector<int>& nums, int left, int right, vector<int>& tmp) {
    int ans = 0;
    if (right - left <= 1) return ans;
    int mid = left + (right - left) / 2;
    ans += merge_sort(nums, left, mid, tmp);
    ans += merge_sort(nums, mid, right, tmp);
    ans += merge(nums, left, mid, right, tmp);
    return ans;
}

/* 归并排序求逆序数 */
int reversePairs(vector<int>& nums) {
    vector<int> tmp(nums.size());
    return merge_sort(nums, 0, nums.size(), tmp);
}

/*
 * 快速幂取模
 * a ^ b % mod
 * a, b, mod > 0
 */
template <typename T>
T power_mod(T a, T b, T mod) {
    if (b == 1) return a % mod;
    T s = power_mod(a, b / 2, mod);
    return (b & 1) == 0 ? s * s % mod : s * s * a % mod;
}

int maxIncludeTwo(int nums[], int n, int mid) {
    int a = nums[mid - 1] + nums[mid];
    int b = a;
    int i = mid + 1;
    while (i < n) {
        b += nums[i++];
        a = max(a, b);
    }
    b = a;
    i = mid - 2;
    while (i >= 0) {
        b += nums[i--];
        a = max(a, b);
    }
    return a;
}

/*
 * 给定一个整数数组 nums(n > 0)，找到一个具有最大和的连续子数组，返回其最大和
 */
int maxSubArray(int nums[], int n) {
    if (n == 1) return nums[0];
    return max(max(maxSubArray(nums, n / 2), maxSubArray(nums + n / 2, n - n / 2)), maxIncludeTwo(nums, n, n / 2));
}

/* 同上，O(n)复杂度 */
int maxSubArray2(int nums[], int n) {
    int ans = nums[0], sum = 0;
    for (int i = 0; i < n; i++) {
        if (sum > 0)
            sum += nums[i];
        else
            sum = nums[i];
        ans = max(ans, sum);
    }
    return ans;
}

int main() {}
