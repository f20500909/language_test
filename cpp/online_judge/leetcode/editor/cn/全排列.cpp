//给定一个 没有重复 数字的序列，返回其所有可能的全排列。 
//
// 示例: 
//
// 输入: [1,2,3]
//输出:
//[
//  [1,2,3],
//  [1,3,2],
//  [2,1,3],
//  [2,3,1],
//  [3,1,2],
//  [3,2,1]
//] 
// Related Topics 回溯算法


//leetcode submit region begin(Prohibit modification and deletion)
class Solution {
public:
    vector <vector<int>> permute(vector<int> &nums) {
        vector <vector<int>> res;
        BT(res, nums, 0);
        return res;
    }

    void BT(vector<vector <int>> &res,vector<int>& nums, int start){

        if (start == nums.size()){
            res.push_back(nums);
            return;
        }
        else{
            for (int i=start;i<nums.size();i++){
                //交换
                swap(nums[i],nums[start]);
                BT(res,nums,start+1);
                //换回去 保证是原始的排列
                swap(nums[i],nums[start]);
            }
        }
    }


};
//leetcode submit region end(Prohibit modification and deletion)









