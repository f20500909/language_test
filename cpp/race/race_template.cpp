
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
using namespace std;

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
  if (!v.empty()) {
    out << '[';
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

struct Data {
  Data(int v, int id) {
    this->v = v;
    this->id = id;
  }
  int v;
  int id;

  bool operator<(const Data& o) const { return v > o.v; }
};

vector<vector<int>> dirs{{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
const uint64_t INF = 0x3f3f3f3f;
const uint64_t mod = 1e9 + 7;

long long quick_pow(long long x, long long n) {
  long long res = 1;
  while (n > 0) {
    //  判断是否为奇数，若是则true
    if (n & 1) res = (res * x) % mod;
    x = (x * x) % mod;
    n >>= 1;  //  相当于n /= 2;
  }
  return res;
}

string big_add(string s1, string s2) {
  if (s1 == "" && s2 == "") return "0";
  if (s1 == "") return s2;
  if (s2 == "") return s1;
  string maxx = s1, minn = s2;
  if (s1.length() < s2.length()) {
    maxx = s2;
    minn = s1;
  }
  int a = maxx.length() - 1, b = minn.length() - 1;
  for (int i = b; i >= 0; --i) {
    maxx[a--] += minn[i] - '0';  //  a一直在减 ， 额外还要减个'0'
  }
  for (int i = maxx.length() - 1; i > 0; --i) {
    if (maxx[i] > '9') {
      maxx[i] -= 10;  // 注意这个是减10
      maxx[i - 1]++;
    }
  }
  if (maxx[0] > '9') {
    maxx[0] -= 10;
    maxx = '1' + maxx;
  }
  return maxx;
}

template <class T>
T gcd(T big, T small) {
  if (small > big) swap(big, small);
  T temp;
  while (small != 0) {  //  辗转相除法
    if (small > big) swap(big, small);
    temp = big % small;
    big = small;
    small = temp;
  }
  return (big);
}

template <class F>
struct scope_guard {
  F f;
  ~scope_guard() { f(); }
};

template <class F>
scope_guard(F) -> scope_guard<F>;

const vector<vector<int>> dir = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------
//---------------------------

int partition(vector<int>& nums, int l, int r) {
  int p = rand() % (r - l + 1) + l;  // 随机选一个作为我们的主元
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

class Solution {
 public:
  // 掉lower_bound接口
  int res = -1;
  int search(vector<int>& nums, int target) {
    auto p = lower_bound(nums.begin(), nums.end(), target);
    if (p != nums.end() && *p == target) {
      res = p - nums.begin();
    }
    return res;
  }
};

void cpp_test() {
  vector<int> data = {1, 2, 3, 4};
  count_if(data.begin(), data.end(), [&](auto&& d) { return d > 2; });
}

bool is_prime(int val ){
  if(val<=1) return false;
  int t = 2;
  while(t*t<val){
    if(val%t == 0) return false;
    t++;
  }
  return true; 
}

void get_primes(int maxV) {
  // 求出maxV中的所有质数
  // 素数列表
  vector<int> primes;
  // 已经检测的元素
  vector<int> check(maxV + 1, 0);
  check[0] = check[1] = 1;
  //
  for (int i = 2; i <= maxV; i++) {
    // 如果没有check到，加入结果集
    if (!check[i]) primes.push_back(i);
    // 遍历素数列表
    for (int j = 0; j < primes.size(); j++) {
      // 如果当前元素乘上i比maxV要小，break;
      if (primes[j] * i > maxV) break;
      // 标记元素为已访问
      check[primes[j] * i] = 1;
      // 如果当前元素是已有素数的倍数，break;
      if (i % primes[j] == 0) break;
    }
  }
}

// red blue search :

/*

l = -1, r= N
while(l+1 !=r){
  m = l+ (r-l)/2; 
  if(isBlue(m)) l = m;
  else r = m
}
return l or r

*/

int main() {
  cout << "a b c d.." << endl;
  cout << " b ===== d" << endl;
  vector<int> test = {1, 2, 3, 4, 5};
  cout << test << endl;
  cout << test << endl;
}
