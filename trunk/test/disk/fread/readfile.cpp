/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 

#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main()
{
	char *filename = "data.txt";

	char buf[1024];
	for(int i=0;;i++) {
		FILE *fp = fopen(filename, "r");

		if(fp == NULL) {
			printf("[%d] open %s fail\n", i, filename);
			sleep(1);
			continue;
		}
		//fseek(fp, 0, SEEK_SET);
		int ret = fread(buf, 1, sizeof(buf), fp);
		if(ret == 0) {
			printf("\e[31;1m[%03d][=FAIL=] read fail: %s\e[m\n", i, strerror(errno));
		} else {
			buf[ret] = 0;
			printf("\e[32;1m[%03d][==OK==] buf: %s\e[m", i, buf);
		}
		fclose(fp);

		sleep(1);
	}

	return 0;
}

















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
