/*
reference: https://cplusplus.com/reference/string/basic_string/
*/
#include <cassert>
#include <cstring>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>

namespace my_std {

static const size_t npos = -1;
static const size_t STRING_INIT_SIZE = 8;

template <class T>
class basic_string {
 public:
  //  Construct string object
  basic_string();

  basic_string(T t);

  basic_string(T *input);

  basic_string(const T *input);

  // String destructor
  basic_string(T t, size_t n);

  // destructor
  ~basic_string();

  T &operator[](size_t index);

  const T operator[](size_t index) const;

  // String assignment
  basic_string &operator=(const basic_string &str);

  // Concatenate strings
  basic_string &operator+(const basic_string &str);

  // Get character in string
  T at(size_t index);

  // Get character in string
  const T at(size_t index) const;

  // Get C string equivalent
  const char *c_str() const;

  // Find first occurrence in string
  int find(const basic_string &input);

  // Return size of allocated storage
  size_t capacity();

  // Return length of string
  const size_t size() const;

  // Iterators:

  // Return iterator to beginning
  T *begin() const;

  // Return iterator to end
  T *end() const;

  // Generate substring
  basic_string substr(size_t pos = 0, size_t len = npos);

  // Extract string from stream
  friend std::istream &operator>>(std::istream &os, basic_string &s);

  // Insert string into stream
  friend std::ostream &operator<<(std::ostream &os, const basic_string &s);

 private:
  void copy_data(T *input_data, T *copy_start, size_t len = npos);

  void deallocate();

  void realloc(size_t len);

  size_t get_realloc_size(size_t len);

  T *_begin = NULL;
  T *_end = NULL;
  size_t _capacity = 0;
  std::allocator<T> _alloc;
};

template <class T>
basic_string<T>::basic_string() {
  this->realloc(STRING_INIT_SIZE);
}

template <class T>
basic_string<T>::basic_string(const T *input) : basic_string((T *)input) {}

template <class T>
basic_string<T>::basic_string(T *input) {
  this->realloc(STRING_INIT_SIZE);
  copy_data(input, this->begin());
  return;
}

template <class T>
basic_string<T>::basic_string(T t) : basic_string(t, 1) {}

template <class T>
basic_string<T>::basic_string(T t, size_t n) {
  this->realloc(n);
  return;
}

template <class T>
T &basic_string<T>::operator[](size_t index) {
  assert(index < this->size());
  return this->_begin[index];
}

template <class T>
size_t basic_string<T>::capacity() {
  return this->_capacity;
}

template <class T>
int basic_string<T>::find(const basic_string &input) {
  size_t i = 0;
  for (int i = 0; i < this->size() - input.size() + 1; i++) {
    for (int j = 0; j < input.size(); j++) {
      if (this->at(i + j) != input[j]) {
        break;
      }

      if (j == input.size() - 1) {
        return i;
      }
    }
  }
  return -1;
}

template <class T>
const T basic_string<T>::operator[](size_t index) const {
  assert(index < this->size());
  return this->_begin[index];
}

template <class T>
const T basic_string<T>::at(size_t index) const {
  assert(index < this->size());
  return this->_begin[index];
}

template <class T>
T basic_string<T>::at(size_t index) {
  assert(index < this->size());
  return this->_begin[index];
}

template <class T>
basic_string<T> &basic_string<T>::operator=(const basic_string<T> &str) {
  this->realloc(STRING_INIT_SIZE);
  this->copy_data(str.begin(), this->begin());
}

template <class T>
basic_string<T> &basic_string<T>::operator+(const basic_string<T> &str) {
  this->copy_data(str.begin(), this->end());
}

template <class T>
void basic_string<T>::copy_data(T *input_data, T *copy_start, size_t len) {
  assert(copy_start);
  assert(copy_start >= this->begin() && copy_start <= this->end());

  if (len == npos) {
    len = strlen(input_data);
  }
  T *cur = copy_start;
  size_t need_size = copy_start - this->begin() + len + 1;

  // need realloc ?
  if (need_size > this->capacity()) {
    size_t alloc_len = this->get_realloc_size(need_size);

    std::allocator<T> new_alloc;
    T *new_begin = new_alloc.allocate(alloc_len);

    this->deallocate();

    cur = new_begin;
    for (size_t i = 0; i < len; i++) {
      new_alloc.construct(cur, input_data[i]);
      cur++;
    }
    this->_alloc = new_alloc;
    this->_begin = new_begin;
    this->_capacity = alloc_len;
  } else {
    for (size_t i = 0; i < len; i++) {
      this->_alloc.construct(cur, input_data[i]);
      cur++;
    }
  }
  this->_end = cur;
  this->_alloc.construct(this->_end, '\0');
  this->_end++;

  return;
}

template <class T>
void basic_string<T>::deallocate() {
  this->_alloc.deallocate(this->begin(), this->capacity());
  this->_begin = NULL;
  this->_end = NULL;
  this->_capacity = 0;

  this->_begin = _alloc.allocate(STRING_INIT_SIZE);
  this->_end = this->begin();
  this->_capacity = STRING_INIT_SIZE;
}

template <class T>
size_t basic_string<T>::get_realloc_size(size_t len) {
  // 8 16 32 64 128...
  size_t res = STRING_INIT_SIZE;
  while (res <= len) {
    res *= 2;
  }
  return res;
}

template <class T>
void basic_string<T>::realloc(size_t len) {
  this->deallocate();

  size_t alloc_len = this->get_realloc_size(len);
  this->_begin = _alloc.allocate(alloc_len);
  this->_end = this->begin();
  this->_capacity = alloc_len;
  return;
}

template <class T>
const char *basic_string<T>::c_str() const {
  return this->begin();
};

template <class T>
basic_string<T>::~basic_string() {
  this->_alloc.deallocate(this->begin(), this->capacity());
  this->_begin = NULL;
  this->_end = NULL;
  this->_capacity = 0;
}

template <class T>
const size_t basic_string<T>::size() const {
  if (this->end() == this->begin()) {
    return 0;
  }

  return this->end() - this->begin() - 1;
}

template <class T>
bool operator==(const basic_string<T> &a, const basic_string<T> &b) {
  if (a.size() != b.size()) {
    return false;
  }
  size_t i = 0;
  while (i < a.size()) {
    if (a[i] != b[i]) {
      return false;
    }
    i++;
  }
  return true;
}

template <class T>
bool operator!=(const basic_string<T> &a, const basic_string<T> &b) {
  return !(a == b);
}

template <class T>
bool operator<(const basic_string<T> &a, const basic_string<T> &b) {
  size_t i = 0;
  while (i < a.size() && i < b.size()) {
    if (a[i] > b[i]) {
      return true;
    } else if (a[i] < b[i]) {
      return false;
    }
  }
  return a.size() > b.size();
}

template <class T>
bool operator>(const basic_string<T> &a, const basic_string<T> &b) {
  return !(a > b);
}

template <class T>
T *basic_string<T>::begin() const {
  return this->_begin;
}

template <class T>
T *basic_string<T>::end() const {
  return this->_end;
}

template <class T>
basic_string<T> basic_string<T>::substr(size_t pos, size_t len) {
  basic_string res;
  if (len == npos) {
    len = this->size();
  }
  res.realloc(len + 1);

  res.copy_data(this->begin() + pos, res.begin(), len);
  return res;
}

using string = basic_string<char>;

// Extract string from stream (function template )
std::istream &operator>>(std::istream &os, my_std::string &s) {
  s.deallocate();
  char temp_str[4096] = {0};

  os >> temp_str;
  int len = strlen(temp_str);
  s.copy_data(temp_str, s.begin());

  return os;
}

// operator<<
// Insert string into stream (function template )
std::ostream &operator<<(std::ostream &os, const my_std::string &s) {
  const char *b = s.begin();
  const char *e = s.end();
  assert(b <= e);
  while (b != e) {
    os << *b;
    b++;
  }
  return os;
}

};  // namespace my_std

