/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 


#ifndef  __ASMHELPER_H_
#define  __ASMHELPER_H_

#include <stdio.h>
#include <bfd.h>
#include <bits/wordsize.h>

//FIXME: 一条指令最大长度，暂时先设置0x10
#define	MAX_INSTRUCTION_LEN	0x10


//jmpcode: 0xff, 0x25, 0x00, 0x00, 0x00, 0x00
//FAR_JMP_CODE_LEN = sizeof(jmpcode) + sizeof(long)
#if	__WORDSIZE == 64
#define	FAR_JMP_CODE_LEN	14
#else
#define	FAR_JMP_CODE_LEN	10
#endif

#define	IRAX	0
#define	IRCX	1
#define	IRDX	2
#define	IRBX	3
#define	IRSP	4
#define	IRBP	5
#define	IRSI	6
#define	IRDI	7

#define	IR8		0
#define	IR9		1

//助记符类型
enum {
	MM_NONE = 0,
	MM_REXB
};


class AsmHelper
{
public:
	struct InstInfo {
		int riprel;		/* 1=>存在相对偏移，0=>不存在相对偏移 */
		bfd_vma realaddr;	/* 绝对地址 */
		unsigned int count;
		bfd_byte *codep[4];	/* 指向4字节长数据 */
	};

	static int Copy(bfd_byte *buf, int bufsize, bfd *abfd, void *src, unsigned int minsize, int *src_dis_len);

	static int FarJmp(bfd_byte *buf, int bufsize, bfd_vma jmpaddr);
private:
	static int empty_fprintf(FILE *stream, const char *fmt, ...) 
	{
		return 0;
	}
	static void print_address (bfd_vma addr, struct disassemble_info *info);

	static int buffer_read_memory (bfd_vma memaddr,
		bfd_byte *myaddr,
		unsigned int length,
		struct disassemble_info *info);
private:
	static struct InstInfo _inst_info;
};











#endif  //__ASMHELPER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
