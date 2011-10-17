/***************************************************************************
 * 
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 

#ifndef  __ELFMANAGER_H_
#define  __ELFMANAGER_H_


#include <stdio.h>
#include <elf.h>

#include "elfdef.h"

typedef struct elf_symbol
{
	char			*name;
	unsigned char	bind;
	unsigned char	type;
	unsigned short	shndx;
	unsigned long	value;
	unsigned long	size;
}esymbol;

typedef struct elf_relsym
{
	unsigned long	offset;
	unsigned long	type;
	unsigned long	symid;
}erelsym;

class ELFManager
{
public:
	ELFManager() {}
	~ELFManager() {}

	/**
	 * @brief 打开文件并读取Elf64_Ehdr
	 *
	 * @param [in/out] filename   : const char*
	 * @return  int 
	 * @retval   
	 * @see 
	 * @note 
	 * @author xuanbiao
	 * @date 2010/04/06 18:40:36
	**/
	int elf_open(const char *filename);
	/**
	 * @brief 关闭文件句柄
	 *
	 * @return  void 
	 * @retval   
	 * @see 
	 * @note 
	 * @author xuanbiao
	 * @date 2010/04/06 18:41:59
	**/
	void elf_close();

	/**
	 * @brief 从pos读取count字节到buf里
	 *
	 * @param [in/out] pos   : int 文件偏移量
	 * @param [in/out] buf   : void* buf指针
	 * @param [in/out] count   : int 读取的字节大小
	 * @return  int 
	 * @retval   
	 * @see 
	 * @note 
	 * @author xuanbiao
	 * @date 2010/04/06 18:42:12
	**/
	int elf_read(int pos, void *buf, int count);

	int elf_read_ehdr(Elf_Ehdr &ehdr);

	int elf_read_shdrs(Elf_Shdr *shdrs, unsigned int shnum);

	/**
	 * @brief 获取读取符号表所需的内存空间
	 *
	 * @return  int 
	 * @retval   
	 * @see 
	 * @note 
	 * @author xuanbiao
	 * @date 2010/04/06 18:43:09
	**/
	int elf_get_symtab_upper_bound();

	/**
	 * @brief 读取符号表信息并保存到symbol_table里
	 *
	 * @param [in/out] symbol_table   : esymbol** 保存符号表信息
	 * @return  int 
	 * @retval   
	 * @see 
	 * @note 
	 * @author xuanbiao
	 * @date 2010/04/06 18:43:47
	**/
	int elf_canonicalize_symtab(esymbol **symbol_table);

	int elf_get_section_upper_bound(unsigned int type, int *pcount = NULL);

	int elf_canonicalize_symbol(esymbol **symbol_table, unsigned int type);

	int elf_get_relsym_upper_bound(int *pcount);

	int elf_canonicalize_relsym(erelsym **rel_table, int max_count);
private:
	const char *_filename;
	int _fd;

private:
	Elf_Ehdr	_ehdr;
	char		_ident[EI_NIDENT];
	bool		_is_32_bit;
};












#endif  //__ELFManager_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
