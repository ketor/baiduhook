/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 

#include <gtest/gtest.h>

#include "mylib.h"

#include "bhook.h"

//隐含类指针参数需要在hook函数中显示定义
int MyPublicSay(MyLib::Person *person)
{
	int err;
	int ret = fault_fail("MyLib::Person::PublicSay", &err);
	if(ret == 0) {
		printf("\e[33;1m[=HOOK=] this is no PublicSay!\n\e[m");
		return err;
	}

	typedef int (*REAL_FUNC)(MyLib::Person *);
	REAL_FUNC old_func = (REAL_FUNC)get_real_func_lib("MyLib::Person::PublicSay");
	return old_func(person);
}

int MyPrivateSay(MyLib::Person *person)
{
	int err;
	int ret = fault_fail("MyLib::Person::PrivateSay", &err);
	if(ret == 0) {
		printf("\e[33;1m[=HOOK=] this is no PrivateSay!\n\e[m");
		return err;
	}

	typedef int (*REAL_FUNC)(MyLib::Person *);
	REAL_FUNC old_func = (REAL_FUNC)get_real_func_lib("MyLib::Person::PrivateSay");
	return old_func(person);
}

TEST(Person, PublicSay) 
{
	//enable
	fault_enable("MyLib::Person::PublicSay", 100, -1);

	MyLib::Person * person = new MyLib::Person();

	int ret = person->PublicSay();

	EXPECT_EQ(0, ret);

	delete person;
}

TEST(Person, PrivateSay) 
{
	//enable
	fault_enable("MyLib::Person::PrivateSay", 100, -2);

	MyLib::Person * person = new MyLib::Person();

	int ret = person->Call();

	EXPECT_EQ(0, ret);

	delete person;
}

int  main(int  argc,  char  **argv) {
	testing::InitGoogleTest(&argc, argv);

	testing::GTEST_FLAG(color) = "yes";
	//attach func
	attach_func_lib("MyLib::Person::PublicSay", (void *)MyPublicSay);

	attach_func_lib("MyLib::Person::PrivateSay", (void *)MyPrivateSay);

	return RUN_ALL_TESTS();
}











/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
