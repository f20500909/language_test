#include <iostream>
using   namespace    std;
/**
	虚继承（虚基类）
*/

#include <iostream>

// 基类A
class A
{
public:
    int dataA;
};

class B : virtual public A
{
public:
    int dataB;
};

class C : virtual public A
{
public:
    int dataC;
};

class D : public B, public C
{
public:
    int dataD;
};


int main()
{

    B b1;
    cout<<"sizeof(B b)"<<sizeof(b1)<<endl;


    D* d = new D;
    cout<<"sizeof(B d)"<<sizeof(d)<<endl;
    d->dataA = 10;
    d->dataB = 100;
    d->dataC = 1000;
    d->dataD = 10000;
    exit(1);

    B* b = d; // 转化为基类B
    C* c = d; // 转化为基类C
    A* fromB = (B*) d;
    A* fromC = (C*) d;

    std::cout << "d address    : " << d << std::endl;
    std::cout << "b address    : " << b << std::endl;
    std::cout << "c address    : " << c << std::endl;
    std::cout << "fromB address: " << fromB << std::endl;
    std::cout << "fromC address: " << fromC << std::endl;
    std::cout << std::endl;

    std::cout << "vbptr address: " << (int*)d << std::endl;
    std::cout << "    [0] => " << *(int*)(*(int*)d) << std::endl;
    std::cout << "    [1] => " << *(((int*)(*(int*)d)) + 1)<< std::endl; // 偏移量20
    std::cout << "dataB value  : " << *((int*)d + 1) << std::endl;
    std::cout << "vbptr address: " << ((int*)d + 2) << std::endl;
    std::cout << "    [0] => " << *(int*)(*((int*)d + 2)) << std::endl;
    std::cout << "    [1] => " << *((int*)(*((int*)d + 2)) + 1) << std::endl; // 偏移量12
    std::cout << "dataC value  : " << *((int*)d + 3) << std::endl;
    std::cout << "dataD value  : " << *((int*)d + 4) << std::endl;
    std::cout << "dataA value  : " << *((int*)d + 5) << std::endl;
}
