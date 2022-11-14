#include <iostream>
#include <unordered_set>
#include <bits/stdc++.h>
#include <unordered_map>


using namespace std;

class Line{
public:
    int k;
    int b;
    Line(int d1, int d2){
       k = d1;
       b = d2;
    }
    bool operator==(const Line& other)const{//重载operator==(),若没有重载==则定义 unordered_map 时需要isEqual
        return other.k ==k && other.b == b;
    }
};

struct createhash {
    size_t operator()( const  Line l) const // size_t
    {
        //return size_t(l.k ^ l.b);//自定义哈希
        return hash<int>()(l.k) ^  hash<int>()(l.b);
    }

};

struct isEqual {
    bool operator()( const Line l1, const Line l2) const//最后的const不能少
    {
       return l1.k == l2.k && l1.b == l2.b; 
    }
};

int main(){
    //unordered_map<Line ,int , createhash, isEqual> mm;//若使用这种方式,Line类中不需要重载==
    unordered_map<Line ,int , createhash> mm; 
    mm.insert({Line(1,2),1});
    mm.insert({Line(2,3),2});
    auto success = mm.insert({Line(2,3),2});
    if(success.second == false)
        std::cout<<"mm insert failed "<<std::endl;
    for(auto ele : mm)
    {
        std::cout<< ele.first.k<<"  " <<ele.first.b<<std::endl; 

    }

    //unordered_set<Line, createhash> ms;
    unordered_set<Line, createhash, isEqual> ms;//若使用这种方式,Line类中不需要重载==
    ms.insert(Line(2,3));
    auto it = ms.insert(Line(2,3));
    if(it.second == false)
       std::cout<<"ms insert failed "<<std::endl; 
    for(auto ele : ms)
    {
        std::cout<< ele.k<<"  " <<ele.b<<std::endl; 

    }
    return 0;
}


