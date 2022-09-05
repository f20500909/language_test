#include <iostream>

using namespace std;

//基类People
class People {
public:
    People(char *name, int age);

    void display();

protected:
    char *m_name;
    int m_age;
};

People::People(char *name, int age) : m_name(name), m_age(age) {}

void People::display() {
    cout << m_name << " age  " << m_age << " no job " << endl;
}

//派生类Teacher
class Teacher : public People {
public:
    Teacher(char *name, int age, int salary);

    void display();

private:
    int m_salary;
};

Teacher::Teacher(char *name, int age, int salary) : People(name, age), m_salary(salary) {}

void Teacher::display() {
    cout << m_name << " age " << m_age << "  teacher " << m_salary << " salary " << endl;
}

int main() {
    People *p = new People("wang ", 23);
    p->display();

    p = new Teacher("zhao", 45, 8200);
    p->display();

    return 0;
}

