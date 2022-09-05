#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

int main() {
	int pid = fork();
	if (pid > 0)
		exit(0);
	if (pid < 0) {
		cout << "子进程创建失败" << endl;
		exit(0);
	}
	//设置新会话
	setsid();
	//修改掩码
	umask(0);
	//修改工作目录
	chdir("/");
	//运行
	int fd = open("/home/cs/桌面/0917/msg", O_WRONLY | O_CREAT, 0644);
	int i = 1;
	while (1 < 1000) {
		write(fd, "demoen", 4);
		sleep(1);
		i++;
		lseek(fd, 1000, SEEK_CUR);
	}

}
