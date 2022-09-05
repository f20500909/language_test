#include <iostream>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

int my_strcmp(const char *str1, const char *str2) {
    assert(*str1 != NULL && *str2 != NULL);
    while (*str1 != '\0' && *str1 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }
    if (*str1 > *str2)
        return 1;
    else if (*str1 < *str2)
        return -1;
    else
        return 0;
}

int my_string(const char *str1, const char *str2) {
    assert(nullptr != str1);
    assert(nullptr != str2);
    while (*str1 != '\0' && *str1 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }
    if (*str1 > *str2) return 1;
    if (*str1 < *str2) return -1;
    return 0;

}


int main34325() {
    char *str1 = "sdfdf";
    char *str2 = "sdf";
    cout << my_strcmp(str1, str2) << endl;
    return 0;
}