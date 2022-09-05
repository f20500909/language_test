#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;
struct s {
    char c =1;
    short s1 =2;
    short s2 =3;
    int i =4;
};
struct s1 {
    char c;
    int i;
};
struct s2 {
    char c;
    short s1;
    int i;
};
struct s3 {
    char c;
    int i;
    short s1;
};

int main() {
    s temp;
    cout << sizeof(s) << endl;
    cout << sizeof(s1) << endl;
    cout << sizeof(s2) << endl;
    cout << sizeof(s3) << endl;
    return 0;
}
