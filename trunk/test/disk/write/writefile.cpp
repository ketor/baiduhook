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

	char *buf = "hello, world!";
	int i=0;
	while(1) {
		//printf("line:%d\n", __LINE__);
		int fd = open(filename, O_CREAT | O_WRONLY, 0664);

		if(fd == -1) {
			printf("\e[31;1m[%03d][=FAIL=] open %s fail: %s\e[m\n", i, filename, strerror(errno));
			goto next;
		}
		int ret = write(fd, buf, strlen(buf));
		if(ret == -1) {
			printf("\e[31;1m[%03d][=FAIL=] write fail: %s\e[m\n", i, strerror(errno));
		} else {
			printf("\e[32;1m[%03d][==OK==] buf: %s\n\e[m", i, buf);
		}
		close(fd);
		fflush(stdout);
next:
		sleep(1);
		i++;
	}

	return 0;
}

















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
