#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
int main(){
	int result=-1;
	int fd[2];
	int nbytes;
	pid_t pid;
	char* string =" hi pipe";
	char readbuffer[80];
	int* write_fd=&fd[1];
	int* read_fd=&fd[0];
	if(-1==pipe(fd)){
		printf("建立消息队列失败\n");
		return -1;
	}
	pid=fork();

	std::cout<<"pid id "<<pid<<std::endl;
	if(0==pid){
		close(*read_fd);
		result=write(*write_fd,string ,strlen(string));
		return 0;
	}
	else{
		close(*write_fd);
		nbytes=read(*read_fd,readbuffer, sizeof(readbuffer));
		printf("got info %d  ,the context is %s\n",nbytes,readbuffer);
		return 0;
	}




	return 0;
}

