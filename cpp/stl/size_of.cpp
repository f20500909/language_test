#include <assert.h>

#include <iostream>
#include <vector>

using namespace std;

int main() {
  cout << "sizeof(vector<char>) = " << sizeof(vector<char>) << endl;
  cout << "sizeof(vector<int>) = " << sizeof(vector<int>) << endl;

  cout << "sizeof(vector<int>) = " << sizeof(vector<int>(300, 0)) << endl;

  cout << "sizeof(vector<short>) = " << sizeof(vector<short>) << endl;
  cout << "sizeof(vector<double>) = " << sizeof(vector<double>) << endl;
  cout << "sizeof(vector<long>) = " << sizeof(vector<long>) << endl;
  cout << "sizeof(vector<float>) = " << sizeof(vector<float>) << endl;
  cout << "sizeof(vector<bool>) = " << sizeof(vector<bool>) << endl;
  cout << "sizeof(vector<string>) = " << sizeof(vector<string>) << endl;

  cout << "======= = " << endl;
  vector<bool> v_b = {false,false,false,false};
  cout << "sizeof(bool) = " << sizeof(bool) << endl;
  cout << "sizeof(v_b[0]) = " << sizeof(v_b[1]) << endl;
}
