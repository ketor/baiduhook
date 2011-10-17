/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 

#include "Core.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>

//int Core::inited = 0;

Core * Core::_instance = NULL;
pthread_mutex_t Core::_inst_lock = PTHREAD_MUTEX_INITIALIZER;

Core::Core()
{
}

int Core::Init()
{
	//init hook_manager
    _hook_manager = new HookManager();
    int ret = _hook_manager->Init();
    if(ret != 0) {
        return -1;
    }

    //init mq
	_mq = new Message();
    ret = _mq->Init(ReadDoneCallBack);
    if(ret != 0) {
        return -1;
    }
    return 0;
}

void Core::Release()
{
	if(_hook_manager) {
		_hook_manager->Destroy();
	}
	if(_mq) {
		_mq->Destroy();
	}
}

int Core::setFaultUnit(const FaultUnit &fu)
{
	string key = fu.getName();
	if(key == "") {
		return 0;
	}
    hook_map_t::iterator it = _hook_vecs.find(key);
    if(it != _hook_vecs.end()) {
        _hook_vecs[key].Update(fu);
	} else {
		_hook_vecs[key] = fu;
	}
    return 0;
}

const FaultUnit * Core::getFaultUnit(string key)
{
    hook_map_t::iterator it = _hook_vecs.find(key);
    if(it != _hook_vecs.end()) {
        return &_hook_vecs[key];
    }
    return NULL;
}

Core * Core::getInstance()
{
    if(_instance == NULL) {
		pthread_mutex_lock(&_inst_lock);
		if(_instance == NULL) {
			_instance = new Core();
			_instance->Init();
			//inited = 1;
		}
		pthread_mutex_unlock(&_inst_lock);
    }
    return _instance;
}

int Core::ReadDoneCallBack(const FaultUnit &fu, char *outbuf, int bufsize)
{
    int ret = Core::getInstance()->setFaultUnit(fu);

	snprintf(outbuf, bufsize, "%d", ret);

    return 0;
}


void __attribute__ ((constructor)) main_init()
{
}

void __attribute__ ((destructor)) main_fini()
{
	Core::getInstance()->Release();
}
