/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 

#include <gtest/gtest.h>

#include "mylib.h"

#include "bhook.h"

int my_func(char *str)
{
	int err;
	int ret = fault_fail("func", &err);
	if(ret == 0) {
		printf("\e[33;1m[=HOOK=] this is hook func\n\e[m");
		return err;
	}

	typedef int (*REAL_FUNC)(char *);
	REAL_FUNC old_func = (REAL_FUNC)get_real_func_lib("func");
	return old_func(str);
}

TEST(MyLib, case1) 
{
	//enable
	fault_enable("func", 100, -1);

	int ret = func("cmd1");
	ASSERT_EQ(1, ret);
}

TEST(MyLib, case2) 
{
	//disable
	fault_disable("func");

	int ret = func("cmd2");
	ASSERT_EQ(2, ret);
}

int  main(int  argc,  char  **argv) {
	testing::InitGoogleTest(&argc, argv);

	testing::GTEST_FLAG(color) = "yes";
	//attach func
	attach_func_lib("func", (void *)my_func);

	return RUN_ALL_TESTS();
}











/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
