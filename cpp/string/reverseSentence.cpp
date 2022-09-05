#include <iostream>
using namespace std;

void reverse(char* beg, char* end) {
	if (beg == NULL || end == NULL) return;
	while (beg < end) {
		char temp = *beg;
		*beg = *end;
		*end = temp;
		beg++; end--;
	}
}
char* reverseSentence(char* data) {
	if (data = NULL)return NULL;
	char* beg = data;
	char* end = data;
	while (*end != '\0')end++;
	end--;
	reverse(beg, end);
	beg = end = data;
	while (*beg != '\0') {
		if (*beg == ' ')
		{
			beg++;
			end++;
		}
		else if (*end == ' ' || *end == '\0') {
			reverse(beg, end);
		}
		else {
			end++;
		}
	}
	return data;
}


int main()
{

	char* row = "wangheng";
	cout << row << endl;
	char* beg = row;
	char* end = row;
	while (*end != '\0') {
		end++;
	}
	end--;

	cout << row << endl;
	cout << reverseSentence (row)<< endl;
	cin.get();




	return 0;
}