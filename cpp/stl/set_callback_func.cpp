#include <iostream>
#include <vector>
using namespace std;
class A {
 private:
  int a = 5;
  vector<void (*)(int)> funcs;

 public:
  void setA(int a_);
  void registerCallback(void (*p)(int));
};

void A::setA(int a_) {
  a = a_;
  for (int i = 0; i < funcs.size(); ++i) {
    funcs[i](a);
  }
}

void A::registerCallback(void (*p)(int)) { funcs.push_back(p); }

void display(int a) { cout << "a=" << a << endl; }

int main() {
  A a1;
  a1.registerCallback(display);
  a1.setA(7);
  system("pause");
  return 0;
}