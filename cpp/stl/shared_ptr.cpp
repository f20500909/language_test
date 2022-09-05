#include <iostream>
using namespace std;


template<class T>
class Shared_ptr{
public:

    Shared_ptr()=delete;

    Shared_ptr(T* _t):_ptr(_t),_cnt(new int(1)){
    }

    Shared_ptr(Shared_ptr<T>& obj):_ptr(obj._ptr){
        _ptr=obj._ptr;
        _cnt=obj._cnt;
        ++(*(_cnt));
    }

    T* operator->(){
        return _ptr;
    }

    T& operator*(){
        return  *_ptr;
    }

    Shared_ptr<T>& operator=(Shared_ptr<T>& obj){
        if(this==&obj) return *this ;

        if(this->_ptr){
            (*this->_cnt)--;
            if(this->cnt==0){
                delete this->_ptr;
                delete this->_cnt;
            }
        }
        this->_ptr=obj->_ptr;
        this->_cnt=obj->_cnt;
        (*this->_cnt)++;
        return *this;
    }

    ~Shared_ptr(){
        --(*_cnt);
        if(*_cnt==0){
            cout<<"delete~"<<endl;
            delete _ptr;
            delete _cnt;
            _ptr=nullptr;
            _cnt=nullptr;
        }
    }
    T* get(){
        return _ptr;
    }

    int count(){
        return *_cnt;
    }


private:
    T* _ptr;
    int* _cnt;
};


template<class T>
class shared_ptr {
public:
    shared_ptr() = delete;

    shared_ptr(T *p) : _ptr(p), _cnt(new int(1)) {
    }

    shared_ptr(shared_ptr<T> &input) {
        this->_ptr = input._ptr;
        this->_cnt = input._cnt + 1;
    }




    ~shared_ptr() {
        (*this->_cnt)--;
        if ((*this->_cnt) == 0) {
            delete _ptr;
            delete _cnt;
            _ptr = nullptr;
            _cnt = nullptr;
        }
    }

    T *get() {
        return _ptr;
    }

    T &operator*() {
        return *_ptr;
    }

    T *operator->() {
        return _ptr;
    }


private:
    T *_ptr;
    int *_cnt;


};


int main(){
    Shared_ptr<char> sp0(nullptr);
    Shared_ptr<char> sp1(new char('a'));
    cout<<sp1.count()<<endl;


    Shared_ptr<char> sp2(new char('b'));
    cout<<sp2.count()<<endl;


    Shared_ptr<char> sp3(sp2);
    cout<<sp3.count()<<endl;

    Shared_ptr<char> sp4=sp1;
    cout<<sp1.count()<<endl;
    cout<<sp4.count()<<endl;

    return 0;
}
