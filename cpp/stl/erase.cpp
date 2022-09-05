#include<iostream>
#include<vector>
#include<algorithm>
#include<functional>
using namespace std;

int main()
{
    vector<char>vec;
    vector<char>::iterator ite;
    vector<char>::iterator p;
    vec.push_back('A');
    vec.push_back('A');
    vec.push_back('A');
    vec.push_back('B');
    vec.push_back('B');
    vec.push_back('B');
    vec.push_back('B');
    vec.push_back('A');
    vec.push_back('A');
    vec.push_back('A');
    p=remove(vec.begin(),vec.end(),'A');
    //p=vec.erase(remove(vec.begin(),vec.end(),'A'),vec.end());
    cout<<vec.end()-p<<endl;
    for(ite=vec.begin();ite!=vec.end();++ite)
        cout<<*ite<<" ";
    cout<<endl;
    cout<<*p++;
    cout<<*p++;
    cout<<*p++;
    cout<<*p++;
    cout<<*p++;
    cout<<*p++;
//    cout<<vec.size();
    return 0;
}
