//根据一棵树的前序遍历与中序遍历构造二叉树。 
//
// 注意: 
//你可以假设树中没有重复的元素。 
//
// 例如，给出 
//
// 前序遍历 preorder = [3,9,20,15,7]
//中序遍历 inorder = [9,3,15,20,7] 
//
// 返回如下的二叉树： 
//
//     3
//   / \
//  9  20
//    /  \
//   15   7 
// Related Topics 树 深度优先搜索 数组


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
    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
        if (preorder.size() == 0) return nullptr;
        if (preorder.size() != inorder.size()) return nullptr;
        TreeNode *root = buildTree(preorder, inorder, 0, preorder.size() - 1);
        return root;
    }
//前序遍历 preorder = [3,9,20,15,7]
//中序遍历 inorder = [9,3,15,20,7]

    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder, int left, int right) {
        TreeNode *cur = new TreeNode(preorder[left]);


        TreeNode *left = buildTree(preorder, inorder, 0, 0);

        TreeNode *right = buildTree(preorder, inorder, 0, 0);
        cur->left = left;
        cur->left = left;
        return cur;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
