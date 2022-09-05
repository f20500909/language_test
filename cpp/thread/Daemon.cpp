#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>

using namespace std;
#define MAXFILE 65535

//实现一个守护进程:每隔5秒在/tmp/dameon.log中写入一句话
int main() {
	pid_t pc;
	int i, fd, len;
	//char *buf="this is a Dameon\n";
	string buf = "this is a Dameon\n";
	//len = strlen(buf);
	len = buf.size();
	pc = fork(); /*第一步:创建子进程*/
	if (pc < 0) {
		printf("error fork\n");
		exit(1);
	} else if (pc > 0) {//父进程退出
		exit(0);
	}
	setsid(); /*第二步:在子进程中创建新会话*/
	char szPath[1024];
	if (getcwd(szPath, sizeof(szPath)) == NULL) {//获得当前路径
		perror("getcwd");
		exit(1);
	}
	//printf("current working directory : %s\n", szPath);
	//chdir("/"); /*第三步:改变当前目录为根目录*/
	chdir(szPath);
	umask(0); /*第四步:重设文件权限掩码*/
	for (i = 0; i < MAXFILE; i++) /*第五步:关闭文件描述符*/
		close(i);
	while (1) {
		//if((fd=open("/tmp/dameon.txt",O_CREAT|O_WRONLY|O_APPEND,0600))<0){
		if ((fd = open("dameon.txt", O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
			perror("open");
			exit(1);
		}
		//write(fd,buf,len+1);
		write(fd, buf.c_str(), len);//写入文件
		close(fd);
		sleep(5);
	}
	return 0;
}
