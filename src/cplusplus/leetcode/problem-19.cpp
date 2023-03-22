// https://leetcode.cn/problems/remove-nth-node-from-end-of-list/
// 给定一个链表，删除链表的倒数第 n 个节点，并且返回链表的头结点。给定的 n 保证是有效的。

#include "leetcode.h"

class Solution {
public:
    ListNode *removeNthFromEnd(ListNode *head, int n) {
        if (head == nullptr) return nullptr;
        ListNode ans;
        ans.next = head;
        ListNode *tmp1 = &ans, *tmp2 = &ans;
        for (int i = 0; i <= n; i++) {
            tmp1 = tmp1->next;
        }
        while (tmp1) {
            tmp2 = tmp2->next;
            tmp1 = tmp1->next;
        }
        tmp2->next = tmp2->next->next;
        return ans.next;
    }
};

int main() {
    ListNode *head = toList({1, 2, 3, 4, 5});
    ListNode *ans = Solution().removeNthFromEnd(head, 2);
    cout << ans << endl;  // [1, 2, 3, 5]
}
