#include <iostream>
#include <cassert>
#include <assert.h>
using namespace std;


//懒汉式,后加载模式
class Singleton_Lazy{
public:
    Singleton_Lazy(){

    }
    static Singleton_Lazy* getInstance(){
        if(nullptr==m_single){
            m_single=new Singleton_Lazy();
        }
        return m_single;
    }

    static Singleton_Lazy* m_single;

    void print(){
        cout<<this<<endl;
    }

};


Singleton_Lazy* Singleton_Lazy::m_single=nullptr;



//饿汗式,预先加载,是线程安全的

class Singleton_Hungry{
private:
    Singleton_Hungry(){

    }



public:
    static Singleton_Hungry* getInstance(){
        return m_single;
    }
    static Singleton_Hungry* m_single;

    void print(){
        cout<<this<<endl;
    }
};

Singleton_Hungry* Singleton_Hungry::m_single=new Singleton_Hungry();

int main34() {

    //懒汗
    auto lazy_1=Singleton_Lazy::getInstance();

    lazy_1->print();


    auto lazy_2=Singleton_Lazy::getInstance();

    lazy_2->print();

    //饿汗

    auto hungry_1=Singleton_Hungry::getInstance();

    hungry_1->print();

    auto hungry_2=Singleton_Hungry::getInstance();

    hungry_2->print();

    return 0;
}