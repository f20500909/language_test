//在数组中的两个数字，如果前面一个数字大于后面的数字，则这两个数字组成一个逆序对。输入一个数组，求出这个数组中的逆序对的总数。 
//
// 
//
// 示例 1: 
//
// 输入: [7,5,6,4]
//输出: 5 
//
// 
//
// 限制： 
//
// 0 <= 数组长度 <= 50000 
//


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    int reversePairs(vector<int> &data) {
        if (data.size() == 0) return 0;
        vector<int> copy(data);
        return reversePairs(data, copy, 0, data.size() - 1) % 1000000007;
    }

    long long reversePairs(vector<int> &data, vector<int> &copy, int left, int right) {
        if (left == right) {
            copy[left] = data[right];
            return 0;
        }
        int mid = left + ((right - left) >> 1);
        long long leftCount = reversePairs(copy, data, left, mid);
        long long rightCount = reversePairs(copy, data, mid + 1, right);

        int i = mid;
        int j = right;
        int indexCopy = right;
        long long count = 0;
        while (i >= left && j >= mid + 1) {
            if (data[i] > data[j]) {
                copy[indexCopy--] = data[i]--;
                count += j - mid;
            } else {
                copy[indexCopy--] = data[j--];
            }
        }
        for (; i >= left; i--) copy[indexCopy--] = data[i];
        for (; i >= mid + 1; j--) copy[indexCopy--] = data[i];
        return leftCount + rightCount + count;
    }

};
//leetcode submit region end(Prohibit modification and deletion)
