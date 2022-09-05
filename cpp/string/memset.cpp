#include <stdio.h>
#include <string.h>
#include <cassert>

//void *memcpy(void *dest, const void *src, size_t size) {
//	assert(dest);
//	assert(src);
//	char *str1 = (char *) dest;
//	const char *str2 = (const char *) src;
//	while (size) {
//		*str1 = *str2;
//		str1++;
//		str2++;
//		size--;
//	}
//	return dest;
//}

//void *memSet(void *dest, int c, size_t size) {
//	assert(dest);
//	char *pdest = (char *) dest;
//	while (size--) {
//		*pdest++ = c;
//	}
//	return dest;
//}
void *memSet(void *dest,const int i,size_t size){
	assert(dest);
	char* cur=(char*) dest;
	while(size--){
		*cur++=i;
	}
	return dest;
}


int main3425() {
	char buf[10];
	int i;
//	printf("%d\n", sizeof(buf));
	for (i = 0; i < 10; i++)
		printf("buf[%d] = %c\n", i, buf[i]);
	printf("------------------------------------\n");
	memSet(buf, 64, sizeof(buf));
	for (i = 0; i < 10; i++)
		printf("buf[%d] = %c\n", i, buf[i]);
	return 0;
}