/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 


#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <fcntl.h>
#include <dlfcn.h>
#include <errno.h>

#include "common.h"
#include "bhook.h"

#ifndef __mode_t_defined
typedef __mode_t mode_t;
# define __mode_t_defined
#endif
#define __mode_t __MODE_T_TYPE
#define __MODE_T_TYPE __U32_TYPE
#define __U32_TYPE unsigned int

extern "C" {
	ssize_t FAULT_(read)(int fd, void *buf, size_t count)
	{
#ifdef DEBUG
		printf("readhook\n");
#endif
		typedef	ssize_t (*REALREAD)(int ,void*,size_t);
		REALREAD realread = (REALREAD)get_real_func("read");
		if(NULL == realread)
		{
			printf("realfun read error");
			return -1;
		}
		int err;
		int ret = fault_fail("read",&err);
		if(ret == 0) {
			errno = err;
			
			return -1;
		}
		return realread(fd,buf,count);
	}
	int FAULT_(open)(const char *pathname, int flags, mode_t mode)
	{
#ifdef DEBUG
		printf("openhook\n");
#endif
		typedef int (*REALOPEN)(const char *, int,mode_t);
		REALOPEN realopen = (REALOPEN)get_real_func("open");
		if(NULL == realopen)
		{
			printf("realfun open error");
			return -1;
		}
		int err;
		int ret = fault_fail("open", &err);
		if(ret == 0) {
			errno = err;
			return -1;
		}
		return realopen(pathname, flags,mode);		
	}
	ssize_t FAULT_(write)(int fd, const void *buf, size_t count)
	{
#ifdef DEBUG
			printf("writehook\n");
#endif
			//printf("[%s] fd:%d, buf:%s", __FUNCTION__, fd, buf);

			typedef ssize_t (*REALWRITE)(int,const void*,size_t);
			REALWRITE realwrite = (REALWRITE)get_real_func("write");
			if(NULL == realwrite)
			{
				printf("realfun write error");
				return -1;
			}
			int err;
			int ret = fault_fail("write", &err);
			if(ret == 0) {
				errno = err;
				return -1;
			}
			return realwrite(fd,buf,count);
	}
	int FAULT_(close)(int fd)
	{
#ifdef DEBUG
			printf("closehook\n");
#endif
			typedef int(*REALCLOSE)(int);
			REALCLOSE realclose = (REALCLOSE)get_real_func("close");
			if(NULL == realclose)
			{
				printf("realfun close error");
				return -1;
			}
			int err;
			int ret = fault_fail("close", &err);
			if(ret == 0) {
				errno = err;
				return -1;
			}
			return realclose(fd);			
	}
}

//void __attribute__ ((constructor)) io_disk_system_init(void)
SO_MAIN(fault_disk_system)
{
	SO_MAIN_START(fault_disk_system);
	//attach
	attach_func("read", (void *)FAULT_(read));
	attach_func("open", (void *)FAULT_(open));
	attach_func("write", (void *)FAULT_(write));
	attach_func("close", (void *)FAULT_(close));
}
