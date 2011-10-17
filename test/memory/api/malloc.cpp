/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bhook.h"

void *my_malloc(size_t size)
{
	typedef void*(*REALMALLOC)(size_t);
	REALMALLOC realmalloc = (REALMALLOC)get_real_func("malloc");
	if(NULL == realmalloc)
	{
		printf("realfun malloc error\n");
		return NULL;
	}
	int err;
	int ret = fault_fail("malloc", &err);
	if(ret == 0) {
		//errno = err;
		return NULL;
	}
	return realmalloc(size);			
}

int main()
{
	attach_func("malloc", (void *)my_malloc);
	fault_enable("malloc", 50, 0);

	int i = 0;
	printf("start! malloc:%p ==========\n", malloc);
	while(1) {
		int size = rand() % 1000;
		void * p = malloc(size);
		if(p) {
			printf("\e[32;1m[%03d][==OK==] malloc(%d)\n\e[m", i, size);
			free(p);
		} else {
			printf("\e[31;1m[%03d][=FAIL=] malloc(%d)\n\e[m", i, size);
		}
		sleep(1);
		i++;
	}
	return 0;
}


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
