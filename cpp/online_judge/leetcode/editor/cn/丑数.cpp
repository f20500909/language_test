//我们把只包含因子 2、3 和 5 的数称作丑数（Ugly Number）。求按从小到大的顺序的第 n 个丑数。 
//
// 
//
// 示例: 
//
// 输入: n = 10
//输出: 12
//解释: 1, 2, 3, 4, 5, 6, 8, 9, 10, 12 是前 10 个丑数。 
//
// 说明: 
//
// 
// 1 是丑数。 
// n 不超过1690。 
// 
//
// 注意：本题与主站 264 题相同：https://leetcode-cn.com/problems/ugly-number-ii/ 
// Related Topics 数学


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    int nthUglyNumber(int n) {
        vector<int> dp(n + 1, 1);
        int c_2 = 0;
        int c_3 = 0;
        int c_5 = 0;
        for (int i = 1; i < n; i++) {
            dp[i] = minVal(dp[c_2] * 2, dp[c_3] * 3, dp[c_5] * 5);
            if (dp[i] / dp[c_2] == 2) c_2++;
            if (dp[i] / dp[c_3] == 3) c_3++;
            if (dp[i] / dp[c_5] == 5) c_5++;
        }
        return dp[n - 1];

    }

    int minVal(int a, int b, int c) {
        if (a > b) {
            return min(b, c);
        } else {
            return min(a, c);
        }
    }

};
//leetcode submit region end(Prohibit modification and deletion)
