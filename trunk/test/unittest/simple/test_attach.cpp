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
	return -1;
}

TEST(MyLib, case1) 
{
	//attach func
	attach_func_lib("func", (void *)my_func);

	int ret = func("cmd1");
	EXPECT_EQ(1, ret);

	detach_func_lib("func");
}

TEST(MyLib, case2) 
{
	int ret = func("cmd2");
	ASSERT_EQ(2, ret);
}

int  main(int  argc,  char  **argv) {
	testing::InitGoogleTest(&argc, argv);

	testing::GTEST_FLAG(color) = "yes";

	return RUN_ALL_TESTS();
}











/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
