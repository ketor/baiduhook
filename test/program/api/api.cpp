/***************************************************************************
 * 
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "bhook.h"


struct Info
{
	int start;
	int end;
	int ret_val;
};

size_t new_fread(void * ptr,size_t size,size_t nmemb,FILE * stream)
{
	//printf("********* hook fread start *********\n");
	int err;
	int ret = fault_fail("fread", &err);
	if(ret == 0) {
		errno = err;
		stream->_flags |=_IO_ERR_SEEN;
		return 0;
	}
	typedef	size_t (*REAL_FUNC)(const void*,size_t,size_t,FILE*);
	REAL_FUNC old_func = (REAL_FUNC)get_real_func("fread");

	return old_func(ptr, size, nmemb, stream);
}

int new_func(int num)
{
	int err;
	Info *param;
	int ret = fault_fail("func", &err, (void **)&param);
	if(ret == 0) {
		if (param == NULL) {
			printf("\e[33;1m[%03d][=HOOK=] this is hooked by command line or API\n\e[m", num);
			return err;
		} else if (num >= param->start && num < param->end) {
			printf("\e[33;1m[%03d][=HOOK=] this is hooked by API\n\e[m", num);
			//printf("num: %d, start:%d, end:%d\n", num, param->start, param->end);
			return param->ret_val;
		}
	}

	typedef int (*REAL_FUNC)(int);
	REAL_FUNC old_func = (REAL_FUNC)get_real_func_lib("func");
	return old_func(num);
}

int func(int num)
{
	//printf("this is func\n");

	char *filename = "data.txt";

	char buf[1024];

	FILE *fp = fopen(filename, "r");

	if(fp == NULL) {
		printf("[%d] open %s fail\n", num, filename);
		return -1;
	}
	//fseek(fp, 0, SEEK_SET);
	int ret = fread(buf, 1, sizeof(buf), fp);
	if(ret == 0) {
		printf("\e[31;1m[%03d][=FAIL=] read fail: %s\e[m\n", num, strerror(errno));
	} else {
		buf[ret] = 0;
		printf("\e[32;1m[%03d][==OK==] buf: %s\e[m", num, buf);
	}
	fclose(fp);

	return 0;
}
int main()
{
	attach_func("fread", (void *)new_fread);
	attach_func_lib("func", (void *)new_func);

	Info param;
	param.start = 10;
	param.end = 20;
	param.ret_val = -3;
	//传递自定义结构体
	fault_enable("func", 50, 0, &param);

	int i = 0;
	while(1) {
		func(i++);
		sleep(1);
	}
	return 0;
}

















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
