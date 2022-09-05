#include<iostream>
#include<math.h>
#include <stdlib.h>
#include <cstdlib>
#include <cassert>

using namespace std;

//二分法
int sqrt(int x) {
	long long i = 0;
	long long j = x / 2 + 1;
	while (i <= j) {
		long long mid = (i + j) / 2;
		long long sq = mid * mid;
		if (sq == x) return mid;
		else if (sq < x) i = mid + 1;
		else j = mid - 1;
	}
	return j;
}


double sqrt2(double n) {
	assert(n >= 0);
	if (n == 1) return 1;
	double left = 0;
	double right = n;
	while ((right - left) > 1e-6) {
		double mid = left + (right - left) / 2;
		if (mid * mid > n) right = mid - 1e-6;
		if (mid * mid < n) left = mid + 1e-6;
	}
	return left;
}


double sqrt(double x) {
	if (x == 0) return 0;
	double last = 0.0;
	double res = 1.0;
	while (res != last) {
		last = res;
		res = (res + x / res) / 2;
	}
	return res;
}

int main() {
	cout << sqrt(16.43434) << endl;
	cout << sqrt2(16.343) << endl;
	return 0;

}
