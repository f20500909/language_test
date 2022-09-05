//编写一个函数，检查输入的链表是否是回文的。 
//
// 
//
// 示例 1： 
//
// 输入： 1->2
//输出： false 
// 
//
// 示例 2： 
//
// 输入： 1->2->2->1
//输出： true 
// 
//
// 
//
// 进阶： 
//你能否用 O(n) 时间复杂度和 O(1) 空间复杂度解决此题？ 
// Related Topics 链表


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
    ListNode *reverse(ListNode *node) {
        ListNode *pre = nullptr;
        ListNode *next = nullptr;
        ListNode *cur = node;
        while (cur) {
            if (!cur->next) {
                cur->next = pre;
                return cur;
            }
            next = cur->next;
            cur->next = pre;
            cur = next;
        }
        return cur;
    }


    bool isPalindrome(ListNode *head) {
        int len = 0;
        ListNode *cur = head;
        while (cur) {
            len++;
            cur = cur->next;
        }

        int reverseNum = (len>>1);
        cur = head;

        while(reverseNum--){
            cur=cur->next;
        }
        ListNode* temp =cur;

        cur -> next = reverse(cur);

        temp =temp->next;
        cur =head;
        while(temp){
            if(cur->val!=temp) {return false;}
        }
        return true;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
