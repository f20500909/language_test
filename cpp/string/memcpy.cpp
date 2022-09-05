#include <stdio.h>
#include <iostream>
#include <memory>
#include <string.h>
#include <stdlib.h>
#include <cassert>

////方法一：不考虑内存重叠
//void* my_memcpy(void *dest, const void *src, size_t size) {
//    if (dest == nullptr || src == nullptr) return nullptr;
//    void *res = dest;
//    while (size--) {
//        *(char *) dest = *(char *) src;
//        dest = (char *) dest + 1;
//        src = (char *) src + 1;
//    }
//    return res;
//}


void *memcpy1(void *dest, const void *src, int size) {
    if (dest == nullptr || src == nullptr) return nullptr;

    char *d = (char *) (dest);
    char *s = (char *) (src);
    while (size--) {
        *d++ = *s++;
    }

    return dest;
}

void *my_memcpy(char *dest, const char *src, size_t n) {
    assert(nullptr != dest);
    assert(nullptr != src);
    char *res = (char *) dest;

    //目标地址
    char *p = (char *) dest;
    //源地址
    const char *q = (const char *) src;

    //   p目的    q源地址
    //   q源地址     .........很长    p目的
    if (p <= q || p >= q + n) {
        while (n--) {
            *(p++) = *(q++);
        }
        //源地址   目的地址 (小于n)  目的地址在后,会污染源地址
    } else {
        p = p + n - 1;
        q = q + n - 1;
        while (n--) {
            *(p--) = *(q--);
        }
    }
    return res;
}

int main(void) {
    char p1[256] = "hello,world!";
    char p2[256] = {0};
    memcpy(p2, p1, strlen(p1) + 1);

    printf("%s\n", p2);

    //出错
//	my_memcpy(NULL,p1,strlen(p1)+1);
    //出错
//	my_memcpy(p2,NULL,strlen(p1)+1);

    my_memcpy(p1 + 1, p1, strlen(p1) + 1);
    printf("%s\n", p1);

    my_memcpy(p1, p1 + 1, strlen(p1) + 1);
    printf("%s\n", p1);
    return 0;
}
















