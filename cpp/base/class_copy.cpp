#include <iostream>

using namespace std;


class Base {
public:
    explicit Base() : data(new int(0)) {}


    ~Base() {
        std::cout << "~Base()" << std::endl;
        delete data;//
    }

    void out() {
        std::cout << data << ":" << *data << std::endl;
    }


private:
    int *data;
};


int main() {
    Base base1;
    {
        Base base2(base1);
        base1.out();       // address1:0
        base2.out();       // address2:0
    }                      // ~Base()，base2析构
    cout << " ==" << endl;


//    base1.out();           // address1:未知
    return 0;
}
