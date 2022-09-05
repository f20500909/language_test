//输入一个整数 n ，求1～n这n个整数的十进制表示中1出现的次数。 
//
// 例如，输入12，1～12这些整数中包含1 的数字有1、10、11和12，1一共出现了5次。 
//
// 
//
// 示例 1： 
//
// 输入：n = 12
//输出：5
// 
//
// 示例 2： 
//
// 输入：n = 13
//输出：6 
//
// 
//
// 限制： 
//
// 
// 1 <= n < 2^31 
// 
//
// 注意：本题与主站 233 题相同：https://leetcode-cn.com/problems/number-of-digit-one/ 
// Related Topics 数学


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    int countDigitOne(int n) {
        if (n < 0) return 0;
        int i = 1;
        int high = n;
        int cnt = 0;
        while (high != 0) {
            high = n / pow(10, i);//high表示当前位的高位;
            int temp = n / pow(10, i - 1);
            int cur = temp % 10;
            int low = n - temp * pow(10, i - 1);
            if (cur > 0) {
                cur += high * pow(10, i - 1);
            } else if (cur < 0) {
                cnt += (high - 1) * pow(10, i - 1);

            } else {
                cnt += (high) * pow(10, i - 1);
                cnt += (low + 1);
            }
            i++;
        }
        return cnt;

    }
};
//leetcode submit region end(Prohibit modification and deletion)
