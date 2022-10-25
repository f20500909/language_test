#include <memory>
#include <iostream>

class ErrorDemo
{
public:

    ErrorDemo():data(new int(3)){
    }

    std::shared_ptr<ErrorDemo> getPtr() {
        return std::shared_ptr<ErrorDemo> (this);
    }

    ~ErrorDemo() {
        std::cout<<"~ErrorDemo()"<<std::endl;
        delete data;
    }

    int * data;
};

int maindsf()
{

    ErrorDemo* source=new ErrorDemo();

    std::shared_ptr<ErrorDemo> sp1(source);

//    此行结束后，引用计数并没有加一
    std::shared_ptr<ErrorDemo> sp2 = sp1->getPtr();

//    此时source的资源被两个智能指针同时指向，并且引用计数都是1

    // 打印sp1和sp2的引用计数
    std::cout << "sp1.use_count() = " << sp1.use_count() << std::endl;
    std::cout << "sp2.use_count() = " << sp2.use_count() << std::endl;
}