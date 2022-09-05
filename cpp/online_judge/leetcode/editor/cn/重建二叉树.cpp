//输入某二叉树的前序遍历和中序遍历的结果，请重建该二叉树。假设输入的前序遍历和中序遍历的结果中都不含重复的数字。 
//
// 
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
//
// 
//
// 限制： 
//
// 0 <= 节点个数 <= 5000 
//
// 
//
// 注意：本题与主站 105 题重复：https://leetcode-cn.com/problems/construct-binary-tree-from-
//preorder-and-inorder-traversal/ 
// Related Topics 树 递归


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
// 前序遍历 preorder = [3,9,20,15,7]
//中序遍历 inorder = [9,3,15,20,7]
class Solution {
public:
    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
        if (preorder.size() == 0) return nullptr;
        if (preorder.size() != inorder.size()) return nullptr;
        TreeNode *res = nullptr;
        return build(preorder, inorder, 0, inorder.size() - 1, res);
    }

    TreeNode *build(vector<int> &preorder, vector<int> &inorder, int left, int right, TreeNode *&res) {
        if (left >= preorder.size()) return res;
        if (right <= 0) return res;
        if (left > right) return res;
        if (!res) {
            res = new TreeNode(preorder[left]);
        }
        int cur = left;
        while (preorder[left] != inorder[cur]) {
            cur++;
        }
        res->left = build(preorder, inorder, left, cur - 1, res);
        res->right = build(preorder, inorder, cur + 1, right, res);

        return res;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
