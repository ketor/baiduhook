/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
#include "mylib.h"

#include <stdio.h>
#include <string.h>

namespace MyLib {

int Person::PublicSay()
{
	printf("[Public] Hello, world!\n");
	return 0;
}

int Person::Call()
{
	return PrivateSay();
}

int Person::PrivateSay()
{
	printf("[Private] Hello, world!\n");
	return 0;
}

}











/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
