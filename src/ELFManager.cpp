/***************************************************************************
 * 
 * Copyright (c) 2010 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "ELFManager.h"


int ELFManager::elf_get_section_upper_bound(unsigned int type, int *pcount)
{
	int ret;
	Elf_Shdr shdrs[_ehdr.e_shnum];

	ret = elf_read_shdrs(shdrs, _ehdr.e_shnum);
	if (ret < 0) {
		return -1;
	}
	int count = 0;
	for (unsigned int i=0; i<_ehdr.e_shnum; i++) {
		if (shdrs[i].sh_type == type) {
			count += shdrs[i].sh_size/shdrs[i].sh_entsize;
		}
	}
	if(pcount != NULL) {
		*pcount = count;
	}
	int needsize = count * sizeof(esymbol *) + count * sizeof(esymbol);
	return needsize;
}
int ELFManager::elf_canonicalize_symbol(esymbol **symbol_table, int unsigned type)
{
	int ret;
	Elf_Shdr shdrs[_ehdr.e_shnum];

	ret = elf_read_shdrs(shdrs, _ehdr.e_shnum);
	if (ret < 0) {
		return -1;
	}
	unsigned int i = 0;
	for (; i<_ehdr.e_shnum; i++) {
		if (shdrs[i].sh_type == type) {
			break;
		}
	}
	if (i == _ehdr.e_shnum) {
		return -1;
	}
	Elf_Shdr *symtab = &shdrs[i];
	//读取name
	Elf_Shdr *link = &shdrs[symtab->sh_link];
	char *sym_name = (char *)malloc(link->sh_size);
	if (sym_name == NULL) {
		fprintf(stderr, "malloc error!\n");
		return -1;
	}
	ret = elf_read(link->sh_offset, sym_name, link->sh_size);
	if (ret < 0) {
		return -1;
	}
	//读取符号表
	int count = symtab->sh_size/symtab->sh_entsize;

	esymbol *esym = (esymbol *)&symbol_table[count];
	if(_is_32_bit) {
		Elf32_Sym syms[count];
		ret = elf_read(symtab->sh_offset, syms, symtab->sh_size);
		if (ret < 0) {
			return -1;
		}
		for (int idx=0; idx<count; idx++) {
			symbol_table[idx] = &esym[idx];
			//printf("name:%s!, symbol_table:%llx\n", &sym_name[syms[idx].st_name], symbol_table[idx]);
			symbol_table[idx]->name = &sym_name[syms[idx].st_name];
			//printf("debug3!\n");
			symbol_table[idx]->bind = ELF32_ST_BIND(syms[idx].st_info);
			symbol_table[idx]->type = ELF32_ST_TYPE(syms[idx].st_info);
			symbol_table[idx]->shndx = syms[idx].st_shndx;
			symbol_table[idx]->value = syms[idx].st_value;
			symbol_table[idx]->size = syms[idx].st_size;
		}
	} else {
		Elf64_Sym syms[count];
		ret = elf_read(symtab->sh_offset, syms, symtab->sh_size);
		if (ret < 0) {
			return -1;
		}
		for (int idx=0; idx<count; idx++) {
			symbol_table[idx] = &esym[idx];
			//printf("name:%s!, symbol_table:%llx\n", &sym_name[syms[idx].st_name], symbol_table[idx]);
			symbol_table[idx]->name = &sym_name[syms[idx].st_name];
			//printf("debug3!\n");
			symbol_table[idx]->bind = ELF64_ST_BIND(syms[idx].st_info);
			symbol_table[idx]->type = ELF64_ST_TYPE(syms[idx].st_info);
			symbol_table[idx]->shndx = syms[idx].st_shndx;
			symbol_table[idx]->value = syms[idx].st_value;
			symbol_table[idx]->size = syms[idx].st_size;
		}
	}
	return count;
}

int ELFManager::elf_get_relsym_upper_bound(int *pcount)
{
	*pcount = 0;

	int count;
	int need_size = elf_get_section_upper_bound(SHT_REL, &count);
	*pcount += count;

	need_size += elf_get_section_upper_bound(SHT_RELA, &count);
	*pcount += count;

	return need_size;
}

int ELFManager::elf_canonicalize_relsym(erelsym **rel_table, int max_count)
{
	int ret;
	Elf_Shdr shdrs[_ehdr.e_shnum];

	ret = elf_read_shdrs(shdrs, _ehdr.e_shnum);
	if (ret < 0) {
		return -1;
	}
	int count = 0;
	int cur_idx = 0;
	erelsym *erel = (erelsym *)&rel_table[max_count];
	for (unsigned int i = 0; i<_ehdr.e_shnum; i++) {
		if (shdrs[i].sh_type == SHT_REL || shdrs[i].sh_type == SHT_RELA) {
			char *rel = (char *)malloc(shdrs[i].sh_size);
			if (rel == NULL) {
				fprintf(stderr, "malloc error!\n");
				return -1;
			}
			int num = shdrs[i].sh_size/shdrs[i].sh_entsize;
			count += num;
			ret = elf_read(shdrs[i].sh_offset, rel, shdrs[i].sh_size);
			if (ret < 0) {
				return -1;
			}
			if(_is_32_bit) {
				if (shdrs[i].sh_type==SHT_REL) {
					Elf32_Rel *prel = (Elf32_Rel *)rel;
					for (int idx=0; idx<num; idx++) {
						rel_table[cur_idx] = &erel[cur_idx];
						rel_table[cur_idx]->offset = prel[idx].r_offset;
						rel_table[cur_idx]->type = ELF32_R_TYPE(prel[idx].r_info);
						rel_table[cur_idx]->symid = ELF32_R_SYM(prel[idx].r_info);
						cur_idx++;
					}
				} else {
					Elf32_Rela *prel = (Elf32_Rela *)rel;
					for (int idx=0; idx<num; idx++) {
						rel_table[cur_idx] = &erel[cur_idx];
						rel_table[cur_idx]->offset = prel[idx].r_offset;
						rel_table[cur_idx]->type = ELF32_R_TYPE(prel[idx].r_info);
						rel_table[cur_idx]->symid = ELF32_R_SYM(prel[idx].r_info);
						//printf("rela[cur_idx:%d] offset:0x%lx, type:%d, symid:%d\n", 
						//	cur_idx, rel_table[cur_idx]->offset, rel_table[cur_idx]->type, rel_table[cur_idx]->symid);
						cur_idx++;
					}
				}
			} else {
				if (shdrs[i].sh_type==SHT_REL) {
					Elf64_Rel *prel = (Elf64_Rel *)rel;
					for (int idx=0; idx<num; idx++) {
						rel_table[cur_idx] = &erel[cur_idx];
						rel_table[cur_idx]->offset = prel[idx].r_offset;
						rel_table[cur_idx]->type = ELF64_R_TYPE(prel[idx].r_info);
						rel_table[cur_idx]->symid = ELF64_R_SYM(prel[idx].r_info);
						cur_idx++;
					}
				} else {
					Elf64_Rela *prel = (Elf64_Rela *)rel;
					for (int idx=0; idx<num; idx++) {
						rel_table[cur_idx] = &erel[cur_idx];
						rel_table[cur_idx]->offset = prel[idx].r_offset;
						rel_table[cur_idx]->type = ELF64_R_TYPE(prel[idx].r_info);
						rel_table[cur_idx]->symid = ELF64_R_SYM(prel[idx].r_info);
						//printf("rela[cur_idx:%d] offset:0x%lx, type:%d, symid:%d\n", 
						//	cur_idx, rel_table[cur_idx]->offset, rel_table[cur_idx]->type, rel_table[cur_idx]->symid);
						cur_idx++;
					}
				}
			}
			if(rel) {
				free(rel);
			}
		}
	}
	return count;
}

int ELFManager::elf_get_symtab_upper_bound()
{
	return elf_get_section_upper_bound(SHT_SYMTAB);
}

int ELFManager::elf_canonicalize_symtab(esymbol **symbol_table)
{
	return elf_canonicalize_symbol(symbol_table, SHT_SYMTAB);
}

int ELFManager::elf_open(const char *filename)
{
	//只读模式打开
	_fd = open(filename, O_RDONLY);
	if (_fd < 0) {
		perror("open fail!");
		return -1;
	}
	_filename = filename;

	int ret = elf_read(0, _ident, sizeof(_ident));
	if (ret < 0) {
		fprintf(stderr, "read e_ident error!\n");
		return -1;
	}
	_is_32_bit = (_ident[EI_CLASS] != ELFCLASS64);

	ret = elf_read_ehdr(_ehdr);
	if(ret != 0) {
		return -1;
	}
	return _fd;
}


void ELFManager::elf_close()
{
	if (_fd > 0) {
		close(_fd);
	}
}

int ELFManager::elf_read(int pos, void *buf, int count)
{
	if (_fd < 0) {
		return -1;
	}
	int ret = lseek(_fd, pos, SEEK_SET);
	if (ret < 0) {
		perror("lseek error!");
		return -2;
	}
	ret = read(_fd, buf, count);
	if (ret != count) {
		fprintf(stderr, "read byte:%d\n", ret);
		return -3;
	}
	return ret;
}

int ELFManager::elf_read_ehdr(Elf_Ehdr &ehdr)
{
	int ret;
	if(_is_32_bit) {
		Elf32_Ehdr ehdr32;
		ret = elf_read(0, &ehdr32, sizeof(ehdr32));
		if (ret < 0) {
			fprintf(stderr, "read ehdr32 error!\n");
			return -1;
		}
		ehdr.e_shoff = ehdr32.e_shoff;
		ehdr.e_shentsize = ehdr32.e_shentsize;
		ehdr.e_shnum = ehdr32.e_shnum;
	} else {
		Elf64_Ehdr ehdr64;
		ret = elf_read(0, &ehdr64, sizeof(ehdr64));
		if (ret < 0) {
			fprintf(stderr, "read ehdr error!\n");
			return -1;
		}
		ehdr.e_shoff = ehdr64.e_shoff;
		ehdr.e_shentsize = ehdr64.e_shentsize;
		ehdr.e_shnum = ehdr64.e_shnum;
	}
	return 0;
}


int ELFManager::elf_read_shdrs(Elf_Shdr *shdrs, unsigned int shnum)
{
	int ret;
	if(_is_32_bit) {
		Elf32_Shdr shdrs32[shnum];

		int shsize = shnum * _ehdr.e_shentsize;
		ret = elf_read(_ehdr.e_shoff , shdrs32, shsize);
		if (ret < 0) {
			return -1;
		}
		for (unsigned int i=0; i<shnum; i++) {
			shdrs[i].sh_type = shdrs32[i].sh_type;
			shdrs[i].sh_offset = shdrs32[i].sh_offset;
			shdrs[i].sh_size = shdrs32[i].sh_size;
			shdrs[i].sh_link = shdrs32[i].sh_link;
			shdrs[i].sh_entsize = shdrs32[i].sh_entsize;
		}
	} else {
		Elf64_Shdr shdrs64[shnum];

		int shsize = shnum * _ehdr.e_shentsize;
		ret = elf_read(_ehdr.e_shoff , shdrs64, shsize);
		if (ret < 0) {
			return -1;
		}
		for (unsigned int i=0; i<shnum; i++) {
			shdrs[i].sh_type = shdrs64[i].sh_type;
			shdrs[i].sh_offset = shdrs64[i].sh_offset;
			shdrs[i].sh_size = shdrs64[i].sh_size;
			shdrs[i].sh_link = shdrs64[i].sh_link;
			shdrs[i].sh_entsize = shdrs64[i].sh_entsize;
		}
	}
	return 0;
}




/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
