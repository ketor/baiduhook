/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 


#ifndef  __MYLIB_H_
#define  __MYLIB_H_


namespace MyLib {


class Person
{
public:
	Person() {}
	~Person() {}

	int PublicSay();

	int Call();
private:
	int PrivateSay();
};



}







#endif  //__MYLIB_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
