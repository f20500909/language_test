#include<iostream>
#include<vector>
#include<unordered_map>
#include<algorithm>
#include<functional>

using namespace std;

int main() {
    unordered_map<int, int> m;
    m[0] = 11;
    m[2] = 22;
    m[2] = 22222222;
    m[3] = 33;
    m[4] = 33;
    m[5] = 33;
    m.insert({2,2344});
    unordered_map<int, int>::iterator s = m.find(2);
    if (s == m.end()) {
        cout << "not found" << endl;
        return 0;
    }
    cout << s->first << endl;
    cout << s->second << endl;


    return 0;
}
