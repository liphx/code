// https://leetcode.cn/problems/insertion-sort-list/

/*
 * 对链表进行插入排序。
 * 插入排序算法：
 *   插入排序是迭代的，每次只移动一个元素，直到所有元素可以形成一个有序的输出列表。
 *   每次迭代中，插入排序只从输入数据中移除一个待排序的元素，找到它在序列中适当的位置，并将其插入。
 *   重复直到所有输入数据插入完为止。
 */

#include "leetcode.h"

class Solution {
public:
    ListNode *insertionSortList(ListNode *head) {
        ListNode ans(0);
        while (head) {
            ListNode *next = head->next;
            ListNode *pre = &ans;
            ListNode *cur = ans.next;
            while (cur && cur->val < head->val) {
                pre = cur;
                cur = cur->next;
            }
            head->next = cur;
            pre->next = head;
            head = next;
        }
        return ans.next;
    }
};

int main() {
    ListNode *head = toList({3, 4, 1});
    ListNode *ans = Solution().insertionSortList(head);
    cout << ans << endl;  // [1, 3, 4]
}
