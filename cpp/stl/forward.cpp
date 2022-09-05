#include <iostream>
#include <vector>

using namespace std;

template<typename T>
void print(T &t) {
	cout << "lvalue" << endl;
}

template<typename T>
void print(T &&t) {
	cout << "rvalue" << endl;
}

template<typename T>
void TestForward(T &&v) {
	print(v);
	print(std::forward<T>(v));
	print(std::move(v));
}

int main() {
//	TestForward(1);
	int x = 1;
//	TestForward(x);
	TestForward(std::forward<int>(x));
	return 0;
}
