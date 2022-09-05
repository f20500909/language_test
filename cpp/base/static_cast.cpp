#include <iostream>
using namespace std;

using namespace std;

class A {
public:
    void print(){
        cout<<"A.."<<endl;
    }

};

class B : public A {
public:
    void print(){
        cout<<"B.."<<endl;
    }
    void fun(){
        cout<<" in B..  data= "<<data<<endl;
    }

    int data=10;

};

class C {
public:
    void print(){
        cout<<"C.."<<endl;
    }
};

int main23() {
    A *a = new A;
    B *b;
    C *c;
    a = static_cast<A*>(b);  // 编译不会报错, B类继承A类
    a->print();
//    a->fun();             //编译出错,A类中没有fun函数 转换后的指针只能访问转换后所指类中的函数地址

    b = static_cast<B*>(a);  // 编译不会报错, B类继承A类
    b->print();
    b->fun();               //编译正确,但是运行出错,因为指针b原本是A*类型,不含数据data,所以报错 interrupted by signal 11: SIGSEGV


//    c = static_cast<C*>(a);  // 编译报错, C类与A类没有任何关系
    return 1;
}
