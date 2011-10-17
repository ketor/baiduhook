/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 

#include "mylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_malloc()
{
	//printf("malloc:%p\n", malloc);
	void *p = malloc(256);
	if(p) {
		printf("\e[32;1m[==OK==] malloc sucess!\n\e[m");
	} else {
		printf("\e[31;1m[=FAIL=] malloc fail!\n\e[m");
	}
}





/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
