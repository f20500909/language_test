#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
	pid_t pid;
	int fd[2], i, n;
	char chr;
	pipe(fd);
	pid = fork();
	if (pid == 0)          //子进程
	{
		close(fd[1]);
		for (i = 0; i < 10; i++) {
			read(fd[0], &chr, 1);
			printf("%c\n", chr);
		}
		close(fd[0]);
		exit(0);
	}
	close(fd[0]);           //父进程
	for (i = 0; i < 10; i++) {
		chr = 'a' + i;
		write(fd[1], &chr, 1);
		sleep(0.3);
	}
	close(fd[1]);
	return 0;
}
