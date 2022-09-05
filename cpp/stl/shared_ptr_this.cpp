#include <iostream>
#include <memory>
using namespace std;


class tester
{
public:
    tester();
    ~tester()
    {
        std::cout << "析构函数被调用!\n";
    }
public:
    shared_ptr<tester> sget()
    {
        return shared_ptr<tester>(this);
    }
};

int main()
{
    tester t;
    shared_ptr<tester> sp =  t.sget(); // …
    return 0;
}
