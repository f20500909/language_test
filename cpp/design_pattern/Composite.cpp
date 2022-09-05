#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

class Component{
public:
    Component(string name){
        this->name=name;
    }
    virtual void add(Component* &component)=0;
    virtual void remove(Component* &component )=0;
    virtual void display(int) =0;
    string name;
};

class Leaf: public Component{
public:
    Leaf(string name):Component(name){

    }

    void add(Component* component){
    }

    void remove(Component* c){

    }
    void display(int depth){
        string str(depth,'-');
        str+=name;
        cout<<str<<endl;
    }


};

class Composite:public Component{
public:
    Composite(string name):Component(name){
    }

    void add(Component* component){
        components.push_back(component);
    }

    void remove(Component* component){
        auto iter=components.begin();
        while(iter!=components.end()){
            if(*iter==component){
                components.erase(iter);
            }
            iter++;
        }
    }

    void display(int depth){
        string str(depth,'-');
        str+=name;
        cout<<str<<endl;
        auto iter=components.begin();
        while(iter!=components.end()){
            (*iter)->display(depth+2);
            iter++;
        }
    }

private:
    vector<Component*> components;
};

int main(){
    Component* p=new Composite("小李");
    p->add(new Leaf("小王"));
    p->add(new Leaf("小强"));

    Component* sub=new Component("小虎");
    sub


    return 0;
}

