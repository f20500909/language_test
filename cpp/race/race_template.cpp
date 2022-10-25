
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <map>
#include <ostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
  if (!v.empty()) {
    out << '[';
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

using namespace std;
// using int = int64_t;

const int maxn = 110;
const int INF = 0x3f3f3f3f;
const int64_t mod = 1e9 + 7;

//  UnionFind
class Solution {
 public:
  int n = 0;
  vector<int> father;

  // 初始化，初始化元素为当前id
  void init() {
    for (int i = 0; i < n; i++) father[i] = i;
  }

  // 找到root节点，递归，找到最root的节点
  int find(int i) {
    if (i == father[i]) return i;
    return find(father[i]);
  }

  // join，分别找到i,j的根，让i ,j合并到一个root里面
  void join(int i, int j) {
    i = find(i);
    j = find(j);
    father[i] = j;
  }

  int code(vector<vector<int>>& isConnected) {
    n = isConnected.size();

    // 设置集合的长度
    father = vector<int>(n, 0);
    // 初始化
    init();

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        // 判断是否连接两个城市
        if (isConnected[i][j]) join(i, j);
      }
    }

    unordered_set<int> roots;
    for (int i = 0; i < n; i++) {
      roots.insert(find(i));
    }
    // 统计省份的个数
    return roots.size();
  }
};

// 函数lower_bound()在begin和end中的左闭右开区间进行二分查找，返回大于或等于val的第一个元素位置（迭代器）。如果所有元素都小于val，则返回last的位置。
// lower_bound 实现
int left_bsearch(vector<int>& nums, const int& target) {
  int left = 0, right = nums.size();  //注意right的位置为nums.size() ;
  while (left < right) {              //搜索区间为[left,right)
    int mid = left + (right - left) / 2;
    if (nums[mid] < target)
      left = mid + 1;
    else {
      right = mid;
    }
  }
  return left;
}

// 函数upper_bound()在begin和end中的左闭右开区间进行二分查找,返回的是被查序列中第一个大于查找值的位置（迭代器）。

// upper_bound 实现
int right_bsearch(vector<int>& nums, int target) {  //右边界
  int left = 0, right = nums.size();  //注意right的位置为nums.size() ;
  while (left < right) {              //搜索区间为[left,right)
    int mid = left + (right - left) / 2;
    if (nums[mid] <= target)
      left = mid + 1;
    else {
      right = mid;
    }
  }
  return left;
}

int main() {
  cout << "a b c d.." << endl;
  cout << " b ===== d" << endl;
  vector<int> test = {1, 2, 3, 4, 5};
  cout << test << endl;
  cout << test << endl;
}
