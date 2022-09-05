//请实现一个函数，把字符串 s 中的每个空格替换成"%20"。 
//
// 
//
// 示例 1： 
//
// 输入：s = "We are happy."
//输出："We%20are%20happy." 
//
// 
//
// 限制： 
//
// 0 <= s 的长度 <= 10000 
//


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    string replaceSpace(string s) {
        int spaceLen = 0;
        int len = s.size();
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == ' ') spaceLen++;
        }
        s = s + string(2 * spaceLen, ' ');

        int plusNum = 2 * spaceLen;

        for (int i = len-1; i >=0; i--) {
            int index = plusNum + i;
            if(s[i] !=' '){
                s[index] = s[i];
            }else{
                //如果等于空格
                s[index] = '0';
                s[index-1] = '2';
                s[index-2] = '%';
                plusNum-=2;
            }
        }


        return s;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
