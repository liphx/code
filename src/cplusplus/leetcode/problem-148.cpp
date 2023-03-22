// https://leetcode.cn/problems/sort-list/
// 给你链表的头结点head，请将其按升序排列并返回排序后的链表。
// 进阶：你可以在O(nlogn)时间复杂度和常数级空间复杂度下，对链表进行排序吗？

#include "leetcode.h"

class Solution {
public:
    ListNode *sortList(ListNode *head) {
        int n = 0;
        ListNode *node = head;
        while (node) {
            n++;
            node = node->next;
        }
        if (n <= 1) return head;
        int mid = n / 2;
        node = head;
        while (mid > 1) {
            node = node->next;
            mid--;
        }
        ListNode *l2 = node->next;
        node->next = nullptr;

        head = sortList(head);
        l2 = sortList(l2);
        return merge(head, l2);
    }

private:
    ListNode *merge(ListNode *l1, ListNode *l2) {
        ListNode ans(0), *pre = &ans;
        while (l1 && l2) {
            if (l1->val < l2->val) {
                pre->next = l1;
                pre = l1;
                l1 = l1->next;
            } else {
                pre->next = l2;
                pre = l2;
                l2 = l2->next;
            }
        }
        pre->next = l1 ?: l2;
        return ans.next;
    }
};

int main() {
    ListNode *head = toList({-1, 5, 3, 4, 0});
    cout << Solution().sortList(head) << endl;  // [-1, 0, 3, 4, 5]
}
