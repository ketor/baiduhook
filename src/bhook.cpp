/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 

#include "bhook.h"

#include "Core.h"
#include "FaultUnit.h"

//******** internal func **********/
inline int fault_ctrl(const char *name, int enable, unsigned int probability, int err, void *param)
{
    FaultUnit fu(name, enable, probability, err, param);

	int ret = Core::getInstance()->setFaultUnit(fu);
	return ret;
}
//***********************************


int fault_fail(const char *name, int *perr, void **pparam)
{
	/*
	if(!Core::inited) {
		return -1;
	}
	*/
    const FaultUnit *fu = NULL;
    fu = Core::getInstance()->getFaultUnit(name);
	if(NULL != fu)	{
		if(fu->Enable()) {
			if(perr != NULL) {
				*perr = fu->getError();
			}
			if(pparam != NULL) {
				*pparam = fu->getParam();
			}
			return 0;
		}
	}
	return -1;
}

int fault_enable(const char *name, unsigned int probability, int err, void *param)
{
	return fault_ctrl(name, 1, probability, err, param);
}

int fault_disable(const char *name)
{
	return fault_ctrl(name, 0, 0, 0, NULL);
}

int attach_func(char *func_name, void *hook_func)
{
	return Core::getInstance()->AttachFunc(func_name, hook_func);
}
int detach_func(char *func_name)
{
	return Core::getInstance()->DetachFunc(func_name);
}

void *get_real_func(char *func_name)
{
	return Core::getInstance()->GetRealFunc(func_name);
}

int attach_func_lib(char *func_name, void *hook_func)
{
	return Core::getInstance()->AttachFuncLib(func_name, hook_func, NULL);
}
int detach_func_lib(char *func_name)
{
	return Core::getInstance()->DetachFuncLib(func_name);
}

void *get_real_func_lib(char *func_name)
{
	return Core::getInstance()->GetRealFuncLib(func_name);
}







/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
