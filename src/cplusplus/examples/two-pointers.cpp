/*
 * 链表中的双指针
 *
 * 1. 两个链表的第一个公共结点
 * 2. 找到链表开始入环的第一个节点
 * 3. 找到链表倒数第k个节点
 *
 * 数组中的双指针与滑动窗口
 *
 * 4. 在递增数组中查找两个数，使得他们的和正好是给定的值
 * 5. 给定一个数组和滑动窗口的大小，找出所有滑动窗口里数值的最大值
 * 6. 给定一个字符串和一些单词，找出字符串中恰好可以由所有单词串联形成的子串的起始位置 (lc.30)
 * 7. 给定一个字符串，找出其中不含有重复字符的最长子串的长度 (lc.3)
 * 8. 替换仅由大写英文字母组成的字符串任意位置上的字符给定的次数，求替换后重复字母的最长子串的长度 (lc.424)
 * 9. 求给定数组中，子数组满足不同整数的个数恰好为 K 的数目 (lc.992)
 *
 */
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

struct ListNode {
    int val;
    struct ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

/*
 * 1. 找出两个链表的第一个公共结点
 */
ListNode *FindFirstCommonNode(ListNode *head1, ListNode *head2) {
    if (head1 == nullptr || head2 == nullptr) return nullptr;
    // 下面的算法适用于一个 ListNode* 为 nullptr 的情况，但有额外开销
    ListNode *p1 = head1;
    ListNode *p2 = head2;
    while (p1 != p2) {
        p1 = (p1 == nullptr ? head2 : p1->next);
        p2 = (p2 == nullptr ? head1 : p2->next);
    }
    return p1;
}

/*
 * 2. 给定一个链表，返回链表开始入环的第一个节点。 如果链表无环，则返回 nullptr
 */
ListNode *detectCycle(ListNode *head) {
    ListNode *p1 = head, *p2 = head;
    while (p1) {
        if (p1->next == nullptr) return nullptr;
        p1 = p1->next->next;
        p2 = p2->next;
        if (p1 == p2) break;
    }

    p2 = head;
    while (p1 != p2) {
        p1 = p1->next;
        p2 = p2->next;
    }
    return p2;
}

/*
 * 3. 输入一个链表，输出该链表中倒数第k个结点
 */
ListNode *FindKthToTail(ListNode *head, int k) {
    if (k <= 0) return nullptr;
    ListNode *p1 = head, *p2 = head;
    while (k > 0 && p1) {
        k--;
        p1 = p1->next;
    }
    if (k > 0) return nullptr;
    while (p1) {
        p1 = p1->next;
        p2 = p2->next;
    }
    return p2;
}

/*
 * 4. 在递增数组 nums 中查找两个数，使得他们的和正好是 sum
 */
vector<int> FindNumbersWithSum(vector<int>& nums, int sum) {
    if (nums.size() < 2) return vector<int>();
    int i = 0, j = nums.size() - 1;
    while (i < j) {
        if (nums[i] + nums[j] == sum)
            return vector<int>{nums[i], nums[j]};
        else if (nums[i] + nums[j] > sum)
            j--;
        else
            i++;
    }
    return vector<int>();
}

/*
 * 5. 给定一个数组和滑动窗口的大小，找出所有滑动窗口里数值的最大值
 */
vector<int> maxSlidingWindow(const vector<int>& nums, int size) {
    if (size == 0 || size > nums.size()) return vector<int>();
    vector<int> ans;

    multiset<int> ms;
    for (int i = 0; i < size; i++) ms.insert(nums[i]);
    ans.emplace_back(*ms.rbegin());
    int i = 0, j = size;
    while (j < nums.size()) {
        ms.erase(ms.find(nums[i++]));
        ms.insert(nums[j++]);
        ans.emplace_back(*ms.rbegin());
    }

    return ans;
}

/*
 * 5. 同上，方法二：单调递减队列
 */
vector<int> maxSlidingWindow2(const vector<int>& nums, int size) {
    if (size == 0 || size > nums.size()) return vector<int>();
    vector<int> ans;
    deque<int> q;
    for (int i = 0; i < nums.size(); i++) {
        if (i >= size && q.front() == nums[i - size]) q.pop_front();
        while (!q.empty() && nums[i] > q.back()) q.pop_back();
        q.push_back(nums[i]);
        if (i + 1 >= size) ans.emplace_back(q.front());
    }
    return ans;
}

/*
 * 6. 串联所有单词的子串
 * 给定一个字符串 s 和一些长度相同的单词 words。找出 s 中恰好可以由 words 中所有单词串联形成的子串的起始位置。
 * 注意子串要与 words 中的单词完全匹配，中间不能有其他字符，但不需要考虑 words 中单词串联的顺序。
 */
template <typename T>
struct counter {
    unordered_map<T, int> data;

    void insert(const T& e) {
        if (data.find(e) == data.end())
            data[e] = 1;
        else
            data[e]++;
    }

    void erase(const T& e) {
        auto iter = data.find(e);
        if (iter != data.end()) {
            if (iter->second > 1)
                iter->second--;
            else
                data.erase(iter);
        }
    }

    bool operator==(const counter& other) { return data == other.data; }
};

vector<int> findSubstring(string s, vector<string>& words) {
    vector<int> ans;
    if (words.empty()) return ans;
    int len = s.length(), m = words.size(), n = words[0].length(), mn = m * n;

    counter<string> count;
    for (int i = 0; i < m; i++) count.insert(words[i]);
    for (int i = 0; i < n; i++) {
        counter<string> count2;
        for (int j = 0; j < m; j++) count2.insert(string(s, i + n * j, n));

        for (int j = i; j <= len - mn; j += n) {
            if (count2 == count) ans.emplace_back(j);
            count2.erase(string(s, j, n));
            count2.insert(string(s, j + mn, n));
        }
    }

    return ans;
}

/*
 * 7. 给定一个字符串 s，找出其中不含有重复字符的最长子串的长度
 * s 由英文字母、数字、符号和空格组成
 */
int lengthOfLongestSubstring(string s) {
    int chars[256] = {};
    int ans = 0, i = 0, j = 0, len = s.length();
    while (j < len) {
        while (j < len && chars[s[j]] == 0) chars[s[j++]] = 1;
        ans = max(ans, j - i);
        while (chars[s[j]] == 1 && i < j) chars[s[i++]] = 0;
    }
    return ans;
}

/*
 * 8. 将仅由大写英文字母组成的字符串任意位置上的字符替换成另外的字符，最多替换 k 次，求替换后重复字母的最长子串的长度
 */
int characterReplacement(string s, int k) {
    int i = 0, j = 0, len = s.length();
    int chars[26] = {};
    int m = 0;
    while (j < len) {
        chars[s[j] - 'A']++;
        m = max(m, chars[s[j] - 'A']);
        if (m + k < j - i + 1) chars[s[i++] - 'A']--;
        j++;
    }
    return j - i;
}

/*
 * 9. 给定一个正整数数组 A，求 A 的子数组中满足不同整数的个数恰好为 K 的数目
 */
int subarraysWithKDistinct(vector<int>& A, int K) {
    int ans = 0, i = 0, j = 0, len = A.size();
    counter<int> count;
    while (j < len) {
        count.insert(A[j]);
        if (count.data.size() < K)
            j++;
        else if (count.data.size() == K) {
            ans++;
            j++;
            if (j == len) {
                count.erase(A[i++]);
                while (count.data.size() >= K) count.erase(A[--j]);
            }
        } else {
            count.erase(A[i++]);
            while (count.data.size() >= K) count.erase(A[j--]);
            j++;
        }
    }

    return ans;
}

int main() {}
