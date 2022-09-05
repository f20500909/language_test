//给定一个二叉树，原地将它展开为链表。 
//
// 例如，给定二叉树 
//
//     1
//   / \
//  2   5
// / \   \
//3   4   6 
//
// 将其展开为： 
//
// 1
// \
//  2
//   \
//    3
//     \
//      4
//       \
//        5
//         \
//          6 
// Related Topics 树 深度优先搜索


//leetcode submit region begin(Prohibit modification and deletion)
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    void flatten(TreeNode *root) {
        if (!root) return;
        TreeNode* cur = root;
        TreeNode* pre = nullptr;
        stack<TreeNode*> s;
        while(!s.empty()||cur) {
            if (cur) {
                s.push(cur);
                cur=cur->left;
            }else{
                cur = s.top();
                s.pop();
                if(!pre){
                    pre = cur;
                }else{
                    pre->right = cur;
//                    cur->left =pre;
                    pre = cur;
                }
                cur = cur->right;
            }
        }

    }
};
//leetcode submit region end(Prohibit modification and deletion)
