#include<iostream>

using namespace std;

#include<assert.h>

template<class T>
class Vector {
private:
		int _size;
		int _capacity;
		int *_start;
		int *_end;
		int *_data;
public:
		Vector(int n, int value) : _size(n), _capacity(n) {
			_data = new int[n];
			for (int i = 0; i < n; i++) {
				_data[i] = value;
			}
			_start = _data;
			_end = _start + n - 1;
		}

		void push_back(T &t) {
			reMall();
			cout << _end - _start << endl;
			*++_end = t;
			_size++;;
		}

		int capacity() { return _capacity; }

		void reMall() {
			if (_capacity == _size) {
				cout << "reMall" << endl;
				T *_temp = new T[2 * size()];
				for (int i = 0; i < size(); i++) _temp[i] = _data[i];

				delete[] _data;
				_data = _temp;
				_start = _data;
				_end = _start + _size-1;
				_capacity = 2 * _size;
			}
		}

		inline int size() { return _size; }

		T &operator[](int index) {
			return _data[index];
		}

		friend ostream &operator<<(ostream &os, Vector<T> &vec) {
			for (int i = 0; i < vec.size(); i++) {
				os << vec[i] << " ";
			}
			return os;
		}
};


int main() {
	Vector<int> vec(9, 0);
	cout << vec[1] << endl;
	cout << vec[0] << endl;
	cout << vec << endl;
	int a = 7;
	vec.push_back(a);

	cout << vec << endl;
	cout << vec.capacity() << endl;
	return 0;
}



