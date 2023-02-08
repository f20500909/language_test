
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <map>
#include <ostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

class A {
 public:
  A() {
    std::cout << "A construct..." << std::endl;
    ptr_ = new int(100);
  }

  A(const A &a) {
    std::cout << "A copy construct ..." << std::endl;
    ptr_ = new int();
    memcpy(ptr_, a.ptr_, sizeof(int));
  }

  ~A() {
    std::cout << "A deconstruct ..." << std::endl;
    if (ptr_) {
      delete ptr_;
    }
  }

  A &operator=(const A &a) {
    std::cout << " A operator= ...." << std::endl;
    return *this;
  }

  int *getVal() { return ptr_; }

 private:
  int *ptr_;
};

class Move_A {
 public:
  Move_A() {
    std::cout << "Move_A construct..." << std::endl;
    ptr_ = new int(100);
  }

  Move_A(const Move_A &a) {
    std::cout << "Move_A copy construct ..." << std::endl;
    ptr_ = new int();
    memcpy(ptr_, a.ptr_, sizeof(int));
  }

  ~Move_A() {
    std::cout << "Move_A deconstruct ..." << std::endl;
    if (ptr_) {
      delete ptr_;
    }
  }

  Move_A &operator=(const Move_A &a) {
    std::cout << " Move_A operator= ...." << std::endl;
    return *this;
  }

  Move_A(Move_A &&a) {
    std::cout << "Move_A move construct ..." << std::endl;
    ptr_ = a.ptr_;
    a.ptr_ = nullptr;
  }

  int *getVal() { return ptr_; }

 private:
  int *ptr_;
};

int main(int argc, char *argv[]) {
  std::vector<A> vec;
  vec.push_back(A());

  std::cout << " ---------------- " << std::endl;
  std::vector<Move_A> move_vec;
  move_vec.push_back(Move_A());
}
