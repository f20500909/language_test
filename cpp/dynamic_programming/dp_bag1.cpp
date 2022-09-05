#include <iostream>
#include<algorithm>
#include<memory.h>

using namespace std;

int main() {

	int total_weight = 10;
	int w[6] = {0, 5, 4, 3, 2, 1};
	int v[6] = {0, 1, 2, 3, 4, 5};
	int dp[11] = {};
	int path[6][11] = {0};
	memset(dp, 0, sizeof(dp));

	for (int i = 0; i < 11; i++) {
		cout << dp[i] << " ";
	}
	cout << endl;


    for (int i = 1; i <= 5; i++) {
        for (int j = 10; j >= w[i]; j--) {
            if (dp[j] < dp[j - w[i]] + v[i]) {
                dp[j] = dp[j - w[i]] + v[i];
                path[i][j] = 1;
            }
        }
    }
	for (int i = 1; i <= 5; i++) {
		for (int j = 10; j >= w[i]; j--) {
			dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
		}
	}


	int i = 5, j = 10;
	while (i > 0 && j > 0)
	{
		if (path[i][j] == 1)
		{
			cout << i << ' ';
			j -= w[i];
		}
		i--;
	}
	cout << endl;

	cout << "total value: " << dp[10] << endl;
	return 0;
}
