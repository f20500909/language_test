#include <iostream>

using namespace std;

int main() {
  int nNum = 0x12345678;
  char chData = *(char *)(&nNum);

  if (chData == 0x12) {
    cout << "big" << endl;
  } else if (chData == 0x78) {
    cout << "small" << endl;
  }

  int num = 0x12345678;
  char c_data = *(char *)(&num);
  if (c_data == 0x12)
    cout << " big endian" << endl;
  else
    cout << "small endian" << endl;

  return 0;
}
