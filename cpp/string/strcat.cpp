#include <iostream>
#include <cassert>
#include <string.h>

using namespace std;

void Mystrcat(char *des, const char *src) {
	assert(src != NULL);
	while (*des != '\0') des++;
	while (*des++ = *src++);
	*des = '\0';//在目标文件的末尾赋值'\0'
}


int main1213() {
	char str1[100] = "abfdef";
	char *str2 = "sdfs";
//	strcat(str1, str2);
	Mystrcat(str1, str2);
	cout << str1 << endl;


	return 0;
}
