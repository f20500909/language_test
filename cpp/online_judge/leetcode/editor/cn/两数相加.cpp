//给出两个 非空 的链表用来表示两个非负的整数。其中，它们各自的位数是按照 逆序 的方式存储的，并且它们的每个节点只能存储 一位 数字。
//
// 如果，我们将这两个数相加起来，则会返回一个新的链表来表示它们的和。
//
// 您可以假设除了数字 0 之外，这两个数都不会以 0 开头。
//
// 示例：
//
// 输入：(2 -> 4 -> 3) + (5 -> 6 -> 4)
//输出：7 -> 0 -> 8
//原因：342 + 465 = 807
//
// Related Topics 链表 数学


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
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        //l1长 l2短
        if (!l1 || !l2) return nullptr;
        ListNode *res = new ListNode(-1);
        ListNode *cur = res;

        ListNode *cur1 = l1;
        ListNode *cur2 = l2;


        int flag = 0;
        while (cur1 && cur2) {
            ListNode *temp = new ListNode(cur1->val + cur2->val);
            if (flag) {
                temp->val += flag;
                flag = 0;
            }
            if (temp->val >= 10) {
                temp->val %= 10;
                flag = 1;
            }
            cur->next = temp;
            cur = cur->next;

            cur1 = cur1->next;
            cur2 = cur2->next;
        }

        while (cur1) {
            ListNode *temp = new ListNode(cur1->val + flag);
            flag = 0;
            if (temp->val >= 10) {
                temp->val %= 10;
                flag = 1;
            }


            cur->next = temp;
            cur=cur->next;
            cur1 = cur1->next;
        }
        while (cur2) {

            ListNode *temp = new ListNode(cur2->val + flag);
            flag = 0;
            if (temp->val >= 10) {
                temp->val %= 10;
                flag = 1;
            }

            cur->next = temp;
            cur=cur->next;
            cur2 = cur2->next;
        }
        if(flag){
            ListNode *temp = new ListNode(flag);

            cur->next = temp;
            cur=cur->next;
        }


        return res->next;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
















