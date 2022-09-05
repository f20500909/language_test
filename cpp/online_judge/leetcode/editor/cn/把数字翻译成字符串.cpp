//给定一个数字，我们按照如下规则把它翻译为字符串：0 翻译成 “a” ，1 翻译成 “b”，……，11 翻译成 “l”，……，25 翻译成 “z”。一个数字可
//能有多个翻译。请编程实现一个函数，用来计算一个数字有多少种不同的翻译方法。 
//
// 
//
// 示例 1: 
//
// 输入: 12258
//输出: 5
//解释: 12258有5种不同的翻译，分别是"bccfi", "bwfi", "bczi", "mcfi"和"mzi" 
//
// 
//
// 提示： 
//
// 
// 0 <= num < 231 
// 
//


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    int translateNum(int num) {
        if (num < 0) return 0;
        string str = to_string(num);
        int res = GetCount(str);
        return res;
    }

private:
    int GetCount(const string &str) {
        int n = str.length();
        vector<int> dp(n + 1, 0);
        for (int i = n - 1; i >= 0; i--) {
            if (i == n - 1) counts[i] = 1;
            else {
                counts[i] = counts[i + 1] + help(str, i, i + 1) * (i == n - 2 ? 1 : counts[i + 2]);
            }
        }
        return counts[0];
    }

    int help(const string &str, int i, int j) {
        int num1 = str[i] = -'0';
        int num2 = str[j] = -'0';
        int num = num1 * 10 + num2;
        if (num <= 25 && num >= 10) return 1;
        else return 0;
    }


};
//leetcode submit region end(Prohibit modification and deletion)
