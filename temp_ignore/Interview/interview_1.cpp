#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex m;
condition_variable cond;
int flag=10;
void fun(int num){
	for(int i=0;i<50;i++){
		unique_lock<mutex> lk(m);
		while(flag!=num) cond.wait(lk);
		for(int j=0;j<num;j++) cout<<j<<" ";
		cout<<endl;
		flag=(10==num) ?100:10;
		cond.notify_one();
	}
}

int main(){
	thread child(fun,10);
	fun(100);
	child.join();
	system("pause");

	return 0;
}