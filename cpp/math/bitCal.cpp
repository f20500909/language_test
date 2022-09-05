#include <iostream>
using namespace std;


int  NumberOf1(int n) {
	int res = 0;
	int flag = 1;
	while (flag) {
		if (n&flag == flag) {
			res++;

		}
		flag = flag << 1;
		cout << flag << endl;
	}
	return res;

}


int main() {

	cout << NumberOf1(1) << endl;
	cin.get();
 

}