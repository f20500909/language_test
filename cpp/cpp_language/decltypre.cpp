#include <string>
using namespace std;

class Student{
public:
    static int total;
    string name;
    int age;
    float scores;
};

int Student::total = 0;

int  main(){
    int n = 0;
    const int &r = n;
    Student stu;

    decltype(n) a = n;  //n 为 int 类型，a 被推导为 int 类型
    decltype(r) b = n;     //r 为 const int& 类型, b 被推导为 const int& 类型
    decltype(Student::total) c = 0;  //total 为类 Student 的一个 int 类型的成员变量，c 被推导为 int 类型
    decltype(stu.name) url = "http://c.biancheng.net/cplus/";  //total 为类 Student 的一个 string 类型的成员变量， url 被推导为 string 类auto

    auto s = stu;  //total 为类 Student 的一个 int 类型的成员变量，c 被推导为 int 类型

    return 0;
}