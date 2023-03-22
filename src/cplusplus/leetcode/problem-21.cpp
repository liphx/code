// https://leetcode.cn/problems/merge-two-sorted-lists/
// 将两个升序链表合并为一个新的升序链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。

#include "leetcode.h"

class Solution {
public:
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
};

int main() {
    ListNode *a = toList({1, 2, 4});
    ListNode *b = toList({1, 3, 4});
    ListNode *ans = Solution().mergeTwoLists(a, b);
    cout << ans << endl;  // [1, 1, 2, 3, 4, 4]
}
