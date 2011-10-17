/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/


 
 
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include <malloc.h>

#include"common.h"
#include"bhook.h"


#if 0
/* Original glibc's hooks, saved in our init function */
static void *(*old_malloc_hook)(size_t, const void *);

/* Our own hooks, that will replace glibc's */
static void *fault_malloc_hook(size_t size, const void *caller);

static void *fault_malloc_hook(size_t size, const void *caller)
{
    void *r;

    /* See __malloc_hook(3) for details */
    __malloc_hook = old_malloc_hook;

	int err;
	int ret = fault_fail("malloc", &err);
    if (ret == 0) {
        goto exit;
    }
	
	printf("mallo start! old_malloc_hook:%p\n", old_malloc_hook);
    r = malloc(size);
	printf("mallo end!\n");

exit:
	printf("malloc exit!\n");
    old_malloc_hook = __malloc_hook;
    __malloc_hook = fault_malloc_hook;

    return r;
}

void __attribute__ ((constructor)) main_init(void)
{
	old_malloc_hook = __malloc_hook;
	__malloc_hook = fault_malloc_hook;
}
#endif
extern "C"{
	void *FAULT_(malloc)(size_t size)
	{
#ifdef DEBUG
			printf("mallochook\n");
#endif
			typedef void*(*REALMALLOC)(size_t);
			REALMALLOC realmalloc = (REALMALLOC)get_real_func("malloc");
			if(NULL == realmalloc)
			{
				printf("realfun malloc error\n");
				return NULL;
			}
			int err;
			int ret = fault_fail("malloc", &err);
			if(ret == 0) {
				//errno = err;
				return NULL;
			}
			return realmalloc(size);			
	}
	void *FAULT_(realloc)(void *ptr,size_t size)
	{
#ifdef DEBUG
			printf("reallochook\n");
#endif
			typedef void*(*REALREALLOC)(void *,size_t);
			REALREALLOC realrealloc = (REALREALLOC)get_real_func("realloc");
			if(NULL == realrealloc)
			{
				printf("realfun realloc error\n");
				return NULL;
			}
			int err;
			int ret = fault_fail("realloc", &err);
			if(ret == 0) {
				//errno = err;
				return NULL;
			}
			return realrealloc(ptr,size);			
	}
	void *FAULT_(calloc)(size_t nmemb,size_t size)
	{
#ifdef DEBUG
			printf("callochook\n");
#endif
			typedef void*(*REALREALLOC)(size_t,size_t);
			REALREALLOC realcalloc = (REALREALLOC)get_real_func("calloc");
			if(NULL == realcalloc)
			{
				printf("realfun calloc error");
				return NULL;
			}
			int err;
			int ret = fault_fail("calloc", &err);
			if(ret == 0) {
				//errno = err;
				return NULL;
			}
			return realcalloc(nmemb,size);			
	}
	void FAULT_(free)(void *ptr)
	{
#ifdef DEBUG
			printf("freehook\n");
#endif
			typedef void(*REALFREE)(void*);
			REALFREE realfree = (REALFREE)get_real_func("free");
			if(NULL == realfree)
			{
				printf("realfun free error");
				return;
			}
			int err;
			int ret = fault_fail("free", &err);
			if(ret == 0) {
				//errno = err;
				return;
			}
			realfree(ptr);		
			return;	
	}
	void *FAULT_(memset)(void *s,int c ,size_t n)
	{
#ifdef DEBUG
			printf("memsethook\n");
#endif
			typedef void*(*REALMEMSET)(void*,int,size_t);
			REALMEMSET realmemset = (REALMEMSET)get_real_func("memset");
			if(NULL == realmemset)
			{
				printf("realfun memset error");
				return NULL;
			}
			int err;
			int ret = fault_fail("memset", &err);
			if(ret == 0) {
				//errno = err;
				return NULL;
			}
			return realmemset(s,c,n);			
	}	
	void *FAULT_(memcpy)(void *dest, const void *src, size_t n)
	{
#ifdef DEBUG
			printf("memcpyhook\n");
#endif
			typedef void*(*REALMEMCPY)(void*,const void*,size_t);
			REALMEMCPY realmemcpy = (REALMEMCPY)get_real_func("memcpy");
			if(NULL == realmemcpy)
			{
				printf("realfun memcpy error");
				return NULL;
			}
			int err;
			int ret = fault_fail("memcpy", &err);
			if(ret == 0) {
				//errno = err;
				return NULL;
			}
			return realmemcpy(dest,src,n);	
	}
	void *FAULT_(memmove)(void *dest, const void *src, size_t n)
	{
#ifdef DEBUG
			printf("memmovehook\n");
#endif
			typedef void*(*REALMEMMOVE)(void*,const void*,size_t);
			REALMEMMOVE realmemmove = (REALMEMMOVE)get_real_func("memmove");
			if(NULL == realmemmove)
			{
				printf("realfun memmove error");
				return NULL;
			}
			int err;
			int ret = fault_fail("memmove", &err);
			if(ret == 0) {
				//errno = err;
				return NULL;
			}
			return realmemmove(dest,src,n);	
	}
	int FAULT_(memcmp)(const void *s1, const void *s2, size_t n)
	{
#ifdef DEBUG
			printf("memcmphook\n");
#endif
			typedef int(*REALMEMCMP)(const void*,const void*,size_t);
			REALMEMCMP realmemcmp = (REALMEMCMP)get_real_func("memcmp");
			if(NULL == realmemcmp)
			{
				printf("realfun memcmp error");
				return NULL;
			}
			int err;
			int ret = fault_fail("memcmp", &err);
			if(ret == 0) {
				//errno = err;
				return NULL;
			}
			return realmemcmp(s1,s2,n);	
	}	
	void *FAULT_(memchr)(const void *s, int c, size_t n)
	{
#ifdef DEBUG
			printf("memchrhook\n");
#endif
			typedef void*(*REALMEMCHR)(const void*,int,size_t);
			REALMEMCHR realmemchr = (REALMEMCHR)get_real_func("memchr");
			if(NULL == realmemchr)
			{
				printf("realfun memchr error");
				return NULL;
			}
			int err;
			int ret = fault_fail("memchr", &err);
			if(ret == 0) {
				//errno = err;
				return NULL;
			}
			return realmemchr(s,c,n);	
	}	
	void *FAULT_(memrchr)(const void *s, int c, size_t n)
	{
#ifdef DEBUG
			printf("memrchrhook\n");
#endif
			typedef void*(*REALMEMRCHR)(const void*,int,size_t);
			REALMEMRCHR realmemrchr = (REALMEMRCHR)get_real_func("memrchr");
			if(NULL == realmemrchr)
			{
				printf("realfun memrchr error");
				return NULL;
			}
			int err;
			int ret = fault_fail("memrchr", &err);
			if(ret == 0) {
				//errno = err;
				return NULL;
			}
			return realmemrchr(s,c,n);	
	}
	void *FAULT_(memccpy)(void *dest, const void *src, int c, size_t n)
	{
#ifdef DEBUG
			printf("memccpyhook\n");
#endif
			typedef void*(*REALMEMCCPY)(void*,const void*,int,size_t);
			REALMEMCCPY realmemccpy = (REALMEMCCPY)get_real_func("memccpy");
			if(NULL == realmemccpy)
			{
				printf("realfun memccpy error");
				return NULL;
			}
			int err;
			int ret = fault_fail("memccpy", &err);
			if(ret == 0) {
				//errno = err;
				return NULL;
			}
			return realmemccpy(dest,src,c,n);	
	}
	void *FAULT_(mempcpy)(void *dest, const void *src, size_t n)
	{
#ifdef DEBUG
			printf("mempcpyhook\n");
#endif
			typedef void*(*REALMEMPCPY)(void*,const void*,size_t);
			REALMEMPCPY realmempcpy = (REALMEMPCPY)get_real_func("mempcpy");
			if(NULL == realmempcpy)
			{
				printf("realfun mempcpy error");
				return NULL;
			}
			int err;
			int ret = fault_fail("mempcpy", &err);
			if(ret == 0) {
				//errno = err;
				return NULL;
			}
			return realmempcpy(dest,src,n);
	}
}



//void __attribute__ ((constructor)) memory_init(void)
SO_MAIN(fault_memory)
{
	SO_MAIN_START(fault_memory);
	//attach
	attach_func("malloc", (void *)FAULT_(malloc));
	attach_func("realloc", (void *)FAULT_(realloc));
	attach_func("calloc", (void *)FAULT_(calloc));
	attach_func("free", (void *)FAULT_(free));
	attach_func("memset", (void *)FAULT_(memset));
	attach_func("memcpy", (void *)FAULT_(memcpy));
	attach_func("memmove", (void *)FAULT_(memmove));
	attach_func("memcmp", (void *)FAULT_(memcmp));
	attach_func("memchr", (void *)FAULT_(memchr));
	attach_func("memrchr", (void *)FAULT_(memrchr));
	attach_func("memccpy", (void *)FAULT_(memccpy));
	attach_func("mempcpy", (void *)FAULT_(mempcpy));
} 

/*
void __attribute__ ((destructor)) memory_fini(void) 
{
	//detach
}
*/














/* vim: set ts=4 sw=4 sts=4 tw=100 noet: */
