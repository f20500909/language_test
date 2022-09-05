#include <iostream>
using namespace std;



int main() {

	const int a = 3;
	const int *pc = &a;
	int *p = const_cast<int *>(&a);


	*p = 5;
	cout << *p << endl;
	cout << *pc << endl;
	cout << a << endl;


	cin.get();

 
	return 0;
}