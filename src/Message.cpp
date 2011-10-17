/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
#include "Message.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "FaultUnit.h"

extern char *optarg;
extern int optind;

Message::Message()
{
}

Message::~Message()
{
}

int Message::Init(read_done_callback cb)
{
	//create pipe
	int ret = CreatePipe();
	if(ret != 0) {
		return -1;
	}
	//create listening
	pthread_t pid;
	ret = pthread_create(&pid, NULL, cmd_monitor_thread, this);
	if(ret != 0) {
		return -1;
	}

	//set callback
	_read_done_cb = cb;

	//init
	char *cmds = getenv("FAULT_INIT_CMDS");
	if (cmds != NULL) {
		Process(cmds, false);
	}
	return 0;
}

void Message::Destroy()
{
	DeletePipe();
}

int Message::IOCtrl(int io, char *buf, int bufsize)
{
	char * file = NULL;
	int flags;
	if(io & IO_INPUT) {
		file = _in_file_name;
		flags = O_RDONLY;
	} else if (io & IO_OUTPUT) {
		file = _out_file_name;
		flags = O_WRONLY;
	} else {
		return -1;
	}

	int fd = open(file, flags);
	if(fd == -1) {
		perror("open");
		return -1;
	}
	//if ((n = read(fd, buf, COMMAND_MAX_LENGTH)) == 0)
	if(io & IO_READ) {
		int ret = read(fd, buf, bufsize);
		if(ret < 0) {
			perror("read");
			close(fd);
			return -1;
		}
		buf[ret] = '\0';
	} else if(io & IO_WRITE) {
		int ret = write(fd, buf, bufsize);
		if(ret < 0) {
			perror("write");
			close(fd);
			return -1;
		}
	}

	close(fd);
	return 0;
}

void * Message::cmd_monitor_thread(void *ptr)
{ 
	//printf("pid:%u tid:%u\n", getpid(), pthread_self());
	Message *mq = (Message*)ptr;
	char line[COMMAND_MAX_LENGTH];

	int ret;
	while(1)
	{
		memset(line, 0, sizeof(line));
		ret = mq->IOCtrl(IO_INPUT | IO_READ, line, sizeof(line));
		if(ret == 0) {
			mq->Process(line);
		} else {
			fprintf(stderr, "read input error!\n");
		}
	}
}

int Message::CreatePipe()
{
	_pid = getpid();

	char *fifo_prefix = getenv("FIFO_PREFIX");
	if (fifo_prefix != NULL) {
		sprintf(_in_file_name, "%s-%u%s", fifo_prefix, _pid, ".in");
		sprintf(_out_file_name, "%s-%u%s", fifo_prefix, _pid, ".out");
	} else {
		sprintf(_in_file_name, "%s-%u%s", "/tmp/fault", _pid, ".in");
		sprintf(_out_file_name, "%s-%u%s", "/tmp/fault", _pid, ".out");
	}

	if (mkfifo(_in_file_name, 0777) == -1 && errno != EEXIST)
	{
		perror("mkfifo");
		return -1;
	}
	if (mkfifo(_out_file_name, 0777) == -1 && errno != EEXIST)
	{
		perror("mkfifo");
		return -1;
	}
	return 0;
}

void Message::DeletePipe()
{
	//避免子进程退出时把fifo文件删除
	int cur_pid = getpid();
	if(cur_pid == _pid) {
		unlink(_in_file_name);
		unlink(_out_file_name);
	}
}

int Message::Process(char *command, bool bout)
{
	FaultUnit *fu = new FaultUnit();

   	int ret = Parse(command, fu);
	if (ret != 0) {
		return -1;
	}

	char outbuf[256];
	_read_done_cb(*fu, outbuf, sizeof(outbuf));

	if(bout) {
		IOCtrl(IO_OUTPUT | IO_WRITE, outbuf, strlen(outbuf));
	}

	delete fu;

	return 0;
}

int Message::Parse(char *command, FaultUnit *fu) 
{
	if(command == NULL) {
		return -1;
	}
	char *state = NULL;
	char *cmd_s = strtok_r(command, ";", &state);
	while(cmd_s != NULL) {
		char name[256] = {0};
		char enable_s[16] = {0};
		char pro_s[16] = {0};
		char err_s[16] = {0};

		int num = sscanf(cmd_s, "%[^,],%[^,],%[^,],%s", name, enable_s, pro_s, err_s);
		if(num != 4) {
			fprintf(stderr, "sscanf error: %s!\n", cmd_s);
			return -1;
		} else {
			fu->setName(name);
			fu->setEnable(atoi(enable_s));
			fu->setProbability(atoi(pro_s));
			fu->setError(atoi(err_s));
			fu->setParam(NULL);
		}

		cmd_s = strtok_r(NULL, ";", &state);
	}
	return 0;
}

