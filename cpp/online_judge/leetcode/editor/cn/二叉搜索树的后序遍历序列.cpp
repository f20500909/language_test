//输入一个整数数组，判断该数组是不是某二叉搜索树的后序遍历结果。如果是则返回 true，否则返回 false。假设输入的数组的任意两个数字都互不相同。 
//
// 
//
// 参考以下这颗二叉搜索树： 
//
//      5
//    / \
//   2   6
//  / \
// 1   3 
//
// 示例 1： 
//
// 输入: [1,6,3,2,5]
//输出: false 
//
// 示例 2： 
//
// 输入: [1,3,2,6,5]
//输出: true 
//
// 
//
// 提示： 
//
// 
// 数组长度 <= 1000 
// 
//


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    bool verifyPostorder(vector<int> &postorder) {
        if (postorder.size() == 0) return true;
        return verify(postorder, 0, postorder.size() - 1);

    }

    bool verify(vector<int> &data, int left, int right) {
        if (left >= right) return true;
        int val = data[right];
        int cur = left;
        while (data[cur] < val) {
            cur++;
        }
        int mid = cur;
        while (cur++ < right) {
            if (data[cur] < val) {
                return false;
            }
        }
        return verify(data, left, mid - 1) && verify(data, mid, right - 1);
    }
};




//leetcode submit region end(Prohibit modification and deletion)
