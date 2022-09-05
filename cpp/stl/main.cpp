#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <ctime>
#include <assert.h>
using namespace std;
const int EMPTY=0;//尚未填充数据标志
const int REMOVE=-1;//数据删除标志
typedef int KeyType;//关键字类型
#include "func.cpp"
#include "HashTable.h"
int main()
{
    HashTable<DATA> h;
    int i, j, n, p=0;
    bool m;
    DATA e;
    KeyType k;
    ifstream fin("test.txt");
    fin>>n;
    for(i=0; i<n; i++)
    {
        InputFromFile(fin, e);
        j=h.InsertHash(e);
        if(j==EXIST)
        {
            cout<<"有人抢着要（已经有主）的关键字（2K战斗力）："<<e.key<<"，妄图再插入成为: ";
            Visit(e);
            cout<<endl;
        }
        if(j==UNSUCCESS)
            j=h.InsertHash(e);
    }
    fin.close();
    cout<<endl;
    cout<<"按哈希地址的顺序遍历哈希表："<<endl;
    h.TraverseHash(Visit);
    if(h.d_type==1)
    {
        cout<<"请输入待删除球星的关键字（2K战斗力）：";
        InputKey(k);
        m=h.DeleteHash(k, e);
        if(m)
        {
            cout<<"成功删除该元素";
            Visit(e);
            cout<<endl;
        }
    }
    cout<<"请输入待查找球星的关键字（2K战斗力）：";
    InputKey(k);
    n=0;
    j=h.SearchHash(k, p, n);
    if(j==SUCCESS)
    {
        Visit(h.GetElem(p));
        cout<<endl;
    }
    else
        cout<<"未找到"<<endl;
    if(h.d_type==1)
    {
        cout<<"插入球星，请输入待插入球星的关键字（2K战斗力）：";
        InputKey(e.key);
        cout<<"请输入待插入球星的名字：";
        cin>>e.star;
        j=h.InsertHash(e);
        cout<<j<<endl;
        cout<<endl;
        cout<<"按哈希地址的顺序遍历哈希表："<<endl;
        h.TraverseHash(Visit);
    }

    return 0;
}

