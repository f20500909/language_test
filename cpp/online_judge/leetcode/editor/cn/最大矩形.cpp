//给定一个仅包含 0 和 1 的二维二进制矩阵，找出只包含 1 的最大矩形，并返回其面积。 
//
// 示例: 
//
// 输入:
//[
//  ["1","0","1","0","0"],
//  ["1","0","1","1","1"],
//  ["1","1","1","1","1"],
//  ["1","0","0","1","0"]
//]
//输出: 6 
// Related Topics 栈 数组 哈希表 动态规划


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    int maximalRectangle(vector <vector<char>> &matrix) {
        int res = 0;
        vector<int> dp(matrix);
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; i < matrix[0].size(); j++) {
                dp[j] = (matrix[i][j] == '0' ? 0 : (1 + dp[j]));
            }
            res = max(res, largestRectangleArea(dp));
        }
        return res;

    }

    int  largestRectangleArea(vector<int>& dp){
        if(dp.empty()) return 0;
        stack<int> s;
        dp.push_back(0);
        int res =0;
        for(int i=0;i<dp.size();i++){
            while(!s.empty()&&)
        }

    }

};
//leetcode submit region end(Prohibit modification and deletion)
