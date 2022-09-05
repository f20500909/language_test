//给定两个大小为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。 
//
// 请你找出这两个正序数组的中位数，并且要求算法的时间复杂度为 O(log(m + n))。 
//
// 你可以假设 nums1 和 nums2 不会同时为空。 
//
// 
//
// 示例 1: 
//
// nums1 = [1, 3]
//nums2 = [2]
//
//则中位数是 2.0
//
//
// 示例 2: 
//
// nums1 = [1, 2]
//nums2 = [3, 4]
//
//则中位数是 (2 + 3)/2 = 2.5
// 
// Related Topics 数组 二分查找 分治算法


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    double findMedianSortedArrays(vector<int> &nums1, vector<int> &nums2) {
        int m = nums1.size();
        int n = nums2.size();
        int left = (m + n + 1) >> 1;
        int right = (m + n + 2) >> 1;
        double data1 = findKth(nums1, 0, nums2, 0, left);
        double data2 = findKth(nums1, 0, nums2, 0, right);
        return (data1 + data2) / 2.0;
    }

    //找到两个数组中的第k个元素
    int findKth(vector<int> &nums1, int i, vector<int> &nums2, int j, int k) {
        //递归出口 i或j 越界时直接返回
        if (i >= nums1.size()) return nums2[j + k - 1];
        if (j >= nums2.size()) return nums1[i + k - 1];
        //如果k==1,也就是第一个元素，返回两者中的最小值
        if (k == 1) return min(nums1[i], nums2[j]);

        int midVal1 = (i + k / 2 - 1 < nums1.size()) ? nums1[i + k / 2 - 1] : INT_MAX;
        int midVal2 = (j + k / 2 - 1 < nums2.size()) ? nums2[j + k / 2 - 1] : INT_MAX;
        if (midVal1 < midVal2) {
            return findKth(nums1, i + k / 2, nums2, j, k - k / 2);
        } else {
            return findKth(nums1, i, nums2, j + k / 2, k - k / 2);
        }

    }
};
//leetcode submit region end(Prohibit modification and deletion)
