/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 

#ifndef  __HOOKIMPL_H_
#define  __HOOKIMPL_H_


#include <map>
#include <bfd.h>

#include "BinaryFile.h"

#include "AsmHelper.h"

class HookImpl
{
public:
	struct HookInfo
	{
		char	backup[FAR_JMP_CODE_LEN];
		void	*hook_func;
		void	*trampoline_func;
	};
	struct SysHookInfo
	{
		void	*hook_func;
		void	*old_addr;
	};
	typedef std::map<void *, HookInfo> hook_dict;
	typedef std::map<void *, SysHookInfo> sys_hook_dict;

	HookImpl(bfd *abfd);
	~HookImpl();

	int AttachFuncLib(const BinaryFile::Symbol &symbol, void *hook_func, void **pold_func);
	int DetachFuncLib(const BinaryFile::Symbol &symbol);

	void * GetRealFuncLib(void *src_func);

	int AttachFunc(void *func_plt, void *hook_func);
	int DetachFunc(void *func_plt);
private:
	int SetProt(const void *addr, size_t len, int prot);

	bfd				*_abfd;
	hook_dict		_hooks;
	sys_hook_dict	_sys_hooks;
};











#endif  //__HOOKIMPL_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
