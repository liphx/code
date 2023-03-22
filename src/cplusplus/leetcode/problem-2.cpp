// https://leetcode.cn/problems/add-two-numbers/

#include "leetcode.h"

class Solution {
public:
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        ListNode ans, *node = &ans;
        int k = 0;
        while (l1 && l2) {
            ListNode *cur = new ListNode((l1->val + l2->val + k) % 10);
            k = (l1->val + l2->val + k) / 10;
            node->next = cur;
            node = cur;
            l1 = l1->next;
            l2 = l2->next;
        }
        ListNode *l = l1 ? l1 : l2;
        while (l) {
            ListNode *cur = new ListNode((l->val + k) % 10);
            k = (l->val + k) / 10;
            node->next = cur;
            node = cur;
            l = l->next;
        }
        if (k > 0) {
            ListNode *cur = new ListNode(k);
            node->next = cur;
        }
        return ans.next;
    }
};

int main() {
    ListNode *l1 = toList({2, 4, 3});
    ListNode *l2 = toList({5, 6, 4});
    ListNode *ans = Solution().addTwoNumbers(l1, l2);
    cout << ans << endl;  // [7, 0, 8]

    l1 = toList({0});
    l2 = toList({0});
    ans = Solution().addTwoNumbers(l1, l2);
    cout << ans << endl;  // [0]

    l1 = toList({9, 9, 9, 9, 9, 9, 9});
    l2 = toList({9, 9, 9, 9});
    ans = Solution().addTwoNumbers(l1, l2);
    cout << ans << endl;  // [8, 9, 9, 9, 0, 0, 0, 1]
}
