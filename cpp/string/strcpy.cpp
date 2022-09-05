//
// Created by root on 19-7-20.
//
#include <iostream>
#include <algorithm>
#include <string.h>

using namespace std;


char* strcpy2(char* dest,const char* src){
    if(nullptr==dest||nullptr==src) return nullptr;
    if(dest==src) return dest;
    char* cur=dest;
    while(*src!='\0'){
        *(cur++)=*(src++);
    }
    *cur = '\0';
    return dest;
}


int main() {
    char *str2 = "szdfgsdf";
    char b[234];
    cout << strcpy2(b, str2) << endl;
    cout << b << endl;

    return 0;
}





