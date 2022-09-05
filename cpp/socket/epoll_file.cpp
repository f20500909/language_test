//==============================================================================
//  Copyright (C) 2019 王小康. All rights reserved.
//
//  作者: 王小康
//  描述: epoll用法举例
//  日期: 2019-04-30
//
//==============================================================================

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>


static void function_key(short code, short flag){
	printf("\e[31m key  : %d %d \e[0m\n", code, flag);
}

static void function_move(short x, short y){
	printf("\e[32m move : %d %d \e[0m\n", x, y);
}

static void function_wheel(short flag){
	printf("\e[33m wheel: %d\e[0m\n", flag);
}

////////////////////////////////////////////////////////////////////////////////

static void readInputEvent(int fd){
	struct input_event inputEvent[32];
	int length = read(fd, inputEvent, sizeof(inputEvent));
	if(length <= 0) return;
	length /= sizeof(struct input_event);

	int i;
	short x = 0, y = 0;
	for(i=0; i<length; i++){
		if(inputEvent[i].type == EV_KEY){
			function_key(inputEvent[i].code, inputEvent[i].value ? 1 : 0);
		}
		else if(inputEvent[i].type == EV_REL){
			if(inputEvent[i].code == REL_X){
				x += inputEvent[i].value;
			}
			else if(inputEvent[i].code == REL_Y){
				y += inputEvent[i].value;
			}
			else if(inputEvent[i].code == REL_WHEEL){
				function_wheel(inputEvent[i].value > 0 ? 1 : 0);
			}
		}
	}
	if(x || y){
		function_move(x, y);
	}
}

static int openInputEvent(int epollFd, char *name){
	char nameBuffer[32];
	snprintf(nameBuffer, 32, "/dev/input/%s", name);
	int fd = open(nameBuffer, O_RDWR);
	printf("open(%s) = %d\n", nameBuffer, fd);
	if(fd >= 0){
		struct epoll_event epollEvent;
		epollEvent.events = EPOLLIN | EPOLLERR;    //监视可读事件和错误事件
		epollEvent.data.fd = fd;                   //文件描述符放入私有数据中
		int err = epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &epollEvent);
		if(err < 0){
			close(fd);
			return 0;
		}
		else{
			return 1;
		}
	}
	else{
		return 0;
	}
}

int main(int argc, char* argv[]){

	//创建epoll实例
	int epollFd = epoll_create(8);
	if(epollFd < 0) return -1;

	//打开目录 /dev/input 下所有 event*，并添加到epoll。
	int n = 0;
	struct dirent *ent;
	DIR *dir = opendir("/dev/input");
	if(dir == NULL){
		close(epollFd);
		return -2;
	}
	while((ent = readdir(dir))){
		if(memcmp(ent->d_name, "event", 5) == 0){
			n += openInputEvent(epollFd, ent->d_name);
		}
	}
	closedir(dir);

	//检查添加了几个文件描述符到epoll
	if(n < 1){
		close(epollFd);
		return -3;
	}

	//等待可读的文件描述符，200次后退出。
	int err;
	struct epoll_event epollEvent;
	for(n=0; n<200; n++){
		epollEvent.events = 0;
		epollEvent.data.fd = 0;
		err = epoll_wait(epollFd, &epollEvent, 1, 1000);
		if(err < 0) break;
		if(err == 0) continue;
		if(epollEvent.events & EPOLLIN){
			readInputEvent(epollEvent.data.fd);
		}
		if(epollEvent.events & EPOLLERR){
			close(epollEvent.data.fd);
		}
	}

	close(epollFd);
	return 0;
}
