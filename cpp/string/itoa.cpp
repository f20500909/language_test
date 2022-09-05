#include <stdio.h>
#include <stdlib.h>
#include <cstring>


int my_atoi(const char *str) {
//    long long res = 0;
//    if (nullptr == str) return res;
//    int len = strlen(str);
//    bool neg=true;
//    int i=0;
//    while()
//

}


int main() {
    int a;
    char *ptr1 = "-12345";

//	a = atoi(ptr1);
    a = my_atoi(ptr1);

    printf("a = %d", a);

    return 0;
}
