/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 

#ifndef  __HOOKMANAGER_H_
#define  __HOOKMANAGER_H_


#include "BinaryFile.h"
#include "HookImpl.h"

class HookManager
{
public:
	static const int MAX_PATH_SIZE = 512;

	HookManager();
	~HookManager();

	int Init();

	void Destroy();

	void * getSymbol(char *symbol_name);

	int AttachFuncLib(char *func_name, void *hook_func, void **pold_func);
	int DetachFuncLib(char *func_name);
	void *GetRealFuncLib(char *func_name);

	int AttachFunc(char *func_name, void *hook_func);
	int DetachFunc(char *func_name);
	void *GetRealFunc(char *func_name);
private:
	bool		_init;

	BinaryFile	*_bf;
	HookImpl	*_hook_impl;
};












#endif  //__HOOKMANAGER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
