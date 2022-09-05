#include <iostream>
#include <pybind11/embed.h>
#include <pybind11/stl.h>
#include <Python.h>
using namespace std;
namespace py = pybind11;
class Person {
public:
    std::string name;
public:
    Person() {};
    Person(std::string name) : name(name) {}
    ~Person() {
        std::cout<<"我被析构了~"<<std::endl;
    };
public:
    void setName(std::string name) {
        this->name = name;
    }
    std::string getName() {
        return this->name;
    }
    void printName() {
        std::cout << name << endl;
    }
    vector<std::string> getList() {
        vector<std::string> res = {name, "王恒", "李四"};
        return res;
    }
};
PYBIND11_MODULE(person_pybind, m) {
py::class_<Person>(m, "person")
.def(py::init<>())
.def(py::init<std::string>())
.def_readwrite("name", &Person::name)
.def("getName", [](Person &m) { return m.getName(); })
.def("printName", [](Person &m) {  })
.def("getList", [](Person &m) { return m.getList(); })
.def("setName", &Person::setName);
}