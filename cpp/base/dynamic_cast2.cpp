#include <iostream>
using namespace std;
class Base {
 public:
  virtual void foo() { cout << "Base foo" << endl; }
};
class Derived : public Base {
 public:
  virtual void foo() override { cout << "Derived foo" << endl; }
  void bar() { cout << "Derived bar" << endl; }
};
int main() {
  Base *b = new Base();
  Base &&bref = Base();
  try {
    dynamic_cast<Derived &>(bref).bar();  // crash
  } catch (exception &e) {
    cout << e.what() << endl;
  }
  Derived *dptr = dynamic_cast<Derived *>(b);
  if (!dptr) {
    dptr->bar();  // doesn't crash
  } else {
    cout << "crash" << endl;
  }
}