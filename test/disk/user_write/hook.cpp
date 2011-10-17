/***************************************************************************
 * 
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include "bhook.h"

int getfilename(int fd, char *buf, int bufsize)
{
	char path[256];
	char tmp[256];

	int pid = getpid();
    int len = snprintf(path, sizeof(path) , "/proc/%d/fd/%d", pid, fd);
	if( readlink(path, tmp, sizeof(tmp)) == -1)
	{
		return -1;
	}
	char *p = strrchr(tmp, '/') + 1;
	strncpy(buf, p, bufsize);
	if(buf[strlen(buf)-1] == '*') {
		buf[strlen(buf)-1] = '\0';
	}

	return 0;
}

ssize_t my_write(int fd, const void *buf, size_t count)
{
	//获取真实地址
	typedef ssize_t (*REALFUNC)(int fd, const void *buf, size_t count);
	REALFUNC real_write = (REALFUNC)get_real_func("write");

	//忽略标准输入/输出
	if(fd >=0 && fd <= 2) {
		return real_write(fd, buf, count);
	}
	//通过fd获取文件名并过滤
	char filename[256];
	getfilename(fd, filename, sizeof(filename));
	char *fname = "data.txt";
	//判断文件名是否为"data.txt"，如果不是则调用原函数
	if(strncmp(filename, fname, strlen(fname)) != 0) {
		return real_write(fd, buf, count);
	}

	//考虑到printf实际内部会调用write函数，避免循环调用，执行printf前先detach之后再attach
	detach_func("write");
	printf("[%s] fd:%d(%s), buf:%s\n", __FUNCTION__, fd, filename, buf);
	attach_func("write", (void *)my_write);

	errno = ENOSPC;
	return -1;
}

void __attribute__ ((constructor)) hook_init(void)
{
	//printf("hook_init \n");
	attach_func("write", (void *)my_write);
}









/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
