#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex m;
condition_variable cond;
int loop = 10;
int flag = 0;

void func(int id) {
	for (int i = 0; i < loop; ++i) {
		unique_lock<mutex> lk(m);
		while (flag != id)
			cond.wait(lk);
		cout << static_cast<char>('A' + id) << " ";
		flag = (flag + 1) % 3;
		cond.notify_all();
	}
}

int main() {
	thread A(func, 0);
	thread B(func, 1);
	func(2);
	cout << endl;

	A.join();
	B.join();

}