int main() {
  my_std::string str1;
  std::cout << "str1:" << str1 << std::endl;

  my_std::string str2("hello!");
  std::cout << "str2:" << str2 << std::endl;

  str1 = str2;
  std::cout << "str1:" << str1 << std::endl;

  std::cout << "['hello!'=='hello!' ?] str1==str2 :" << std::boolalpha
            << (str1 == str2) << std::endl;
  str1 = 'b';
  std::cout << "['hello!'=='b' ?] str1==str2 :" << std::boolalpha
            << (str1 == str2) << std::endl;

  std::cout << "['hello!'] str2[0]:" << str2[0] << std::endl;
  std::cout << "['hello!'] str2[1]:" << str2[1] << std::endl;

  std::cout << "['hello!'] str1.size(): " << str2.size() << std::endl;

  std::cout << "['hello!'] str.capacity(): " << str2.capacity() << std::endl;

  std::cout << "['hello!'] str.find(\"haha\"): " << str2.find("haha")
            << std::endl;

  std::cout << "['hello!'] str.find(\"ello\"): " << str2.find("ello")
            << std::endl;

  std::cout << "['hello!'] str.strstr(1,3): " << str2.substr(1, 3) << std::endl;

  str2 = "worldxxxxxxxxxxxxxx";
  std::cout << "['worldxxxxxxxxxxxxxx'] str2: " << str2
            << " str2.size(): " << str2.size()
            << " str2.capacity(): " << str2.capacity() << std::endl;

  std::cout << "input data to test : func: operator>>" << std::endl;
  ;
  my_std::string str3;
  std::cin >> str3;

  std::cout << "cin str3 res:  " << str3 << std::endl;
  return 0;
}