
#include <iostream>
#include <vector>

using namespace std;

void print(const vector<int> &vec) {
    for (auto v:vec) {
        cout << v << " ";
    }
    cout << endl;
}

// 关于排序基准，试了三种方法，随机数、首位元素、两端和中间点取中值，结果是随机更快一点
int partition(vector<int>& nums, int left, int right) {
    // 为了防止第一位元素是最小或者最大的那几个，取随机元素，尽量每次将区间对半分
    int idx = rand() % (right - left + 1) + left;
    swap(nums[left], nums[idx]);
    int base = nums[left];
    // 快速排序，注意是从大到小，因为我们找的是第K 大
    while (left < right) {
        while (left < right && nums[right] >= base) --right;
        nums[left] = nums[right];
        while (left < right && nums[left] <= base) ++left;
        nums[right] = nums[left];
    }
    nums[left] = base;
    return left;
}



void quickSort(vector<int> &vec, int left, int right) {
    if (left > right) return;
    int index = partition(vec, left, right);
    if (index > left) quickSort(vec, left, index - 1);
    if (index < right) quickSort(vec, index + 1, right);
}


void quickSort3(vector<int> &data, int left, int right) {
    if (left >= right) return;
    int index = partition(data, left, right);
    if (index > left) quickSort3(data, left, index - 1);
    if (index < right) quickSort3(data, index + 1, right);
}

int main() {

    vector<int> data;
    int len = 100;
    for(int i=0; i<len; i++){
        data.push_back(rand()%100);
    }

    // print(data);
//    bubleSort(data);
    quickSort3(data, 0, data.size() - 1);
//    selectSort(data);

    print(data);
    return 0;
}


class Solution {
public:
    int len = 0;


    int partition2(vector<int>& nums,int l,int r){
        int idx = l + (rand() % (r-l+1));
        swap(nums[l],nums[idx]);
        int base = nums[l];

        while(l < r){
            while(l < r && nums[r] >= base) r--;
            nums[l] = nums[r];

            while(l < r && nums[l] <= base) l++;
            nums[r] = nums[l];
        }
        nums[l] = base;
        return l;
    }

    vector<int> sortArray(vector<int>& nums) {
        len = nums.size();
        quickSort(nums, 0, len-1);
        return nums;
    }

    void quickSort(vector<int>& nums,int l,int r){
        if(l>=r) return ;
        int p = partition(nums, l, r);
        if(p > l) quickSort(nums,l, p-1);
        if(p < r) quickSort(nums,p+1,r);
        return ;
    }

};
