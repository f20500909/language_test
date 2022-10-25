#include <iostream>
using namespace std;

class Base {
 public:
  Base(){};

  virtual void Show() { cout << "This is Base calss"<<endl; }
};

class Derived : public Base {
 public:
  Derived(){};

  void Show() { cout << "This is Derived class"<<endl; }
};

int main() {
  //这是第一种情况
  Base *base = new Derived;
  base->Show();
  if (Derived *der = dynamic_cast<Derived *>(base)) {
    cout << "first success .." << endl;
    der->Show();
    cout << endl;
  }
  //这是第二种情况
  Base *base1 = new Base;
  if (Derived *der1 = dynamic_cast<Derived *>(base1)) {
    cout << "second success..." << endl;
    der1->Show();
  } else {
    cout << "second fail " << endl;
  }

  delete (base);
  delete (base1);
  system("pause");
}