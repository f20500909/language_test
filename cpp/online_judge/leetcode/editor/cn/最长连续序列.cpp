//给定一个未排序的整数数组，找出最长连续序列的长度。 
//
// 要求算法的时间复杂度为 O(n)。 
//
// 示例: 
//
// 输入: [100, 4, 200, 1, 3, 2]
//输出: 4
//解释: 最长连续序列是 [1, 2, 3, 4]。它的长度为 4。 
// Related Topics 并查集 数组


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    int longestConsecutive(vector<int> &nums) {
        int res = 0;
        unordered_map<int, int> m;
        for (int i = 0; i < nums.size(); i++) {
            int cur = nums[i];
            if (m.count(cur)) continue;
            int left = m.count(cur - 1) ? m[cur - 1] : 0;
            int right = m.count(cur + 1) ? m[cur + 1] : 0;
            int sum = left + right + 1;
            m[cur] = sum;
            res = max(res, sum);
            m[cur - left] = sum;
            m[cur + right] = sum;
        }
        return res;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
