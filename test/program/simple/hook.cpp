/***************************************************************************
 * 
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/


#include <stdio.h>
#include "bhook.h"


int my_B()
{
	printf("this is my hook B function!\n");
	return -1;
}

void __attribute__ ((constructor)) hook_init(void)
{
	//printf("hook_init \n");
	attach_func_lib("B", (void *)my_B);
}








/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
