#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main()
{
	mkfifo("fifo",0660);   //创建一个命令管道，属主和用户组具有读写权限
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		char buf[256];
		int fd=open("fifo",O_RDONLY);   //子进程读管道中的数据
		read(fd,buf,10);
		buf[10]=0;
		printf("%s",buf);
		close(fd);
		exit(0);
	}
	int fd=open("fifo",O_WRONLY);    //父进程向管道写入数据
	write(fd,"fifo test\n",10);
	close(fd);
	return 0;
}
