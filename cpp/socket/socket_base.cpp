#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 8888
#define BACKLOG 2
#define BUFFLEN 1024


int main() {
	int s_s, s_c;
	sockaddr_in sever_addr;
	sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	int err;

	time_t now;

	char buff[BUFFLEN];

	//build socket
	s_s = socket(AF_INET, SOCK_STREAM, 0);
	if (s_s < 0) {
		printf("socket err\n");
		return -1;
	}

	memset(&sever_addr, 0, sizeof(sever_addr));

	sever_addr.sin_family = AF_INET;
	sever_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sever_addr.sin_port = htons(PORT);
	err = bind(s_s, reinterpret_cast<sockaddr *>(&sever_addr), sizeof(sever_addr));

	if (err < 0) {
		printf("bind err\n");
		return -1;
	}
	err = listen(s_s, BACKLOG);

	if (err < 0) {
		printf("listen err\n");
		return -1;
	}
	while (1) {
		s_c = accept(s_s, reinterpret_cast<sockaddr *>(&client_addr), &len);
		memset(buff, 0, BUFFLEN);
		err = recv(s_c, buff, BUFFLEN, 0);
		if (err > 0 && !strncmp(buff, "TIME", 4)) {
			memset(buff, 0, BUFFLEN);
			now = time(nullptr);
			sprintf(buff, "%24s\r\n", ctime(&now));
			send(s_c, buff, strlen(buff), 0);
		}
		close(s_c);
	}
	close(s_s);
	return 0;
}
