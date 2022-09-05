//输入一个矩阵，按照从外向里以顺时针的顺序依次打印出每一个数字。 
//
// 
//
// 示例 1： 
//
// 输入：matrix = [[1,2,3],[4,5,6],[7,8,9]]
//输出：[1,2,3,6,9,8,7,4,5]
// 
//
// 示例 2： 
//
// 输入：matrix = [[1,2,3,4],[5,6,7,8],[9,10,11,12]]
//输出：[1,2,3,4,8,12,11,10,9,5,6,7]
// 
//
// 
//
// 限制： 
//
// 
// 0 <= matrix.length <= 100 
// 0 <= matrix[i].length <= 100 
// 
//
// 注意：本题与主站 54 题相同：https://leetcode-cn.com/problems/spiral-matrix/ 
// Related Topics 数组


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    vector<int> spiralOrder(vector <vector<int>> &matrix) {
        vector<int> res;
        int col = matrix.size();
        if(col==0) return res;
        int row = matrix[0].size();
        //  t_l           t_r
        //
        //  b_l           b_r

        int top = 0;
        int bot = col - 1;
        int left = 0;
        int right = row - 1;
        int round = ((min(col, row) + 1) >> 1);

        int i;
        int j;
        for (int r = 0; r < round; r++) {

            i = top;
            j = left;
            while (j <= right&&j<row) {
                res.push_back(matrix[i][j]);
                j++;
            }

            i = top+1;
            j=right;
            while (i < bot&&i<col) {
                res.push_back(matrix[i][j]);
                i++;
            }


            i = bot;
            j= right;
            while(j>=left&&j>=0&&top!=bot){ //避免最后一圈出错
                res.push_back(matrix[i][j]);
                j--;
            }


            i = bot-1;
            j = left;
            while(i>top&&i>=0&&left!=right){ //避免最后一圈出错
                res.push_back(matrix[i][j]);
                i--;
            }


            top++;
            bot--;
            left++;
            right--;
        }
        return res;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
























