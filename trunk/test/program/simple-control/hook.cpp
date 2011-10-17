/***************************************************************************
 * 
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "bhook.h"


int B()
{
	printf("hook B start!\n");
	int err;
	int ret = fault_fail("B", &err);
	if(ret == 0) {
		printf("this is hook B\n");
		return err;
	}

	int (* old_B)();
	old_B = (int (*)())get_real_func_lib("B");
	return old_B();
}

void __attribute__ ((constructor)) hook_init(void)
{
	printf("hook_init \n");
	attach_func_lib("B", (void *)B);
}









/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
