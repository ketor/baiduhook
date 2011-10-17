/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "mylib.h"

#include "bhook.h"

extern "C" {
	
void *my_malloc(ssize_t size) 
{
	printf("this is my malloc!\n");
	int ret = fault_fail("malloc", NULL);
	if(ret == 0) {
		return NULL;
	}
	typedef void*(*REALMALLOC)(size_t);
	REALMALLOC real_malloc = (REALMALLOC)get_real_func("malloc");
	return real_malloc(size);
}

}
int main()
{
	//attach
	printf("------------ [1] attach malloc, but not enable(default)\n");
	attach_func("malloc", (void *)my_malloc);
	test_malloc();

	printf("------------ [2] enable malloc fault, 100%% probability\n");
	fault_enable("malloc", 100, -1);
	test_malloc();

	//detach
	printf("------------ [3] detach malloc\n");
	detach_func("malloc");
	test_malloc();

	//attach again
	printf("------------ [4] attach malloc again, maintain the last state\n");
	attach_func("malloc", (void *)my_malloc);
	test_malloc();

	printf("------------ [5] disable malloc fault\n");
	fault_disable("malloc");
	test_malloc();
	return 0;
}
















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
