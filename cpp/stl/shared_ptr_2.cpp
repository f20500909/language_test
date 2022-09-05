#include <iostream>

using namespace std;

template<typename T>
class shared_ptr {
public:
		shared_ptr(T *p) : _cnt(new int(1)), _ptr(p) {}

		shared_ptr(shared_ptr<T> &other) : _cnt(&(++*other._cnt)), _ptr(other._ptr) {}

		T *operator->() { return _ptr; }

		T &operator*() { return *_ptr; }

		shared_ptr<T> &operator=(shared_ptr<T> &other) {
			++*other._cnt;
			if (this->_ptr && 0 == --*this->count) {
				delete _cnt;
				delete _ptr;
			}
			this->_ptr = other._ptr;
			this->count = other._cnt;
			return *this;
		}

		~shared_ptr() {
			if (--*_cnt == 0) {
				delete _cnt;
				delete _ptr;
			}
		}

		int getCount() { return *_cnt; }

private:
		int *_cnt;
		T *_ptr;
};


int main() {
	int a = 1;
	int *p = &a;
	shared_ptr<int> p1(p);
	cout << *p1;


	return 0;
}
