#include<iostream>
#include<vector>
#include<string>
#include<queue>
#include<deque>
#include<assert.h>

using namespace std;


void vec_del() {
	vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vector<int>::iterator iter = vec.begin();

	for (auto v:vec) {
		cout << v << " ";
	}
	cout << endl;

//	for (; iter != vec.end();) {
//		if (*iter == 3) {
//			iter = vec.erase(iter);
//		} else {
//			iter++;
//		}
//	}

	for(;iter!=vec.end();iter++){
		if(*iter==2){
			vec.erase(iter);
		}
	}



	for (auto v:vec) {
		cout << v << " ";
	}
	cout << endl;


}


int main() {

	vec_del();


	return 0;
}



