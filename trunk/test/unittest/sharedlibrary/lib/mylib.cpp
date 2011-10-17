/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 

#include "mylib.h"

#include <stdio.h>
#include <string.h>

int func(char *str)
{
	printf("[%s][%d] str:%s\n", __FILE__, __LINE__, str);
	if(strcmp(str, "cmd1") == 0) {
		return 1;
	} else if(strcmp(str, "cmd2") == 0) {
		return 2;
	} else if(strcmp(str, "cmd3") == 0) {
		return 3;
	}
	return 0;
}













/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
