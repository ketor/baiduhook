/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 

#ifndef  __ELFDEF_H_
#define  __ELFDEF_H_




typedef struct
{
	unsigned long		e_shoff;
	unsigned int		e_shentsize;
	unsigned int		e_shnum;
}Elf_Ehdr;


typedef struct
{
	unsigned int		sh_type;
	unsigned long		sh_offset;
	unsigned long		sh_size;
	unsigned int		sh_link;
	unsigned long		sh_entsize;
}Elf_Shdr;


typedef struct
{
	unsigned long		st_name;
	unsigned char		st_info;
	unsigned int		st_shndx;
	unsigned long		st_value;
	unsigned long		st_size;
}Elf_Sym;






#endif  //__ELFDEF_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
