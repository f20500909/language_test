//在未排序的数组中找到第 k 个最大的元素。请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素。 
//
// 示例 1: 
//
// 输入: [3,2,1,5,6,4] 和 k = 2
//输出: 5
// 
//
// 示例 2: 
//
// 输入: [3,2,3,1,2,4,5,5,6] 和 k = 4
//输出: 4 
//
// 说明: 
//
// 你可以假设 k 总是有效的，且 1 ≤ k ≤ 数组的长度。 
// Related Topics 堆 分治算法


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    int partition(vector<int> &nums, int left, int right) {
        int val = nums[right];
        while (left < right) {
            while (left < right && nums[left] <= val) {
                left++;
            }
            nums[right] = nums[left];
            while (left < right && nums[right] >= val) {
                right--;
            }
            nums[left] = nums[right];
        }
        nums[left] = val;
        return left;
    }

    int findKthLargest(vector<int> &nums, int k) {
        if (nums.size() == 0) return -1;
        if (nums.size() < k) return -1;
        //进行转换，第k大，变为按序过程中的第k个
        k = nums.size() - k;
        int left = 0;
        int right = nums.size() - 1;
        int cur = partition(nums, left, right);
        while (cur != k) {
            if (cur > k) {
                cur = partition(nums, left, cur - 1);
            } else {
                cur = partition(nums, cur + 1, right);
            }

        }
        return nums[k];

    }

};















//leetcode submit region end(Prohibit modification and deletion)
