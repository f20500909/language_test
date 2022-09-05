#include <iostream>

using namespace std;

class Parent {
public:
    int iparent;

    Parent() : iparent(10) {}

    virtual void f() {
        cout << "Parent::f()" << endl;
    }

    virtual void g() {
        cout << "Parent::g()" << endl;
    }

    virtual void h() {
        cout << "Parent::g()" << endl;
    }
};

class Child : public Parent {
public:
    int ichild;

    Child() : ichild(100) {}

    virtual void f() {
        cout << "Child::f()" << endl;
    }

    virtual void g_child() {
        cout << "Child::g_child()" << endl;
    }

    virtual void h_child() {
        cout << "Child::h_child()" << endl;
    }
};

class GrandChild : public Child {
public:
    int igrandchild;

    GrandChild() : igrandchild(1000) {};

    virtual void f() {
        cout << "GrandChild::f()" << endl;
    }

    virtual void g_child() {
        cout << "GrandChild::g_child()" << endl;
    }

    virtual void h_gradnchild() {
        cout << "GrandChild::h_grandchild()" << endl;
    }
};

int main(int argc, char **argv) {
    GrandChild gc;
    typedef void(*Fun)(void);
    Fun pf;

    cout << "[0] GrandChild::vfptr->" << endl;
    cout << "    [0]";
    pf = (Fun) *(int *) *(int *) &gc;
    pf();

    cout << "    [1]";
    pf = (Fun) *((int *) *(int *) &gc + 1);
    pf();

    cout << "    [2]";
    pf = (Fun) *((int *) *(int *) &gc + 2);
    pf();

    cout << "    [3]";
    pf = (Fun) *((int *) *(int *) &gc + 3);
    pf();

    cout << "    [4]";
    pf = (Fun) *((int *) *(int *) &gc + 4);
    pf();

    cout << "    [5]";
    pf = (Fun) *((int *) *(int *) &gc + 5);
    pf();

    cout << "[1] Parent.iparent = " << (int) (*((int *) &gc + 1)) << endl;
    cout << "[2] Child.ichild = " << (int) (*((int *) &gc + 2)) << endl;
    cout << "[3] GrandChild.igrandchild = " << (int) (*((int *) &gc + 3)) << endl;
    return 0;
}