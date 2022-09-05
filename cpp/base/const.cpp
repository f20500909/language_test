#include <iostream>
#include <string>
#include <tuple>

using namespace std;


class A {
public:
    int a = 0;
    const int c1 = 0;

    void fun() const {
        cout << "f1" << endl;
    }

    void fun2() {
        a = 2;
        cout << "f2" << endl;
    }
};

int main() {
    A a;
    const A c_a;

    a.fun();
    c_a.fun();
    cout<<c_a.a<<endl;
    cout<<c_a.c1<<endl;


    return 0;
}

