#ifndef LEETCODE_H_
#define LEETCODE_H_

#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

// begin ListNode

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

inline ListNode *toList(const vector<int>& vc) {
    ListNode ans(0), *node = &ans;
    for (int x : vc) {
        node->next = new ListNode(x);
        node = node->next;
    }
    return ans.next;
}

inline ostream& operator<<(ostream& o, ListNode *head) {
    o << "[";
    string sep;
    while (head) {
        o << sep << head->val;
        sep = ", ";
        head = head->next;
    }
    o << "]";
    return o;
}

// end ListNode

// begin TreeNode

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// end TreeNode

template <typename T>
ostream& operator<<(ostream& o, const vector<T>& vc) {
    o << "[";
    string sep;
    for (const auto& x : vc) {
        o << sep << x;
        sep = ", ";
    }
    o << "]";
    return o;
}

#endif  // LEETCODE_H_
