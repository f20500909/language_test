#include <iostream>  
#include <stdexcept>  
using namespace std;

class Test
{
public:
	Test(int data):_data(data) {
		cout << "¹¹ÔìTest : "<<_data << endl;
	}
	void print() {
		cout << "print : "<<_data << endl;
	}
	~Test() {
		cout << "Îö¹¹Test" << endl;
	}
	int _data;
};

template<class T>
class SharePtr {
public:
	int* _count;
	T* _ptr;
	SharePtr(T* ptr):_ptr(ptr),_count(1) {

	}
	T&



};

int main()
{

	cin.get();

	return 0;
}