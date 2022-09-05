#include <iostream>
using namespace std;

void reverse(char* beg, char* end) {
	if (beg == NULL || end == NULL) return ;
	while (beg < end) {
	cout << beg << endl;

		char temp = *beg;
		*beg = *end;
		*end = temp;
		beg++; end--;
	}

}

int main() {
	char* row= "wangheng";


	cout << row << endl;

	char* beg = row;
	char* end = row;
	while (*end != '\0') {
		end++;
	}
	end--;

	cout << row << endl;
	cout << end << endl;
	reverse(beg,end) ;
	cout << row << endl;
	cin.get();


	return 0;
}


