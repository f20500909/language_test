/* File Name: client.c */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/types.h>
#include <arpa/inet.h>
#include <string>

#define MAXLINE 4096


int main(int argc, char **argv) {
	int sockfd, n, rec_len;
	char buf[MAXLINE];
	struct sockaddr_in servaddr;

	char ip[] = "127.0.0.1";
	argv[1] = ip;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(8000);

	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		printf("inet_pton error for %s\n", argv[1]);
		exit(0);
	}


	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}


	printf("send msg to server: \n");


	const char *sendline = "dsfsdf";
	if (send(sockfd, sendline, strlen(sendline), 0) < 0) {
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	if ((rec_len = recv(sockfd, buf, MAXLINE, 0)) == -1) {
		perror("recv error");
		exit(1);
	}
	buf[rec_len] = '\0';
	printf("Received : %s ", buf);
	close(sockfd);
	exit(0);
}
