
#include <iostream>
#include <vector>

using namespace std;

void print(const vector<int> &vec) {
    for (auto v:vec) {
        cout << v << " ";
    }
    cout << endl;
}


class Solution {
public:
    int partition(vector<int>& nums, int l, int r) {
        int p = rand() % (r - l + 1) + l; // 随机选一个作为我们的主元
        swap(nums[r], nums[p]);
        int m_v = nums[r];
        int small = l - 1;
        
        for (int big = l; big < r; ++big) {
            if (nums[big] <= m_v) {
                small++;
                swap(nums[small], nums[big]);
            }
        }
        
        small++;
        swap(nums[small], nums[r]);
        return small;
    }

    void sortArray(vector<int>& nums, int l, int r) {
        if(l>=r) return ;

        int pos = partition(nums, l, r);
        sortArray(nums, l, pos - 1);
        sortArray(nums, pos + 1, r);
    }
    
    vector<int> sortArray(vector<int>& nums) {
        int len = nums.size();
        sortArray(nums, 0, len - 1);
        return nums;
    }

};
