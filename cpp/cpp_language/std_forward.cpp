#include <iostream>

using namespace std;


void Test0(int a) {
    cout << "int a" << endl;
}

void Test1(int &a) {
    cout << "int &a L" << endl;
}

void Test2(int &&a) {
    cout << "int &&a R" << endl;
}


void run_test(int &&a) {
    Test0(a);
    Test1(a);
    Test2(forward<int>(a));
}

int main() {
    run_test(1);
}

