#include<iostream>

using namespace std;

void ConstTest1() {
    const int a = 5;
    int *p;
    p = const_cast<int *>(&a);
    (*p)++;
    cout << a << endl;
    cout << *p << endl;
}

void ConstTest2() {
    const int a = 11;
    int& r = const_cast<int &>(a);
    r++;
    cout << r << endl;
    cout << a << endl;
}

int main() {
    ConstTest1();
    ConstTest2();
    return 0;
}