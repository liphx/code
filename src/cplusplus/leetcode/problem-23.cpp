// https://leetcode.cn/problems/merge-k-sorted-lists/
// 给你一个链表数组，每个链表都已经按升序排列。
// 请你将所有链表合并到一个升序链表中，返回合并后的链表。

#include "leetcode.h"

class Solution {
public:
    // 合并两个链表，problem-21.cpp
    ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
        ListNode ret;
        ListNode *node = &ret;
        while (l1 != nullptr && l2 != nullptr) {
            if (l1->val < l2->val) {
                node->next = l1;
                l1 = l1->next;
            } else {
                node->next = l2;
                l2 = l2->next;
            }
            node = node->next;
        }
        node->next = l1 ? l1 : l2;
        return ret.next;
    }

    ListNode *mergeKLists(vector<ListNode *>& lists) {
        int size = lists.size();
        if (size == 0) {
            return nullptr;
        }

        ListNode *ret = lists[0];
        for (int i = 1; i < size; i++) {
            ret = mergeTwoLists(ret, lists[i]);
        }

        return ret;
    }
};

int main() {
    ListNode *a = toList({1, 4, 5});
    ListNode *b = toList({1, 3, 4});
    ListNode *c = toList({2, 6});
    vector<ListNode *> lists = {a, b, c};
    cout << Solution().mergeKLists(lists) << endl;  // [1, 1, 2, 3, 4, 4, 5, 6]
}
