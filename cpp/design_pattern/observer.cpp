//
// Created by 刘春雷 on 2020/3/28.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

class ObserverInterface{
public:
    virtual void dosomething()=0;
    virtual ~ObserverInterface(){}
};

using pObserverInterface=std::unique_ptr<ObserverInterface>;
class SubjectInterface{
public:
    virtual void Add( pObserverInterface obr)=0;
    virtual void Remove(pObserverInterface obr)=0;
    virtual void Notify()=0;

    virtual ~SubjectInterface(){}
};


class Me:public SubjectInterface{
public:
    void Add(pObserverInterface obr) override{
        observers.push_back(std::move(obr));
    }

    void Remove(pObserverInterface obr) override{
        //TODO
    }

    void Notify() override{
        for(const auto& obs:observers){
            obs->dosomething();
        }
    }

private:
    std::vector<pObserverInterface> observers;
};


class Wife:public ObserverInterface{
public:
    void dosomething() override{
        std::cout<<"老公快回来了，开始做饭"<<std::endl;
    }

    Wife(){
        std::cout<<"wife is created"<<std::endl;
    }
    ~Wife(){
        std::cout<<"wife is destroyed"<<std::endl;
    }
};

class Son:public  ObserverInterface{
public:
    void dosomething() override {
        std::cout<<"爸爸快回来了，不能玩游戏了"<<std::endl;
    }
    Son(){
        std::cout<<"son is created"<<std::endl;
    }
    ~Son(){
        std::cout<<"son is destroyed"<<std::endl;
    }
};


int main(){
    Me me;
    pObserverInterface wife=std::make_unique<Wife>();
    pObserverInterface son=std::make_unique<Son>();
    me.Add(std::move(wife));
    
    me.Add(std::move(son));
    //下班了 发消息
    me.Notify();
}