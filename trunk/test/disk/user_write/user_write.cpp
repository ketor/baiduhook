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

int main(int argc, char *argv[])
{

	if(argc != 2) {
		fprintf(stderr, "usage: ./user_write filename\n");
		return -1;
	}
	char *filename = argv[1];

	char *buf = "hello, world!";
	int fd = open(filename, O_CREAT | O_WRONLY, 0664);

	if(fd == -1) {
		printf("\e[31;1m[=FAIL=] open %s fail: %s\e[m\n", filename, strerror(errno));
		return -1;
	}

	int ret = write(fd, buf, strlen(buf));
	if(ret == -1) {
		printf("\e[31;1m[=FAIL=] write fail: %s\e[m\n", strerror(errno));
	} else {
		printf("\e[32;1m[==OK==] write ok: %s\n\e[m", buf);
	}

	close(fd);

	return 0;
}

















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
