#include <utility>

#include <iostream>
#include <type_traits>
using namespace std;
 
 
void funcLR(const int& x) //左值
{
	std::cout << "lvalue" << std::endl;
}
 
void funcLR(int&& x) //右值
{
	std::cout << "rvalue" << std::endl;
}
 
template <class T> 
void func(T&& x) 
{
	funcLR(x);
	funcLR(std::forward<T>(x));  //std::forward<int>(x) 会进左值引用
	funcLR(std::forward<T&>(x)); 
    //forward函数会根据参数进行选择对应的forward模板函数，左引用叠加左引用还是左引用
}
 
int main()
{
	int i = 1;
	int str = std::forward<int>(55);
	funcLR(std::forward<int>(i));   //进右值引用函数 普通左值引用输出右值
	funcLR(std::forward<int&>(i));  //进左值引用函数 指定int&输出左值引用
	funcLR(std::forward<int&&>(i)); //进右值引用函数 右值引用输出右值引用
 
	func(i); //编译器会将模板T指定为int& 左值引用
	func(std::move(i)); //右值输出，x为普通变量
 
	return 0;
}
 

