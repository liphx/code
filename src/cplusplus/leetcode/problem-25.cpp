// https://leetcode.cn/problems/reverse-nodes-in-k-group/
// 给你一个链表，每k个节点一组进行翻转，请你返回翻转后的链表。
// k是一个正整数，它的值小于或等于链表的长度。
// 如果节点总数不是k的整数倍，那么请将最后剩余的节点保持原有顺序。
// 你的算法只能使用常数的额外空间。
// 你不能只是单纯的改变节点内部的值，而是需要实际进行节点交换。

#include "leetcode.h"

class Solution {
public:
    ListNode *reverseKGroup(ListNode *head, int k) {
        ListNode ret, *node = &ret;
        ret.next = head;
        ListNode *nodes[k];  // ListNode**, 每次读取k节点
        while (true) {
            for (int i = 0; i < k; i++) {
                if (head == nullptr) {
                    return ret.next;
                }
                nodes[i] = head;
                head = head->next;
            }
            node->next = nodes[k - 1];
            for (int i = 0; i < k - 1; i++) {
                nodes[k - 1 - i]->next = nodes[k - 2 - i];
            }
            nodes[0]->next = head;
            node = nodes[0];
        }
    }
};

int main() {
    ListNode *head = toList({1, 2, 3, 4, 5});
    cout << Solution().reverseKGroup(head, 2) << endl;  // [2, 1, 4, 3, 5]
}
