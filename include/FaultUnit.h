/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 

#ifndef  __FAULTUNIT_H_
#define  __FAULTUNIT_H_


#include <string.h>


class FaultUnit
{
public:
    static const int MAX_NAME_LEN = 64;

	FaultUnit();
	FaultUnit(const char *name, int enable, unsigned int probability, int err, void *param);
	~FaultUnit();

	bool Enable() const;

	int Update(const FaultUnit &eu);

	void setName(const char *name)
	{
		strncpy(_name, name, MAX_NAME_LEN);
	}
	const char * getName() const
	{
		return _name;
	}

	void setEnable(int enable)
	{
		_enable = enable;
	}
	int getEnable() const 
	{
		return _enable;
	}

	void setProbability(unsigned int probability)
	{
		_probability = probability;
	}

	unsigned int getProbability() const
	{
		return _probability;
	}

	void setError(int err)
	{
		_err = err;
	}
	int getError() const 
	{
		return _err;
	}

	void setParam(void *param) 
	{
		_param = param;
	}
	void *getParam() const
	{
		return _param;
	}
private:
	char			_name[MAX_NAME_LEN];
	int				_enable;
	unsigned int	_probability;
	int				_err;
	void			*_param;
};











#endif  //__FAULTUNIT_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
