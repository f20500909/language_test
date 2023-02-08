
#include <iostream>
#include <vector>
using namespace std;

int main() {
  vector<bool> data_bool = {false};
  vector<int> data_int = {1};
  vector<long long> data_long_long = {1};
  cout << "sizeof(bool):" << sizeof(bool) << endl;

  cout << "sizeof(data_bool[0]):" << sizeof(data_bool[0]) << endl;

  cout << "================== " << endl;
  cout << "sizeof(int):" << sizeof(int) << endl;
  cout << "sizeof(data_int[0]):" << sizeof(data_int[0]) << endl;

  cout << "================== " << endl;
  cout << "sizeof(long long ):" << sizeof(long long) << endl;
  cout << "sizeof(data_long_long[0]):" << sizeof(data_long_long[0]) << endl;

  return 0;
}

