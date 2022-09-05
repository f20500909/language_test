#include <iostream>
#include <string>

using namespace std;

void func(int *num) {
    cout << " ptr function..." << endl;
}

void func(int num) {
    cout << "int function..." << endl;
}


class my_nullptr_t
{
public:
    template<class T>
    inline operator T*() const    //定义类型转换操作符，使nullptr_t 可转为任意非类成员指针类型
    { return 0; }


    //重载类型转换操作符，使 nullptr_t 可以转换为类 C 中任意的指针类型（数据成员指针/函数成员指针）
    //对类中数据成员的指针，T 将被推导为数据成员的类型 eg: int (X::*); 此时 T 为 int,C 为 X
    //对类中函数成员的指针，T 将被推导为函数成员的类型 eg: int (X::*)(int); 此时T 等效于： typedef int (T)(int)
    template<class C, class T>
    inline operator T C::*() const
    { return 0; }
private:
    void operator&() const;
};

const my_nullptr_t my_nullptr = {};

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
        #define NULL    0
    #else  /* __cplusplus */
        #define NULL    ((void *)0)
    #endif  /* __cplusplus */
#endif  /* NULL */

#ifndef NULL
#ifdef __cplusplus
        #define NULL 0
    #else
        #define NULL ((void *)0)
    #endif
#endif



int main(int argc, char **argv) {
    func(0);
    func(NULL);
    func(nullptr);
    void (*pf)();
    auto pf1 = reinterpret_cast<int *>(pf);
    func(pf1);
}
