/*
说明：程序是在 64 位编译器下测试的
*/
#include <iostream>
using namespace std;
#define offset(TYPE,MEMBER) ((long)&((TYPE *)0)->MEMBER)

#pragma pack(push)
#pragma pack(1)
struct A
{
    short var; // 偏移 0 字节 （内存对齐原则 : short 2 字节 + 填充 2 个字节）
    int var1;  // 偏移 4 字节 （内存对齐原则：int 占用 4 个字节）
    long var2; // 偏移 8 字节 （内存对齐原则：long 占用 8 个字节）
    char var3; // 偏移 16 字节 （内存对齐原则：char 占用 1 个字节 + 填充 7 个字节）
    string s;  // 偏移 24 字节 （string 占用 32 个字节）
};
#pragma pack(pop)

int main()
{
    string s;
    A ex1;
    cout << offset(A, var) <<endl;
    cout << offset(A, var1) <<endl;
    cout << offset(A, var2) <<endl;
    cout << offset(A, var3) <<endl;
    cout << offset(A, s) <<endl;
    cout << sizeof(ex1) << endl;  // 56 struct
    return 0;
}