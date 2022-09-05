//
// Created by root on 19-7-20.
//

#ifndef MAIN_MYSTRING_H
#define MAIN_MYSTRING_H

class MyString{
public:
    MyString();
    MyString(const MyString& str);
    MyString(const char* str);
    ~MyString();
    MyString& operator=(const MyString& str);
    MyString& operator+(const MyString& str);
    bool operator==(const MyString& str);
    int len();
private:
    char* data;
    int size;
};

MyString::MyString(){

};
MyString::MyString(const char* str){

};

#endif //MAIN_MYSTRING_H
