//合并 k 个排序链表，返回合并后的排序链表。请分析和描述算法的复杂度。 
//
// 示例: 
//
// 输入:
//[
//  1->4->5,
//  1->3->4,
//  2->6
//]
//输出: 1->1->2->3->4->4->5->6 
// Related Topics 堆 链表 分治算法


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
    ListNode* mergeTwoLists(ListNode* l1,ListNode* l2){
        if(l1==nullptr) return l2;
        if(l2==nullptr) return l1;
        if(l1->val<l2->val){
            l1->next = mergeTwoLists(l1->next,l2);
            return l1;
        }else{
            l2->next = mergeTwoLists(l1,l2->next);
            return l2;
        }
    }

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode* head = new ListNode(-1);
        ListNode* cur =  head;
        for(int i=0;i<lists.size();i++){
            cur->next=mergeTwoLists(cur->next,lists[i]);
        }
        return head->next;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
