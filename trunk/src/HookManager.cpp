/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
#include "HookManager.h"
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>

HookManager::HookManager()
{
	_init = false;
	_bf = NULL;
	_hook_impl = NULL;
}

HookManager::~HookManager()
{
}

int HookManager::Init()
{
	if(_init) {
		//init again return 0 directly
		return 0;
	}

	char proc_exe[32];
	char filename[MAX_PATH_SIZE];

	pid_t pid = getpid();

	snprintf(proc_exe, sizeof(proc_exe), "/proc/%d/exe", pid);

	memset(filename, 0, sizeof(filename));
	//获取程序路径
	if(readlink(proc_exe, filename, sizeof(filename)) == -1) {
		fprintf(stderr, "readlink fail!\n");
		return -1;
	}

	//init binary file
	_bf = new BinaryFile();
	int ret = _bf->Init(filename);
	if(ret != 0) {
		fprintf(stderr, "binary file init fail!\n");
		return -1;
	}

	//create hook impl
	_hook_impl = new HookImpl(_bf->getBFD());

	//update status
	_init = true;
	return 0;
}

void HookManager::Destroy()
{
	_init = false;

	if(_bf != NULL) {
		_bf->Destroy();
		delete _bf;
	}

	if(_hook_impl != NULL) {
		delete _hook_impl;
	}
}

void * HookManager::getSymbol(char *symbol_name)
{
	if(_init == false) {
		return NULL;
	}
	BinaryFile::Symbol symbol;
	int ret = _bf->getSymbol(symbol_name, symbol);
	if(ret != 0) {
		return NULL;
	}
	return symbol.addr;
}

int HookManager::AttachFuncLib(char *func_name, void *hook_func, void **pold_func)
{
	if(_init == false) {
		return -1;
	}
	BinaryFile::Symbol symbol;
	int ret = _bf->getSymbol(func_name, symbol);
	if(ret != 0) {
		fprintf(stderr, "can't find symbol[%s]\n", func_name);
		return -1;
	}
	return _hook_impl->AttachFuncLib(symbol, hook_func, pold_func);
}

int HookManager::DetachFuncLib(char *func_name)
{
	if(_init == false) {
		return -1;
	}
	BinaryFile::Symbol symbol;
	int ret = _bf->getSymbol(func_name, symbol);
	if(ret != 0) {
		fprintf(stderr, "can't find symbol[%s]\n", func_name);
		return -1;
	}
	return _hook_impl->DetachFuncLib(symbol);
}

void * HookManager::GetRealFuncLib(char *func_name)
{
	BinaryFile::Symbol symbol;
	int ret = _bf->getSymbol(func_name, symbol);
	if(ret != 0) {
		fprintf(stderr, "can't find symbol[%s]\n", func_name);
		return NULL;
	}
	return _hook_impl->GetRealFuncLib(symbol.addr);
}

int HookManager::AttachFunc(char *func_name, void *hook_func)
{
	BinaryFile::RelSym relsym;
	int ret = _bf->getRelocs(func_name, relsym);
	if(ret != 0) {
		return -1;
	}
	return _hook_impl->AttachFunc(relsym.pgot, hook_func);
}

int HookManager::DetachFunc(char *func_name)
{
	BinaryFile::RelSym relsym;
	int ret = _bf->getRelocs(func_name, relsym);
	if(ret != 0) {
		return -1;
	}
	return _hook_impl->DetachFunc(relsym.pgot);
}
void *HookManager::GetRealFunc(char *func_name)
{
	BinaryFile::RelSym relsym;
	int ret = _bf->getRelocs(func_name, relsym);
	if(ret != 0) {
		return NULL;
	}

	/*
	static void * libc = dlopen("libc.so.6", RTLD_NOW);
	if (libc == NULL) {
		fprintf(stderr, "Error loading libc: %s\n", dlerror());
		return NULL;
	}
	void *real_func = (void *)dlsym(libc, func_name);
	*/
	void *real_func = (void *)dlsym(RTLD_NEXT, relsym.realname.c_str());
	if(real_func == NULL)
	{
		fprintf(stderr,"real func not found: %s\n", func_name);
		return NULL;
	}
	return real_func;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
