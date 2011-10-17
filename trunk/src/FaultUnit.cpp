/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
#include "FaultUnit.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FaultUnit::FaultUnit()
{
	memset(_name, 0, MAX_NAME_LEN);
	_enable = 0;
	_probability = 0;
	_err = 0;
	_param = NULL;
}
FaultUnit::FaultUnit(
	const char *name, 
	int enable, 
	unsigned int probability, 
	int err, 
	void *param)
{
	strncpy(_name, name, MAX_NAME_LEN);
	_enable = enable;
	_probability = probability;
	_err = err;
	_param = param;
}

FaultUnit::~FaultUnit()
{
}


bool FaultUnit::Enable() const
{
	if(_enable) {
		srand(time(NULL) * (long)this);
		unsigned int r = rand() % 100;
		if(r < _probability) {
			return true;
		}
	}
	return false;
}

int FaultUnit::Update(const FaultUnit &fu)
{
	_enable = fu.getEnable();
	_probability = fu.getProbability();
	_err = fu.getError();
	_param = fu.getParam();
	return 0;
}













/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
