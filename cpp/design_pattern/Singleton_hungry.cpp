#include <iostream>
#include <cassert>

using namespace std;


class Singleton {
private:
    static Singleton *instance;

    Singleton() {
        cout << "Singleton begin ~" << endl;
    }

public:
    static Singleton *getInstance();
};

Singleton *Singleton::getInstance() {
    return instance;
}

Singleton *Singleton::instance = new Singleton();



int main() {
    std::cout << "Enter main" << std::endl;    //这里主函数开始
    Singleton *test1 = Singleton::getInstance();    //通过公共接口获取单例对象
    Singleton *test2 = Singleton::getInstance();    //通过公共接口获取单例对象

    assert(test1 == test2);
    std::cout << "End" << std::endl;
    return 0;
}
