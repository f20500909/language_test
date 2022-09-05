#include<iostream>
#include<vector>
#include<functional>
#include<map>

using namespace std;


//typedef map<int, int> Map;
//typedef map<int, int>::iterator MapIt;

void print(map<int, int> &m) {
    map<int, int>::iterator it;
    for (it = m.begin(); it != m.end(); it++) {
        cout << it->second << " ";
    }

    cout << endl;
}

void deleteValueFromMap(map<int, int> &m, int n = 5) {
    map<int, int>::iterator it;
    for (it = m.begin(); it != m.end();) {
        if (0 == it->second % n) {
            it = m.erase(it++);
        } else it++;
    }
}

int main() {
    map<int, int> m;
    int i = 0;
    for (i = 0; i < 21; i++) m[i] = i;
    print(m);

    deleteValueFromMap(m); 

    print(m);
    return 0;
}
