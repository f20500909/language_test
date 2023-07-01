#include <iostream>

class Counter {
 public:
  Counter() : s(0), w(0){};
  int s;
  int w;
};

template <class T>
class WeakPtr;  // 为了用weak_ptr的lock()，来生成share_ptr用，需要拷贝构造用
template <class T>

class SharePtr {
 public:
  SharePtr(T* p = 0) : _ptr(p) {
    cnt = new Counter();
    if (p) cnt->s = 1;
    cout << "SharePtr construct " << cnt->s << endl;
  }
  ~SharePtr() { release(); }

  SharePtr(SharePtr<T> const& s) {
    cout << "in copy con" << endl;
    _ptr = s._ptr;
    (s.cnt)->s++;
    cout << "copy construct" << (s.cnt)->s << endl;
    cnt = s.cnt;
  }
  SharePtr(WeakPtr<T> const&
               w)  // 为了用weak_ptr的lock()，来生成share_ptr用，需要拷贝构造用
  {
    cout << "in w copy con " << endl;
    _ptr = w._ptr;
    (w.cnt)->s++;
    cout << "copy w  construct" << (w.cnt)->s << endl;
    cnt = w.cnt;
  }
  SharePtr<T>& operator=(SharePtr<T>& s) {
    if (this != &s) {
      release();
      (s.cnt)->s++;
      cout << "assign construct " << (s.cnt)->s << endl;
      cnt = s.cnt;
      _ptr = s._ptr;
    }
    return *this;
  }
  T& operator*() { return *_ptr; }
  T* operator->() { return _ptr; }
  friend class WeakPtr<T>;  // 方便weak_ptr与share_ptr设置引用计数和赋值。
 public:
  void release() {
    cnt->s--;
    cout << "into SharePtr release " << cnt->s << endl;
    if (cnt->s < 1) {
      delete _ptr;
      cout << "SharePtr release " << cnt->s << endl;
      if (cnt->w < 1) {
        delete cnt;
        cnt = NULL;
      }
    }
  }
  T* _ptr;
  Counter* cnt;
};
template <class T>
class WeakPtr {
 public:  // 给出默认构造和拷贝构造，其中拷贝构造不能有从原始指针进行构造
  WeakPtr() {
    _ptr = 0;
    cnt = 0;
    cout << "WeakPtr construct " << endl;
  }
  WeakPtr(SharePtr<T>& s) : _ptr(s._ptr), cnt(s.cnt) {
    cout << "w con s" << endl;
    cnt->w++;
  }
  WeakPtr(WeakPtr<T>& w) : _ptr(w._ptr), cnt(w.cnt) { cnt->w++; }
  ~WeakPtr() { release(); }
  WeakPtr<T>& operator=(WeakPtr<T>& w) {
    if (this != &w) {
      release();
      cnt = w.cnt;
      cnt->w++;
      _ptr = w._ptr;
    }
    return *this;
  }
  WeakPtr<T>& operator=(SharePtr<T>& s) {
    cout << "w = s" << endl;
    release();
    cnt = s.cnt;
    cnt->w++;
    _ptr = s._ptr;
    return *this;
  }
  SharePtr<T> lock() { return SharePtr<T>(*this); }
  bool expired() {
    if (cnt) {
      if (cnt->s > 0) {
        cout << "empty " << cnt->s << endl;
        return false;
      }
    }
    return true;
  }
  friend class SharePtr<T>;  // 方便weak_ptr与share_ptr设置引用计数和赋值。
 private:
  void release() {
    cout << "into WeakPtr release" << endl;
    if (cnt) {
      cnt->w--;
      if (cnt->w < 1 && cnt->s < 1) {
        cout << "weakptr release" << endl;
        cnt = NULL;
      }
    }
  }
  T* _ptr;
  Counter* cnt;
};

class parent;
class child;
class parent {
 public:
  // SharePtr<child> ch;
  WeakPtr<child> ch;
};
class child {
 public:
  SharePtr<parent> pt;
};




using namespace std;
void test() {
  SharePtr<parent> ft(
      new parent());  //  s2    w0           s2 引用计数到了2 没有销毁
  SharePtr<child> son(
      new child());  //  s1    w0           s2 引用计数到了2 没有销毁

  ft->ch = son;  // 左w右s，右边的w--，右边w++
  son->pt = ft;  // 左s右s，左边s--销毁，右边s++
  cout << "-------------" << endl;
}

int main() {
  test();
  // SharePtr<child> son2=(ft->ch).lock();
  cin.get();
  return 0;
}