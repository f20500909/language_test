#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

using namespace std;

mutex m;
condition_variable cond;
int loop = 100;
int flag = 0;

void func(int id) {
	for (int i = 0; i < loop; i++) {
		unique_lock<mutex> lk(m);
		while (flag != id) cond.wait(lk);
		cout << static_cast<char>('A' + id) << " ";
		flag = (flag + 1) % 3;
		cond.notify_all();
	}
	return;
}

void func2(int id){
	for(int i=0;i<loop;i++){
		cout<<static_cast<char>('A'+id)<<" ";
//		flag=(flag+1)%3;
	}
	return ;
}

int main() {
	thread A(func, 0);
	thread B(func, 1);
	thread C(func, 2);

	A.join();
	B.join();
	C.join();

	return 0;
}




