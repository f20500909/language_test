#include <iostream>
#include <vector>
#include <string>

using namespace std;


int myStrlen(const char *str) {
    if (nullptr == str) return -1;
    int cnt = 0;
    while (*str++ != '\0') cnt++;
    return cnt;
}

int strlen2(const char* str){
    if(nullptr==str)
        return -1;
    int cnt=0;
    while(*str!='\0'){
        cnt++;
        str++;
    }
    return cnt;
}


int main435() {
    char d[] = "afds";
    cout << strlen2(d) << endl;
    return 0;
}