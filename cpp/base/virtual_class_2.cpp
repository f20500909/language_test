#include <iostream>

using namespace std;

class Base {
public:
    virtual void f() { cout << "Base::f()" << endl; }

    virtual void g() { cout << "Base::g()" << endl; }

    virtual void h() { cout << "Base::h()" << endl; }
};

template<class T>
class Tool {
private:
    int addr() {
        return &T;
    }

    T obj;

public:
    void run() {
        cout << "Base address is :" << &obj << endl;
        int *vptr = reinterpret_cast<int *>(&obj);
        cout << "vptr address is :" << vptr << endl;
        int *fun1 = reinterpret_cast<int *>(*vptr);
        cout << "first function address is " << fun1 << endl;
        int *fun2 = ++fun1;
        cout << "second function address is " << fun2 << endl;
        int *fun3 = ++fun2;
        cout << "third function address is " << fun3 << endl;
        cout << "use thrid function: " << endl;
        auto pFun = reinterpret_cast<void (*)(void)> (*fun3);


    }

    void print() {
        Base b;
        cout << "Base address is :" << &b << endl;
        int *vptr = reinterpret_cast<int *>(&b);
        cout << "vptr address is :" << vptr << endl;
        int *fun1 = reinterpret_cast<int *>(*vptr);
        cout << "first function address is " << fun1 << endl;
        int *fun2 = ++fun1;
        cout << "second function address is " << fun2 << endl;
        int *fun3 = ++fun2;
        cout << "third function address is " << fun3 << endl;
        cout << "use thrid function: " << endl;
        auto pFun = reinterpret_cast<void (*)(void)> (*fun3);
        pFun();
    }
};

int main() {
    Tool<Base> t;
    t.run();

    return 0;
}
