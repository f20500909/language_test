#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) {
	vector<vector<int> > res;
	int n = nums.size();
	sort(nums.begin(),nums.end());
	for (int i = 0; i < n - 2; i++) {
		int j = i + 1;
		int k = n - 1;
		while (j < k) {
			if (nums[i] + nums[j] + nums[k] > 0) k--;//总和大于0，k自减，将总和变小
			else if (nums[i] + nums[j] + nums[k] < 0) j++;//总和小于0，j自增，将总和变大

			else {
				res.push_back({nums[i],nums[j],nums[k]});
				while (nums[j + 1] == nums[j]) j++;//跳过重复的
				j++;
			}
		}
		while (nums[i + 1] == nums[i]) i++;//跳过重复的,保证结果唯一
	}
	return res;
}

int main() {
	vector<int> demo = { -1, 0, 1, 2, -1, -4 };
	vector<vector<int> > ans = threeSum(demo);

	for (auto dim1 : ans) {
		for (auto dim2 : dim1) {

			std::cout << dim2 << " ";

		}
			std::cout << endl;
	}

	cin.get();

	return 0;
}
