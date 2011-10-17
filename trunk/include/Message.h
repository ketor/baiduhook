/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
#ifndef  __MESSAGE_H_
#define  __MESSAGE_H_

#include "FaultUnit.h"

class Message
{
public:
	static const int COMMAND_MAX_LENGTH = 1024;
	static const int COMMAND_MAX_COUNT = 16;
	static const int FILE_NAME_MAX_SIZE = 64;

	enum {
		IO_UNKNOW = 0x0,

		IO_INPUT = 0x1,
		IO_OUTPUT = 0x2,

		IO_READ = 0x10,
		IO_WRITE = 0x20,
	};
	typedef int (*read_done_callback)(const FaultUnit &eu, char *outbuf, int bufsize);

	Message();
	~Message();

	int Init(read_done_callback cb);
	void Destroy();

	int IOCtrl(int io, char *buf, int bufsize);
private:
	int CreatePipe();
	void DeletePipe();

	int Process(char *command, bool bout = true);
	int Parse(char *command, FaultUnit *eu);

	static void * cmd_monitor_thread(void *ptr);

	char _in_file_name[FILE_NAME_MAX_SIZE];
	char _out_file_name[FILE_NAME_MAX_SIZE];

	read_done_callback  _read_done_cb;

	int	_pid;
};



#endif  //__MESSAGE_H_
