#include <iostream>
#include <string>
#include <vector>
using namespace std;

void lcsLength(string x, string y, vector< vector<int>> &c, vector< vector<char>> &b)
{
	int m = x.size();
	int n = y.size();
	c.resize(m + 1);
	b.resize(m + 1);
	for (int i = 0; i < c.size(); ++i)
		c[i].resize(n + 1);
	for (int i = 0; i < b.size(); ++i)
		b[i].resize(n + 1);

	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {
			if (x[i - 1] == y[j - 1]) {
				c[i][j] = c[i - 1][j - 1] + 1;
				b[i][j] = 'c';
			}
			else if (c[i - 1][j] >= c[i][j - 1]) {
				c[i][j] = c[i - 1][j];
				b[i][j] = 'u';
			}
			else {
				c[i][j] = c[i][j - 1];
				b[i][j] = 'l';
			}
		}
	}
}

void print_lcs(vector< vector<char>> &b, string x, int i, int j)
{
	if (i == 0 || j == 0)
		return;
	if (b[i][j] == 'c') {
		print_lcs(b, x, i - 1, j - 1);
		cout << x[i - 1];
	}
	else if (b[i][j] == 'u')
		print_lcs(b, x, i - 1, j);
	else
		print_lcs(b, x, i, j - 1);
}

int main()
{
    string x = "ABCBDAB";
    string y = "BDCABA";
    vector< vector<int>> c;
    vector< vector<char>> b;

    lcsLength(x,y,c,b);
    print_lcs(b,x,x.size(),y.size());
	cin.get();
}