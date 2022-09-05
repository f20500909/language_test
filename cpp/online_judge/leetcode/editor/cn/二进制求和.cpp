//给你两个二进制字符串，返回它们的和（用二进制表示）。 
//
// 输入为 非空 字符串且只包含数字 1 和 0。 
//
// 
//
// 示例 1: 
//
// 输入: a = "11", b = "1"
//输出: "100" 
//
// 示例 2: 
//
// 输入: a = "1010", b = "1011"
//输出: "10101" 
//
// 
//
// 提示： 
//
// 
// 每个字符串仅由字符 '0' 或 '1' 组成。 
// 1 <= a.length, b.length <= 10^4 
// 字符串如果不是 "0" ，就都不含前导零。 
// 
// Related Topics 数学 字符串


//leetcode submit region begin(Prohibit modification and deletion)
//补上零的方法
class Solution {
public:
    string addBinary(string a, string b) {
        string res="";
        int carry = 0;
        int m = a.size() - 1;
        int n = b.size() - 1;
        while (m >= 0 || n >= 0) {
            int q=(m>=0)?a[m--]-'0':0;
            int p=(n>=0)?b[n--]-'0':0;
            int k = q+p+carry;
            res =to_string(k%2)+res;
            carry=k/2;
        }
        if(1==carry) res="1" +res;
        return res;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
