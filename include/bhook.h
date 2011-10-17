/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 

#ifndef  __BHOOK_H_
#define  __BHOOK_H_

#define	SO_MAIN(Name)	\
	int __tmp_##Name() { return 0; } \
	static int __init_##Name = __tmp_##Name(); \
	static void __attribute__ ((constructor)) Name##_main(void)

#define	SO_MAIN_START(Name)	\
	if(__init_##Name) { \
		return;	\
	} \
	__init_##Name = 1;


#include <stdio.h>

/**
 * @brief 在hook函数中调用来判断异常是否触发，并返回err和param做进一步使用
 *
 * @param [in/out] name   : const char*
 * @param [in/out] perr   : int*
 * @param [in/out] ppram   : void**
 * @return  int 
 * @retval  0表示异常触发，非0表示异常没有触发 
 * @see 
 * @note 
 * @author xuanbiao
 * @date 2011/07/05 11:21:18
**/
int fault_fail(const char *name, int *perr, void **pparam = NULL);

/**
 * @brief enable异常，并设置触发的概率和传递的参数
 *
 * @param [in/out] name   : const char*
 * @param [in/out] probability   : unsigned int
 * @param [in/out] err   : int
 * @param [in/out] param   : void*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author xuanbiao
 * @date 2011/07/05 11:28:29
**/
int fault_enable(const char *name, unsigned int probability, int err, void *param = NULL);

/**
 * @brief disable异常
 *
 * @param [in/out] name   : const char*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author xuanbiao
 * @date 2011/07/05 11:32:39
**/
int fault_disable(const char *name);

/**
 * @brief 进行系统函数的挂载
 *
 * @param [in/out] func_name   : char*
 * @param [in/out] hook_func   : void*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author xuanbiao
 * @date 2011/07/13 18:50:06
**/
int attach_func(char *func_name, void *hook_func);

/**
 * @brief 进行系统函数的卸载
 *
 * @param [in/out] func_name   : char*
 * @return  int 
 * @retval   
 * @see 
 * @note 
 * @author xuanbiao
 * @date 2011/07/13 18:50:26
**/
int detach_func(char *func_name);

/**
 * @brief 在动态库函数hook中通过函数名获取原真实的函数地址
 *
 * @param [in/out] func_name   : char*
 * @return  void* 
 * @retval  成功返回函数地址，否则返回NULL 
 * @see 
 * @note 
 * @author xuanbiao
 * @date 2011/07/05 11:33:23
**/
void *get_real_func(char *func_name);


/**
 * @brief 进行静态库函数的挂载
 *
 * @param [in/out] func_name   : char*
 * @param [in/out] hook_func   : void*
 * @return  int 
 * @retval  成功返回0，错误返回非0 
 * @see 
 * @note 
 * @author xuanbiao
 * @date 2011/07/05 11:53:17
**/
int attach_func_lib(char *func_name, void *hook_func);

/**
 * @brief 取消静态库函数的挂载
 *
 * @param [in/out] func_name   : char*
 * @return  int 
 * @retval  成功返回0，错误返回非0 
 * @see 
 * @note 
 * @author xuanbiao
 * @date 2011/07/05 12:03:01
**/
int detach_func_lib(char *func_name);

/**
 * @brief 在静态库函数hook中通过函数名获取原真实的函数地址
 *
 * @param [in/out] func_name   : char*
 * @return  void* 
 * @retval  成功返回函数地址，错误返回NULL
 * @see 
 * @note 
 * @author xuanbiao
 * @date 2011/07/05 12:03:44
**/
void *get_real_func_lib(char *func_name);







#endif  //__BHOOK_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
