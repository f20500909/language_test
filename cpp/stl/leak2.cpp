//share_ptr可能出现循环引用，从而导致内存泄露
class A
{
public:
		share_ptr<B> p;
};

class B
{
public:
		share_ptr<A> p;
}

int main()
{
	while(true)
	{
		share_prt<A> pa(new A()); //pa的引用计数初始化为1
		share_prt<B> pb(new B()); //pb的引用计数初始化为1
		pa->p = pb; //pb的引用计数变为2
		pb->p = pa; //pa的引用计数变为2
	}
	//假设pa先离开，引用计数减一变为1，不为0因此不会调用class A的析构函数，因此其成员p也不会被析构，pb的引用计数仍然为2；
	//同理pb离开的时候，引用计数也不能减到0
	return 0;
}

/*
** weak_ptr是一种弱引用指针，其存在不会影响引用计数，从而解决循环引用的问题
*/