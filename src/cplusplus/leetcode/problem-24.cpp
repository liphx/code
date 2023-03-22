// https://leetcode.cn/problems/swap-nodes-in-pairs/
// 给定一个链表，两两交换其中相邻的节点，并返回交换后的链表。
// 你不能只是单纯的改变节点内部的值，而是需要实际的进行节点交换。

#include "leetcode.h"

class Solution {
public:
    ListNode *swapPairs(ListNode *head) {
        ListNode ret, *node = &ret;
        ret.next = head;
        while (node->next && node->next->next) {
            ListNode *tmp1 = node->next;
            ListNode *tmp2 = node->next->next;
            tmp1->next = tmp2->next;
            tmp2->next = tmp1;
            node->next = tmp2;
            node = tmp1;
        }
        return ret.next;
    }
};

int main() {
    ListNode *head = toList({1, 2, 3, 4});
    cout << Solution().swapPairs(head) << endl;  // [2, 1, 4, 3]
}
