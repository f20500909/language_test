#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>
//#include <fcntl.h>

using namespace std;
#define FILELENGTH 80

int main() {
	int fd = -1;
	char buf[] = "the context for write";
	char *ptr = nullptr;
	fd = open("sdfsd", O_RDWR, S_IRWXU);
	if (-1 == fd) return -1;
	lseek(fd, FILELENGTH - 1, SEEK_SET);
	write(fd, "a", 1);
	ptr = (char *) mmap(nullptr, FILELENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if ((char *) -1 == ptr) {
		printf("mmap failure\n");
		close(fd);
		return -1;
	}
	memcpy(ptr + 16, buf, strlen(buf));
	munmap(ptr, FILELENGTH);
	close(fd);
	return 0;
}






