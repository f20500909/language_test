#include <iostream>
#include <tuple>
using  namespace std;


tuple<int, bool> test() {
	return make_tuple(123,false);
}
 

int main() {

	auto data = test();
	cout << get<0>(data) << endl;
	cout << get<1>(data) << endl;
	cin.get();




	return 0;
}