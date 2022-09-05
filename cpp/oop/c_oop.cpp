#include <stdio.h>

typedef void(*FUNC)(); //定义一个函数指针来实现对成员函数的继承

typedef struct _Base  //父类
{
    FUNC _fun;//由于C语言中结构体不能包含函数，故借用函数指针在外面实现
    int _B1;
} Base;

typedef struct _Derived//子类
{
    Base _b1;//在子类中定义一个基类的对象即可实现对父类的继承
    int _D1;
} Derived;

void fb_()       //父类的同名函数
{
    printf("b1:_fun()\n");
}

void fd_()       //子类的同名函数
{
    printf("d1:_fun()\n");
}

int main() {
    Base _b1;//定义一个父类对象_b1
    Derived _d1;
//    定义一个子类对象_d1

    _b1._fun = fb_;//父类的对象调用父类的同名函数
    _d1._b1._fun = fd_;//子类的对象调用子类的同名函数

    Base *_p1 = &_b1;//定义一个父类指针指向父类的对象
    _p1->_fun(); //调用父类的同名函数

//    _p1 = (Base *) &_d1;//让父类指针指向子类的对象,由于类型不匹配所以要进行强转
    _p1 = (Base *) &_d1;//让父类指针指向子类的对象,由于类型不匹配所以要进行强转
    _p1->_fun(); //调用子类的同名函数

    return 0;
}
