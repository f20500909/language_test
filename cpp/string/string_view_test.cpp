#include <string>
#include <string_view>

void* operator new(size_t n) {
  void* p = malloc(n);
  //   printf("new :%p \r\n", p);
  return p;
}

void operator delete(void* p) {
  //   printf("delete :%p  %s\r\n", p, (char*)p);

  free(p);
  p = nullptr;
  return;
}

std::string get_str() {
  //   printf("get_str =========\r\n");
  std::string str = "abcdexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx111111111111111111";
  //   printf("get_str end =========\r\n");
  return str;
}

void test_func() {
  //   printf("test_func =========\r\n");

  std::string_view str = get_str();

  if (std::string(str.data()) !=
      "abcdexxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx111111111111111111") {
    printf("%s\r\n", str);
  }

  //   printf("test_func end =========\r\n");
}

int main() {
  for (int i = 0; i < 100000; i++) {
    test_func();
  }
  return 0;
}



