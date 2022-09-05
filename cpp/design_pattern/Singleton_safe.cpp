#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <iostream>

using namespace std;

template<class T>
class singleton {
protected:
		singleton() {};
private:
		singleton(const singleton &) {};

		singleton &operator=(const singleton &) {};
		static T *m_instance;
public:
		template<typename... Args>
		static T *GetInstance(Args &&... args) {
			if (m_instance == NULL)
				m_instance = new T(std::forward<Args>(args)...);
			return m_instance;
		}


		static void DestroyInstance() {
			if (m_instance)
				delete m_instance;
			m_instance = NULL;
		}
};


template<class T>
T *singleton<T>::m_instance = NULL;

struct A {
		int _a;
		int _b;

		A(int a, int b) : _a(a), _b(b) {}
};

int main() {
	int *p1 = singleton<int>::GetInstance(5);
	int *p2 = singleton<int>::GetInstance(10);
	cout << *p1 << "  " << *p2 << endl;
	string *p3 = singleton<string>::GetInstance("aa");
	string *p4 = singleton<string>::GetInstance("bb");

	cout << *p3 << "  " << *p4 << endl;

	A *p5 = singleton<A>::GetInstance(1, 2);

	A *p6 = singleton<A>::GetInstance(4, 5);

	cout << p5->_a << "  " << p6->_a << endl;
	return 0;
}

#endif
