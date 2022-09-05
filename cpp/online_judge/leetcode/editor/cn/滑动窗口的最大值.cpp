//给定一个数组 nums 和滑动窗口的大小 k，请找出所有滑动窗口里的最大值。 
//
// 示例: 
//
// 输入: nums = [1,3,-1,-3,5,3,6,7], 和 k = 3
//输出: [3,3,5,5,6,7] 
//解释: 
//
//  滑动窗口的位置                最大值
//---------------               -----
//[1  3  -1] -3  5  3  6  7       3
// 1 [3  -1  -3] 5  3  6  7       3
// 1  3 [-1  -3  5] 3  6  7       5
// 1  3  -1 [-3  5  3] 6  7       5
// 1  3  -1  -3 [5  3  6] 7       6
// 1  3  -1  -3  5 [3  6  7]      7 
//
// 
//
// 提示： 
//
// 你可以假设 k 总是有效的，在输入数组不为空的情况下，1 ≤ k ≤ 输入数组的大小。 
//
// 注意：本题与主站 239 题相同：https://leetcode-cn.com/problems/sliding-window-maximum/ 
// Related Topics 栈 Sliding Window


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    //大顶堆，储存大元素
    void heapify(vector<int> &data, int len, int index) {
        int left = (index >> 1) + 1;
        int right = (index >> 1) + 2;
        int maxId = index;
        if (left < len && data[left] > data[maxId]) maxId = left;
        if (right < len && data[right] > data[maxId]) maxId = right;
        if(maxId!=index){
            swap(data[maxId],data[index]);
            heapify(data,len,maxId);
        }
    }

    vector<int> maxSlidingWindow(vector<int> &nums, int k) {
        vector<int> res = {};
        if (nums.size() == 0) return res;
        if (nums.size() == 1) return {nums[0]};
        if (k <= 1) return {nums[0]};
        vector<int> heap(k, 0);
        for (int i = 0; i < heap.size(); i++) {
            heap[i] = nums[i];
        }
        for (int i = k >> 1; i >= 0; i--) {
            heapify(heap, k, i);
        }
        res.push_back(heap[0]);
        for (int i = k; i < nums.size(); i++) {
            if (heap[0] <= nums[i]) {
                heap[0] = nums[i];
            }
            res.push_back(heap[0]);
        }
        return res;
    }
};
//leetcode submit region end(Prohibit modification and deletion)
