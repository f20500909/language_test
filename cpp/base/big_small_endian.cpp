#include <iostream>

using namespace std;

int main() {
    int nNum = 0x12345678;
    char chData = *(char *) (&nNum);

//    if (chData == 0x12) {
//        cout << "big" << endl;
//    } else if (chData == 0x78) {
//        cout << "small" << endl;
//    }

    int data = 0x12345678;
    char chData2 = *(char *) (&data);
    std::cout << chData2 << std::endl;


    return 0;
}
