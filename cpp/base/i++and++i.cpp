#include <stdio.h>
#include <iostream>
using namespace std;

class myInt{
public:
    myInt(int data):_data(data){ }
    int operator*() const;
    myInt& operator++();
    const myInt operator++(const int k);
    myInt& operator+(const int& A);
    friend ostream& operator<<(ostream& out,const myInt& _int);
    int data() const;

private:
    int _data;
};

int myInt::data() const{
    return _data;
}

myInt& myInt::operator++(){
    *this=*this+1;
    return *this;
}

const myInt myInt::operator++(int k){
    int oldValue=_data;
    *this=*this+1;
    return oldValue;
}

myInt& myInt::operator+(const int& A){
    _data+=A;
    return *this;
}

int myInt::operator*() const{
    return this->_data;
}

ostream& operator<<(ostream& out ,const myInt& i){
    out<<i.data();
    return out;
}

int main(void)
{
    myInt i(0);//我的int
    int t=0;   //标准int

    cout<<"i为: "<<i<<endl;
    cout<<"t为: "<<t<<endl;
    ++i =3;//++i可以为左值
    ++t=3;//++t可以为左值
    //i++ =3;//i++不可以为左值
    //t++=3;//t++不可以为左值

    cout<<"i为: "<<i<<endl;
    cout<<"t为: "<<t<<endl;
//    (t++)++;
    (++t)++;
    cout<<"t为: "<<t<<endl;


    return 0;
}

