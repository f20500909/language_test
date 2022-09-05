#include <iostream>
#include <string>

using namespace std;

class Solution {
    void reverse(string &s, int left, int right) {
        if (left < 0 || right >= s.size()) return;
        while (left < right) {
            swap(s[left++], s[right--]);
        }
    }

public:
    string reverseWords(string s) {
        if (s.size() <= 0) return s;
        string rm_s = "";
        int i = 0;
        while (s[i] == ' ' && i < s.size()) {
            i++;
        }

        // 修空格
        while (i < s.size()) {
            if (s[i] == ' ') {
                while (i + 1 < s.size() && s[i + 1] == ' ') {
                    i++;
                }
            }
            rm_s += s[i];
            i++;
        }

        //修最后一个空格
        if (rm_s[rm_s.size() - 1] == ' ') {
            rm_s = rm_s.substr(0, rm_s.size()-1);
        }


        //修空格结束 首尾都没有空格  中间最多有一个空格

        int left = 0;
        int right = -1;
        for (int i = 0; i < rm_s.size(); i++) {
            if (rm_s[i] == ' ') {
                right = i - 1;
                reverse(rm_s, left, right);
                left = right + 2;
            }
            if (i == rm_s.size() - 1) {
                reverse(rm_s, left, rm_s.size() - 1);
            }
        }


        reverse(rm_s, 0, rm_s.size() - 1);
        return rm_s;
    }
};


int main() {

    Solution s;
    cout <<"||"<<s.reverseWords("  hell o world!   ")<<"||"<<endl;

}


