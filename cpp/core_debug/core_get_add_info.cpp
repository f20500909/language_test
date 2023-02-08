#include <fstream>
#include <map>
#include <memory>
#include <memory.h>
#include <sstream>
#include <string>
using namespace std;

const int max_depth = 100;
int main() {
  ifstream in("./path");
  map<uint64_t, uint64_t> data;
  char line[1024] = {};
  while (!in.getline(line, 1024).eof()) {
    string s = line;
    string key = s.substr(0, s.find(" "));
    string value = s.substr(0, s.find(" ") + 1);
    uint64_t int_key = 0;
    uint64_t int_value = 0;
    stringstream ss;
    ss << hex << key;
    ss >> int_key;
    ss.clear();
    ss << hex << value;
    ss >> int_value;
    data.insert({int_key, int_value});
    memset(line, 0, 1024);
  }
  for (auto& [k, v] : data) {
    uint64_t t_k = k;
    int depth = 0;
    while (data.count(t_k) > 0) {
      t_k = data[t_k];
      depth++;
    }
    t_k = k;
    if (depth == max_depth) {
      while (data.count(t_k)) {
        if (data.count(t_k + 4) > 0) {
          printf("info symbol 0x%llx\n", data[t_k + 4]);
        }
        t_k = data[t_k];
      }
      printf("\r\n");
    }
  }
}