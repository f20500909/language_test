//给定一个二叉搜索树（Binary Search Tree），把它转换成为累加树（Greater Tree)，使得每个节点的值是原来的节点值加上所有大于它的节
//点值之和。 
//
// 
//
// 例如： 
//
// 输入: 原始二叉搜索树:
//              5
//            /   \
//           2     13
//
//输出: 转换为累加树:
//             18
//            /   \
//          20     13
// 
//
// 
//
// 注意：本题和 1038: https://leetcode-cn.com/problems/binary-search-tree-to-greater-s
//um-tree/ 相同 
// Related Topics 树


//leetcode submit region begin(Prohibit modification and deletion)
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:

    TreeNode* convertBST(TreeNode* root) {
        int sum = 0;
        helper(root,sum);
        return root;
    }
    void helper(TreeNode* root,int& sum){
        if(!root) return ;
        helper(root->right,sum);
        root->val+= sum;
        sum =root->val;
        helper(root->left,sum);
    }

    TreeNode* convertBST2(TreeNode* root) {
        //中序遍历一次，得到由小到大的栈， 反转栈 然后再次遍历，进行累加。
        //中序遍历  得到有序栈 栈底小  栈顶大
        if(!root) return nullptr;
        stack<TreeNode*> s; //遍历栈
        stack<TreeNode*> data;//数据栈
        TreeNode* cur =root;
        while(cur||!s.empty()){
            if(cur){
                s.push(cur);
                cur=cur->left;
            }else{
                TreeNode* cur = s.top();
                s.pop();
                data.push(cur);
                cur =cur->right;
            }
        }
        //弹出最大值的节点
        TreeNode* pre = data.top();
        data.pop();
        //依次弹出栈
        while(!data.empty()){
            TreeNode* cur = data.top();
            data.pop();
            //累加上一项
            cur->val+=pre->val;
            //更新上一项
            pre = cur;
        }

        return root;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
