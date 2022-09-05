#include <iostream>
#include <string.h>

using namespace std;

int cnt =0;

class ClassA {
public:
    ClassA() {
    }

    ClassA(const char *pszInputStr) {
        std::cout<<"do construct ~"<<std::endl;
        _data = new char[strlen(pszInputStr) + 1];
        strncpy(_data, pszInputStr, strlen(pszInputStr) + 1);
    }

    virtual ~ClassA() {
        std::cout<<"do destruct"<<cnt++ <<std::endl;
        delete _data;
    }

public:
    char *_data;
};

int main() {
    ClassA obj1("liitdar");

    ClassA obj2 = obj1;

    cout << "obj2._data is: " << obj2._data << endl;
    cout << "addr(obj1._data) is: " << &obj1._data << endl;
    cout << "addr(obj2._data) is: " << &obj2._data << endl;

    cin.get();

    return 0;
}
