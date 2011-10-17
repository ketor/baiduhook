/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 



#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <fcntl.h>
#include <dlfcn.h>
#include <errno.h>

#include"common.h"
#include"bhook.h"

#define COMMERRNO 10
//Ïê¼øerrno.hÓëerrno-base.h
//size_t commlasterrno[COMMERRNO]={0,1,2,3,4,5,6,7,8,9};
extern "C" {     
	FILE * FAULT_(fopen)(const char * path,const char * mode)
	{
#ifdef DEBUG
		printf("fopenhook\n");
#endif
		typedef FILE*(*REALFOPEN)(const char *path,const char *mode);
		REALFOPEN realfopen = (REALFOPEN)get_real_func("fopen");
		if(NULL == realfopen)
		{
			printf("realfun fopen error");
			return NULL;
		}
		int err;
		int ret = fault_fail("fopen", &err);
		if(ret == 0) {
			errno = err;
			return NULL;
		}
		return realfopen(path, mode);
	}

	size_t FAULT_(fwrite)(const void * ptr,size_t size,size_t nmemb,FILE * stream)
	{
#ifdef DEBUG
		printf("fwritehook\n");
#endif
		typedef	size_t (*REALFWRITE)(const void*,size_t,size_t,FILE*);
		REALFWRITE realfwrite = (REALFWRITE)get_real_func("fwrite");
		if(NULL == realfwrite)
		{
			printf("realfun fwrite error");
			return 0;
		}
		int err;
		int ret = fault_fail("fwrite", &err);
		if(ret == 0) {
			errno = err;
			stream->_flags |=_IO_ERR_SEEN;
			return 0;
		}
		return realfwrite(ptr,size,nmemb,stream);

	}
	size_t FAULT_(fread)(void * ptr,size_t size,size_t nmemb,FILE * stream)
	{
#ifdef DEBUG
		printf("freadhook\n");
#endif
		typedef	size_t (*REALFREAD)(const void*,size_t,size_t,FILE*);
		REALFREAD realfread = (REALFREAD)get_real_func("fread");
		if(NULL == realfread)
		{
			printf("realfun fread error");
			return 0;
		}
		int err;
		int ret = fault_fail("fread", &err);
		if(ret == 0) {
			errno = err;
			stream->_flags |=_IO_ERR_SEEN;
			return 0;
		}
		return realfread(ptr,size,nmemb,stream);
	}

	int FAULT_(fclose)(FILE *stream)
	{
#ifdef DEBUG
		printf("fclosehook\n");
#endif
		typedef	int (*REALFCLOSE)(FILE *);
		REALFCLOSE realfclose = (REALFCLOSE)get_real_func("fclose");
		if(NULL == realfclose)
		{
			printf("realfun error");
			return 0;
		}
		int err;
		int ret = fault_fail("fclose", &err);
		if(ret == 0) {
			errno = err;
			stream->_flags |=_IO_ERR_SEEN;
			return 0;
		}
		return realfclose(stream);
	}
	int FAULT_(fseek)(FILE *stream, long offset, int whence)
	{
#ifdef DEBUG
		printf("fseekhook\n");
#endif
		typedef	int (*REALFSEEK)(FILE *,long,int);
		REALFSEEK realfseek = (REALFSEEK)get_real_func("fseek");
		if(NULL == realfseek)
		{
			printf("realfun fseek error");
			return -1;
		}
		int err;
		int ret = fault_fail("fseek",&err);
		if(ret == 0) {
			errno = err;
			stream->_flags |=_IO_ERR_SEEN;
			return -1;
		}
		return realfseek(stream,offset,whence);
	}	
	long FAULT_(ftell)(FILE *stream)
	{
#ifdef DEBUG
		printf("ftellhook\n");
#endif
		typedef	long (*REALFTELL)(FILE *);
		REALFTELL realftell = (REALFTELL)get_real_func("ftell");
		if(NULL == realftell)
		{
			printf("realfun ftell error");
			return -1;
		}
		int err;
		int ret = fault_fail("ftell", &err);
		if(ret == 0) {
			errno = err;
			stream->_flags |=_IO_ERR_SEEN;
			return -1;
		}
		return realftell(stream);
	}	
	char *FAULT_(fgets)(char *s, int size, FILE *stream)
	{
#ifdef DEBUG
		printf("fgetshook\n");
#endif
		typedef	char*(*REALFGETS)(char*,int,FILE*);
		REALFGETS realfgets = (REALFGETS)get_real_func("fgets");
		if(NULL == realfgets)
		{
			printf("realfun fgets error");
			return NULL;
		}
		int err;
		int ret = fault_fail("fgets",&err);
		if(ret == 0) {
			errno = err;
			stream->_flags |=_IO_ERR_SEEN;
			return NULL;
		}
		return realfgets(s,size,stream);
	}
	int FAULT_(fgetc)(FILE *stream)
	{
#ifdef DEBUG
		printf("fgetchook\n");
#endif
		typedef	int (*REALFGETC)(FILE*);
		REALFGETC realfgetc = (REALFGETC)get_real_func("fgetc");
		if(NULL == realfgetc)
		{
			printf("realfun fgetc error");
			return -1;
		}
		int err;
		int ret = fault_fail("fgetc", &err);
		if(ret == 0) {
			errno = err;
			stream->_flags |=_IO_ERR_SEEN;
			return -1;
		}
		return realfgetc(stream);
	}
	int FAULT_(fputc)(int c, FILE *stream)
	{
#ifdef DEBUG
		printf("fputchook\n");
#endif
		typedef	int (*REALFPUTC)(int,FILE*);
		REALFPUTC realfputc = (REALFPUTC)get_real_func("fputc");
		if(NULL == realfputc)
		{
			printf("realfun fputc error");
			return -1;
		}
		int err;
		int ret = fault_fail("fputc",&err);
		if(ret == 0) {
			errno = err;
			stream->_flags |=_IO_ERR_SEEN;
			return -1;
		}
		return realfputc(c,stream);
	}
	int FAULT_(fputs)(const char *s, FILE *stream)
	{
#ifdef DEBUG
		printf("fputshook\n");
#endif
		typedef	int (*REALFPUTS)(const char*,FILE*);
		REALFPUTS realfputs = (REALFPUTS)get_real_func("fputs");
		if(NULL == realfputs)
		{
			printf("realfun fputs error");
			return -1;
		}
		int err;
		int ret = fault_fail("fputs", &err);
		if(ret == 0) {
			errno = err;
			stream->_flags |=_IO_ERR_SEEN;
			return -1;
		}
		return realfputs(s,stream);		
	}

	//open
	//write
	//close
	//.........
}

//void __attribute__ ((constructor)) fault_disk_file_init(void)
SO_MAIN(fault_disk_file)
{
	SO_MAIN_START(fault_disk_file);
	//attach
	attach_func("fopen", (void *)FAULT_(fopen));
	attach_func("fwrite", (void *)FAULT_(fwrite));
	attach_func("fread", (void *)FAULT_(fread));
	attach_func("fclose", (void *)FAULT_(fclose));
	attach_func("fseek", (void *)FAULT_(fseek));
	attach_func("ftell", (void *)FAULT_(ftell));
	attach_func("fgets", (void *)FAULT_(fgets));
	attach_func("fgetc", (void *)FAULT_(fgetc));
	attach_func("fputc", (void *)FAULT_(fputc));
	attach_func("fputs", (void *)FAULT_(fputs));
}


