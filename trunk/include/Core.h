/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
#ifndef __CORE_H__
#define __CORE_H__

#include <map>
#include <string>

#include "FaultUnit.h"
#include "Message.h"
#include "HookManager.h"


using std::map;
using std::string;


class Core
{
public:
	typedef map<string, FaultUnit>  hook_map_t;

    static Core * getInstance();

	void Release();

    int  setFaultUnit(const FaultUnit &eu);
    
    const FaultUnit *getFaultUnit(string key);

	int AttachFuncLib(char *func_name, void *hook_func, void **pold_func) {
		return _hook_manager->AttachFuncLib(func_name, hook_func, pold_func);
	}
	int DetachFuncLib(char *func_name) {
		return _hook_manager->DetachFuncLib(func_name);
	}
	void * GetRealFuncLib(char *func_name) {
		return _hook_manager->GetRealFuncLib(func_name);
	}

	int AttachFunc(char *func_name, void *hook_func) {
		return _hook_manager->AttachFunc(func_name, hook_func);
	}
	int DetachFunc(char *func_name) {
		return _hook_manager->DetachFunc(func_name);
	}
	void * GetRealFunc(char *func_name) {
		return _hook_manager->GetRealFunc(func_name);
	}

public:
	//static int inited;

private:
    Core();
	int Init();
	static int ReadDoneCallBack(const FaultUnit &eu, char *outbuf, int bufsize);

	static Core		*_instance;
	static pthread_mutex_t _inst_lock;
	
    hook_map_t		_hook_vecs;    
    Message			*_mq;
	HookManager		*_hook_manager;
};



#endif
