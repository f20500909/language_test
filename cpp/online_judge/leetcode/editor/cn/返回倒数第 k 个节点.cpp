//实现一种算法，找出单向链表中倒数第 k 个节点。返回该节点的值。 
//
// 注意：本题相对原题稍作改动 
//
// 示例： 
//
// 输入： 1->2->3->4->5 和 k = 2
//输出： 4 
//
// 说明： 
//
// 给定的 k 保证是有效的。 
// Related Topics 链表 双指针


//leetcode submit region begin(Prohibit modification and deletion)
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    int kthToLast(ListNode *head, int k) {
        if (!head) return -1;
        if (k < 0) return -1;
        int len = 0;
        ListNode *cur = head;
        while(cur) {
            cur = cur->next;
            len++;
        }
        if (k >= len) return head->val;
        int step = len - k;
        cur = head;
        while(step--)
        {
            cur = cur->next;
        }
        return cur->val;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
