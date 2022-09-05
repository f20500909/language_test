#include <iostream>
#include <vector>
#include <list>

using namespace std;

void print(list<int> l){
    for(auto _l:l) cout<<_l<<" ";
    cout<<endl;
}

int main() {



//    vector<int> vec = {1, 2, 3, 4, 5, 7, 8};
//    vector<int>::iterator iter = vec.begin();
//    cout << *iter << endl;
//
////    vec.push_back(9);
//    vec.insert(iter++,9);
////    vec.erase(iter);
//    iter++;
////    cout << *iter << endl;
////    cout << *iter << endl;

    list<int> l = {1, 2, 3, 4, 5, 6};
    auto it1 = l.begin();
    auto it2 = l.begin();
    it2++;
    it2++;
    it2++;

    cout << *it1 << endl;
    cout << *it2 << endl;


    l.push_back(3);
    l.insert(it1,3);
    l.erase(it1);

//    cout << *it1 << endl;
    cout << *it2 << endl;
    print(l);

    return 0;
}

