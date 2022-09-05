#include <iostream>
#include <cstdio>
using namespace std;

class B
{
public:
    virtual void f(){
        cout<<"B"<<endl;
    };
};

class D : public B
{
public:
    virtual void f(){
        cout<<"D"<<endl;
    };
};

int main()
{
    B* pb = new D;   // unclear but ok
    B* pb2 = new B;
    pb->f();
    pb2->f();

    D* pd = dynamic_cast<D*>(pb);   // ok: pb actually points to a D
    D* pd2 = dynamic_cast<D*>(pb2);   // pb2 points to a B not a D, now pd2 is NULL

    pd->f();
    pb2->f();



    char *p = "This is an example.";


    auto i = reinterpret_cast<int*>(p);
//    auto t = reinterpret_cast<float*>(p);
    auto t = reinterpret_cast<char*>(p);
    cout<<*i<<endl;
    cout<<*t++<<endl;
    cout<<*t++<<endl;
    cout<<*t++<<endl;
    cout<<*t++<<endl;
    cout<<*t++<<endl;

    return 0;
}
