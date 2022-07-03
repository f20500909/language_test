/*


*/
#include <cassert>
#include <cstring>
#include <iostream>
#include <istream>
#include <memory>
#include <ostream>

namespace my_std {

template <class T>
class basic_string {
 public:
  // (constructor) constructs a basic_string

  // (constructor)
  // Construct basic_string object (public member function )
  // (destructor)
  // operator=
  // String assignment (public member function )

  // Iterators:
  // begin
  // Return iterator to beginning (public member function )
  // end
  // Return iterator to end (public member function )
  // rbegin
  // Return reverse iterator to reverse beginning (public member function )
  // rend
  // Return reverse iterator to reverse end (public member function )
  // cbegin
  // Return const_iterator to beginning (public member function )
  // cend
  // Return const_iterator to end (public member function )
  // crbegin
  // Return const_reverse_iterator to reverse beginning (public member function
  // ) crend Return const_reverse_iterator to reverse end (public member
  // function )

  // Capacity:
  // size
  // Return size (public member function )
  // length
  // Return length of string (public member function )
  // max_size
  // Return maximum size (public member function )
  // resize
  // Resize string (public member function )
  // capacity
  // Return size of allocated storage (public member function )
  // reserve
  // Request a change in capacity (public member function )
  // clear
  // Clear string (public member function )
  // empty
  // Test whether string is empty (public member function )
  // shrink_to_fit
  // Shrink to fit (public member function )

  // Element access:
  // operator[]
  // Get character of string (public member function )
  // at
  // Get character of string (public member function )
  // back
  // Access last character (public member function )
  // front
  // Access first character (public member function )

  // Modifiers:
  // operator+=
  // Append to string (public member function )
  // append
  // Append to string (public member function )
  // push_back
  // Append character to string (public member function )
  // assign
  // Assign content to string (public member function )
  // insert
  // Insert into string (public member function )
  // erase
  // Erase characters from string (public member function )
  // replace
  // Replace portion of string (public member function )
  // swap
  // Swap string values (public member function )
  // pop_back
  // Delete last character (public member function )

  // String operations:
  // c_str
  // Get C-string equivalent

  // data
  // Get string data (public member function )

  // get_allocator
  // Get allocator (public member function )

  // copy
  // Copy sequence of characters from string (public member function )

  // find
  // Find first occurrence in string (public member function )

  basic_string();

  basic_string(T t);

  basic_string(T *input);

  basic_string(const T *input);

  // String destructor (public member function )
  basic_string(T t, size_t n);

  // (destructor)
  ~basic_string();

  T &operator[](size_t index);

  const T operator[](size_t index) const;

  basic_string &operator=(const basic_string &str);

  basic_string &operator+(const basic_string &str);

  T at(size_t index);

  const T at(size_t index) const;

  void copy_data(T *input_data, T *copy_start);

  void deallocate();

  void realloc(size_t len);

  size_t get_realloc_size();

  const char *c_str() const;

  size_t capacity();

  const size_t size() const;

  T *begin() const;

  T *end() const;

 private:
  T *_begin = NULL;
  T *_end = NULL;
  size_t _capacity = 0;
  std::allocator<T> _alloc;
};

template <class T>
basic_string<T>::basic_string() {
  // defalut alloc 8 object
  this->realloc(0);
}

template <class T>
basic_string<T>::basic_string(const T *input) : basic_string((T *)input) {}

template <class T>
basic_string<T>::basic_string(T *input) {
  this->realloc(0);
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
  this->realloc(999);
  this->copy_data(str.begin(), this->begin());
}

template <class T>
basic_string<T> &basic_string<T>::operator+(const basic_string<T> &str) {
  // this->realloc(str.size());
  this->copy_data(str.begin(), this->end());
}

template <class T>
void basic_string<T>::copy_data(T *input_data, T *copy_start) {
  assert(copy_start);
  assert(copy_start >= this->begin() && copy_start <= this->end());
  if (NULL == this->begin()) {
    this->realloc(0);
  }

  size_t input_len = strlen(input_data);
  T *cur = copy_start;
  size_t need_size = copy_start - this->begin() + input_len + 1;

  if (need_size > this->capacity()) {
    size_t alloc_len = this->get_realloc_size();

    std::allocator<T> new_alloc;
    T *new_begin = new_alloc.allocate(alloc_len);
    // this->realloc(input_len);

    this->deallocate();

    for (size_t i = 0; i < input_len; i++) {
      new_alloc.construct(cur, input_data[i]);
      cur++;
    }
    this->_alloc = new_alloc;
    this->_begin = new_begin;
    this->_capacity = alloc_len;
  } else {
    for (size_t i = 0; i < input_len; i++) {
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
}

template <class T>
size_t basic_string<T>::get_realloc_size() {
  // 8 16 32 64 128...
  int alloc_len = 8;
  while (this->size() > alloc_len) {
    alloc_len *= 2;
  }
  return alloc_len;
}

template <class T>
void basic_string<T>::realloc(size_t len) {
  this->deallocate();

  size_t alloc_len = this->get_realloc_size();
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
  this->deallocate();
}

template <class T>
const size_t basic_string<T>::size() const {
  return this->end() - this->begin();
}

// Non-member function overloads
// relational operators
// Relational operators for basic_string (function template )
// swap
// Exchanges the values of two strings (function template )

// getline
// Get line from stream into string (function template )

// operator+
// Concatenate strings (function template )
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

using string = basic_string<char>;

};  // namespace my_std

// operator>>
// Extract string from stream (function template )
std::istream &operator>>(std::istream &os, my_std::string &s) {
  s.deallocate();
  // 2.通过cin添加新的字符串
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
  while (b != e) {
    os << *b;
    b++;
  }
  return os;
}

int main() {
  my_std::string str1;
  std::cout << "str1:" << str1 << std::endl;

  my_std::string str2("hello!");
  std::cout << "str2:" << str2 << std::endl;

  str1 = str2;
  std::cout << "str1:" << str1 << std::endl;

  std::cout << "str1==str2 :" << std::boolalpha << (str1 == str2) << std::endl;
  str1 = 'b';
  std::cout << "str1==str2 :" << std::boolalpha << (str1 == str2) << std::endl;

  std::cout << "[hello!] str2[0]:" << str2[0] << std::endl;
  std::cout << "[hello!] str2[1]:" << str2[1] << std::endl;

  std::cout << "[hello!] str1.size(): " << str2.size() << std::endl;

  std::cout << "[hello!] str.capacity(): " << str2.capacity() << std::endl;

  str2 = "world";
  std::cout << "str2.: " << str2 << " str2.capacity(): " << str2.capacity()
            << std::endl;

  my_std::string str3;
  std::cin >> str3;

  std::cout << "cin: str3 " << str3 << std::endl;
  return 0;
}