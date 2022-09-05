#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>

using namespace std;

int main() {
	fd_set rd;
	struct timeval tv;
	int err;
	FD_ZERO(&rd);
	FD_SET(0, &rd);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	err = select(1, &rd, nullptr, nullptr, &tv);
	if (-1 == err) perror("select()");
	else if (err) printf("Data is available now. \n");
	else printf("No data within five seconds .\n");
	return 0;
}

