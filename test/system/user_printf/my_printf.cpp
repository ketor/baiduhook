/***************************************************************************
 * 
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
#include <stdio.h>
#include "bhook.h"

int my_printf(const char *format, ...)
{
	typedef int (*REALFUNC)(const char *format, ...);
	REALFUNC real_printf = (REALFUNC)get_real_func("printf");

	real_printf("this is my printf function!\n");
	return 0;
}

void __attribute__ ((constructor)) my_printf_init(void)
{
	attach_func("printf", (void *)my_printf);
}









/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
