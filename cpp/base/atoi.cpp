#include<iostream>

using namespace std;


int _atoi(const char *str) {
	int res = 0;
	int falg = 1;
	while (*str == ' ') str++;
	if (*str == '-' || *str == '+') {
		if (*str == '-')falg = -1;
		str++;
	}
	while (*str >= '0'&&*str <= '9') {
		res = res * 10 + *str - '0';
		str++;
		if (res < 0) {
			res = 2147483647;
			break;
		}
	}
	return res*falg;
}

int main()
{
	char *s1 = "43234";
	char *s2 = "-12345";
	char *s3 = "432-4.3423";
	char *s4 = "-4324j";
	char *s5 = "+34";

	cout << "atoi:  :" << _atoi(s1) << endl;

	cout << "atoi:  :" << _atoi(s2) << endl;

	cout << "atoi:  :" << _atoi(s3) << endl;

	cout << "atoi:  :" << _atoi(s4) << endl;

	cout << "atoi:  :" << _atoi(s5) << endl;

	system("pause");
	return 0;
}
