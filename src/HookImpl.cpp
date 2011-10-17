/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 

#include "HookImpl.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>	/* for mprotect */

HookImpl::HookImpl(bfd *abfd)
{
	_abfd = abfd;
}

HookImpl::~HookImpl()
{
}


int HookImpl::AttachFuncLib(const BinaryFile::Symbol &symbol, void *hook_func, void **pold_func)
{
	if(symbol.size < FAR_JMP_CODE_LEN) {
		fprintf(stderr, "function code len is too small!\n");
		return -1;
	}
	if(hook_func == NULL) {
		fprintf(stderr, "hook_func is null!\n");
		return -1;
	}
	void *src_func = symbol.addr;
	hook_dict::iterator it = _hooks.find(src_func);
	//如果不存在则先需要创建一个跳板函数，存在了则不需要做更改
	if(it == _hooks.end()) {
		int bufsize = 2 * FAR_JMP_CODE_LEN + MAX_INSTRUCTION_LEN;
		void *buf = malloc(bufsize);
		if(buf == NULL) {
			fprintf(stderr, "malloc trampoline fail!\n");
			return -1;
		}
		//设置内存块可读可写可执行
		int ret = SetProt(buf, bufsize, PROT_READ | PROT_WRITE | PROT_EXEC);
		if (ret < 0) {
			fprintf(stderr, "[line:%d] setprot fail\n", __LINE__);
			return -1;
		}
		int src_dis_len = 0;
		//将目标函数（被hook函数）将被覆盖的指令拷贝到trampoline备份
		int len = AsmHelper::Copy((bfd_byte *)buf, bufsize, _abfd, src_func, FAR_JMP_CODE_LEN, &src_dis_len);
		if (len < FAR_JMP_CODE_LEN) {
			fprintf(stderr, "no enough buffer!len:%d\n", len);
			return -1;
		}
		//在原函数指令后加入跳转指令
		len = AsmHelper::FarJmp((bfd_byte *)buf + len, bufsize - len, (bfd_vma)src_func + src_dis_len);
		if (len < 0) {
			fprintf(stderr, "set far jump fail!len:%d\n", len);
			return -1;
		}
		_hooks[src_func].trampoline_func = buf;

		//设置src_func可读可写
		ret = SetProt(src_func, FAR_JMP_CODE_LEN, PROT_READ | PROT_WRITE | PROT_EXEC);
		if (ret < 0) {
			fprintf(stderr, "[line:%d] setprot fail\n", __LINE__);
			return -1;
		}
		//backup
		memcpy(_hooks[src_func].backup, src_func, FAR_JMP_CODE_LEN);

		//初始为NULL
		_hooks[src_func].hook_func = NULL;
	}

	//更改了hook函数则需要重新跳转
	if(_hooks[src_func].hook_func != hook_func) {
		AsmHelper::FarJmp((bfd_byte *)src_func, FAR_JMP_CODE_LEN, (bfd_vma)hook_func);

		_hooks[src_func].hook_func = hook_func;
	}

	//不为NULL则设置跳板函数，等同于原目标函数
	if (pold_func != NULL) {
		*(bfd_vma *)pold_func = (bfd_vma)_hooks[src_func].trampoline_func;
	}

	return 0;
}

int HookImpl::DetachFuncLib(const BinaryFile::Symbol &symbol)
{
	void *src_func = symbol.addr;
	hook_dict::iterator it = _hooks.find(src_func);
	if(it != _hooks.end()) {
		if(_hooks[src_func].hook_func == NULL) {
			return 0;
		}
		//recovery
		memcpy(src_func, _hooks[src_func].backup, FAR_JMP_CODE_LEN);
		if(_hooks[src_func].trampoline_func != NULL) {
			free(_hooks[src_func].trampoline_func);
			_hooks[src_func].trampoline_func = NULL;
		}
		_hooks[src_func].hook_func = NULL;
		_hooks.erase(it);
	}
	return 0;
}

int HookImpl::SetProt(const void *addr, size_t len, int prot)
{
	unsigned long PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
	bfd_vma page_base = (bfd_vma)(addr) & ~(PAGE_SIZE - 1);

	unsigned long page_num = ((bfd_vma)addr + len - page_base) / PAGE_SIZE + 1;
	unsigned long size = page_num * PAGE_SIZE;
	//设置可读可写可执行，默认不可写
	if(mprotect((void *)page_base, size, prot)) {
		perror("mprotect");
		return -1;
	}
	return 0;
}

void * HookImpl::GetRealFuncLib(void *src_func)
{
	if(src_func == NULL) {
		return NULL;
	}
	hook_dict::iterator it = _hooks.find(src_func);
	if(it != _hooks.end()) {
		return _hooks[src_func].trampoline_func;
	}
	return NULL;
}

int HookImpl::AttachFunc(void *func_plt, void *hook_func)
{
	if(func_plt == NULL) {
		return -1;
	}
	unsigned long * p = (unsigned long *)func_plt;
	sys_hook_dict::iterator it = _sys_hooks.find(func_plt);
	//第一次attach需要备份old_addr
	if(it == _sys_hooks.end()) {
		_sys_hooks[func_plt].old_addr = (void *)*p;
	}
	_sys_hooks[func_plt].hook_func = hook_func;
	*p = (unsigned long)hook_func;
	return 0;
}
int HookImpl::DetachFunc(void *func_plt)
{
	if(func_plt == NULL) {
		return 0;
	}
	unsigned long * p = (unsigned long *)func_plt;
	sys_hook_dict::iterator it = _sys_hooks.find(func_plt);
	if(it != _sys_hooks.end()) {
		*p = (unsigned long)_sys_hooks[func_plt].old_addr;
		_sys_hooks.erase(it);
	}
	return 0;
}



/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
