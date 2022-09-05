#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <bitset>
#include <vector>
using namespace std;
vector<int> findDisappearedNumbers(vector<int>& nums) {
    int len=nums.size();
    vector<int> res;
    if(0==len) return res;
    for(int i=0;i<len;i++){
        nums[nums[i]-1]=nums[nums[i]-1]-len;
    }
    for(int i=0;i<len;i++){
        if(nums[i]>=1) res.push_back(i+1);
    }
    return res;
}

int mainsdfsdfsdfee(){
    unsigned int i=-100;
    cout<<i<<endl;


    return 0;
}
