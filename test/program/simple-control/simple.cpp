/***************************************************************************
 * 
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 

#include <stdio.h>


void A()
{
	printf("this is A\n");
}
int B()
{
	printf("this is B\n");
	return 0;
}
void C()
{
	printf("this is C\n");
}
void D()
{
	printf("this is D\n");
}
int main()
{
	A();

	int ret = B();
	if(ret == 0) {
		C();
	} else {
		D();
	}

	return 0;
}

















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
