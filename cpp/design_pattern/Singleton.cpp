#include <iostream>
#include <stddef.h>
using namespace std;

class Singleton {
public:
		static Singleton *Instance();

		void fun() {
			cout << "===" << endl;
		}

protected:
		Singleton() {
			cout << "Singleton...." << endl;
		};
private:
		static Singleton *_instance;
};

Singleton *Singleton::_instance =nullptr;

Singleton *Singleton::Instance() {
	if (_instance == 0) {
		_instance = new Singleton();
	}
	return _instance;
}

int main() {
	Singleton *sgn = Singleton::Instance();
	sgn = Singleton::Instance();
	sgn->fun();
	return 0;
}

