#include <iostream>
using namespace std;

//±©¡¶∆•≈‰À„∑®
int violentMatch(char* s, char* p)
{
	int sLen = strlen(s);
	int pLen = strlen(p);
	for (int i = 0; i < sLen-pLen; i++) {
		for (int j = 0; j < pLen; j++) {
			if(s[i+j]!=p[j]){
				break;
			}
			if (j == pLen - 1) return i;
		}
	}
	return -1;
}


int main() {
	char* targetStr = "0ABCD";
	char* str = " ABCD";
	int res = violentMatch(targetStr,str);
	cout << res << endl;
	cin.get();
 
	return 0;
}
