
#include <iostream>
using namespace std;

class Handler {
 public:
  Handler() {}
  virtual ~Handler() {}
  
  void SetNextHandler(Handler *successor) { m_pNextHandler = successor; }
  
  virtual void handleRequest(int days) = 0;

 protected:
  Handler *m_pNextHandler = nullptr;  // 后继者
};

//具体处理者、主管
class Director : public Handler {
 public:
  //处理请求
  virtual void handleRequest(int days) {
    if (days <= 1) {
      cout << "我是主管，有权批准一天假，同意了！" << endl;
    } else {
      m_pNextHandler->handlerequest(days);
    }
  }
};

//具体处理者、经理
class Manager : public Handler {
 public:
  //处理请求
  virtual void handleRequest(int days) {
    if (days <= 3) {
      cout << "我是经理，有权批准三以下的假，同意了！" << endl;
    } else {
      m_pNextHandler->handleRequest(days);
    }
  }
};

//具体处理者、老板
class Boss : public Handler {
 public:
  //处理请求
  virtual void handleRequest(int days) {
    if (days <= 7) {
      cout << "我是老板，最多让你请7天假，同意了！" << endl;
    } else {
      cout << "你请的假事假太长了，不同意！" << endl;
    }
  }
};

//场景
int main() {
  Handler *director = new Director;
  Handler *manager = new Manager;
  Handler *boss = new Boss;

  //设置责任链
  director->SetNextHandler(manager);
  manager->SetNextHandler(boss);

  director->handleRequest(1);
  director->handleRequest(2);
  director->handleRequest(5);
  director->handleRequest(8);
  return 0;
}