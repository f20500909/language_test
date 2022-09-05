#include <memory>
#include <iostream>

class Demo:public std::enable_shared_from_this<Demo>
{
public:

    Demo():data(new int(3)){
    }

    std::shared_ptr<Demo> getPtr() {
        return shared_from_this();
    }

    ~Demo() {
        std::cout<<"~Demo()"<<std::endl;
//        delete data;
    }

    int * data;
};

int main()
{

    Demo* source=new Demo();

    std::shared_ptr<Demo> sp1(source);

//    此行结束后，引用计数加一
    std::shared_ptr<Demo> sp2 = sp1->getPtr();

//    此时source的资源被两个智能指针同时指向，并且引用计数都是2

    // 打印sp1和bp2的引用计数
    std::cout << "sp1.use_count() = " << sp1.use_count() << std::endl;
    std::cout << "sp2.use_count() = " << sp2.use_count() << std::endl;
}