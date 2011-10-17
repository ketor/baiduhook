/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
#include "BinaryFile.h"

#include <stdlib.h>
#include <string.h>

#ifdef	HAVE_BFD_DEMANGLE

#define DMGL_PARAMS  (1 << 0)   /* Include function args */
#define DMGL_ANSI    (1 << 1)   /* Include const, volatile, etc */
#include <bfd.h>

#else

#include <cxxabi.h> /* for demangle */

#endif
//#include <bits/stl_algo.h>

#include "ELFManager.h"

BinaryFile::BinaryFile()
{
	_abfd = NULL;
}

BinaryFile::~BinaryFile()
{
}


int BinaryFile::Init(char *filename)
{
	Destroy();

	bfd_init();

	_abfd = bfd_openr(filename, NULL); 
	if (_abfd == NULL) { 
		perror("bfd_openr");
		return -1;
	} 
	bfd_boolean ret = bfd_check_format(_abfd, bfd_object);
	if (ret == false) { 
		perror("bfd_check_format_matches");
		return -2;
	}

	int result = InitSymbols(_symbols, SYM_FUNC);
	if (result < 0) {
		fprintf(stderr, "get symbols error!\n");
		return -3;
	}
	result = InitRelsym(_rels);
	if (result < 0) {
		fprintf(stderr, "get rels error!\n");
		return -3;
	}
	return 0;
}

void BinaryFile::Destroy()
{
	if (_abfd) {
		bfd_close(_abfd);
		_abfd = NULL;
	}
	_symbols.clear();
}

int BinaryFile::getSymbol(char *symbol_name, Symbol &symbol)
{
	int len = strlen(symbol_name);
	symbol_dict::iterator it = _symbols.begin();
	for(; it != _symbols.end(); ++it) {
		if(strncmp(it->first.c_str(), symbol_name, len) == 0) {
			symbol = it->second;
			return 0;
		}
	}
	return -1;
	/*
	symbol_dict::iterator it;
	it = find_if(_symbols.begin(), _symbols.end(), bind2nd(near_symbol(), symbol_name));
	if (it == _symbols.end()) {
		fprintf(stderr, "symbol:'%s' is not found!\n", symbol_name);
		return -1;
	}
	symbol = it->second;
	return 0;
	*/
}

int BinaryFile::getRelocs(char *symbol_name, RelSym &relsym)
{
	int len = strlen(symbol_name);
	relsym_dict::iterator it = _rels.begin();
	for(; it != _rels.end(); ++it) {
		if(strncmp(it->first.c_str(), symbol_name, len) == 0) {
			relsym = it->second;
			return 0;
		}
	}
	//fprintf(stderr, "[%s]symbol:'%s' is not found!\n", __FUNCTION__, symbol_name);
	return -1;
	/*
	it = find_if(_rels.begin(), _rels.end(), bind2nd(near_symbol<relsym_dict::value_type>(), symbol_name));
	if (it == _rels.end()) {
		fprintf(stderr, "[%s]symbol:'%s' is not found!\n", __FUNCTION__, symbol_name);
		return NULL;
	}
	return it->second;
	*/
}

int BinaryFile::InitSymbols(symbol_dict &symbols, int sym_flag)
{
	ELFManager elf;

	//open
	elf.elf_open(_abfd->filename);
	//获取符号表所需大小
	long storage_needed = elf.elf_get_symtab_upper_bound (); 
	if (storage_needed < 0) {
		fprintf(stderr, "elf_get_symtab_upper_bound\n"); 
		return -1;
	}
	if (storage_needed == 0) { 
		fprintf(stderr, "storage_needed is 0\n"); 
		return -1; 
	} 

	esymbol **symbol_table = NULL;
	symbol_table = (esymbol **) malloc (storage_needed); 
	if (symbol_table == NULL) {
		perror("malloc"); 
		return -1;
	}
	long number_of_symbols = elf.elf_canonicalize_symtab (symbol_table); 

	if (number_of_symbols < 0) {
		fprintf(stderr, "elf_canonicalize_symtab"); 
		return -1;
	}
	int type = 0;

	if (sym_flag & SYM_FUNC) {
		type = STT_FUNC;
	} 

	for(long i=0; i<number_of_symbols; i++)
	{ 
		char *name = symbol_table[i]->name;
		bfd_vma value = symbol_table[i]->value;
		if (symbol_table[i]->type != type
			|| value == 0
			//|| symbol_table[i]->size < minsize	//小于跳转指令长度则忽略
			|| symbol_table[i]->shndx == 0) { 
			continue; 
		}
#ifdef	HAVE_BFD_DEMANGLE
		char *demangle_name = bfd_demangle(NULL, name, DMGL_PARAMS | DMGL_ANSI);
		if (demangle_name != NULL) {
			name = demangle_name;
		}
#else
		//FIXME: 规避libstd++低版本demangle("FT_SUPERDOC_cmp")出core的bug
		if(strcmp(name, "FT_SUPERDOC_cmp") == 0) {
			symbols[name].addr = (void *)value;
			symbols[name].size = symbol_table[i]->size;
			continue;
		}
		//demangle
		int status;
		char *demangle_name = abi::__cxa_demangle(name, NULL, NULL, &status);
		if (status == 0) {
			name = demangle_name;
		}
#endif
		symbols[name].addr = (void *)value;
		symbols[name].size = symbol_table[i]->size;

		if (demangle_name != NULL) {
			free(demangle_name);
		}
	}

	//free
	if (symbol_table) {
		free(symbol_table);
	}

	elf.elf_close();
	return 0;
}

int BinaryFile::InitRelsym(relsym_dict &rels)
{
	ELFManager elf;

	//open
	elf.elf_open(_abfd->filename);

	//rel symbol
	int rel_count;
	long relsym_needed = elf.elf_get_relsym_upper_bound (&rel_count); 
	if (relsym_needed <= 0) {
		fprintf(stderr, "elf_get_relsym_upper_bound: SHT_REL\n"); 
		return -1;
	}
	erelsym **rel_table = (erelsym **) malloc (relsym_needed); 
	if (rel_table == NULL) {
		perror("malloc"); 
		return -1;
	}
	long number_of_relsym = elf.elf_canonicalize_relsym (rel_table, rel_count); 
	if (number_of_relsym < 0) {
		fprintf(stderr, "elf_canonicalize_symtab"); 
		return -1;
	}

	//获取符号表所需大小
	long dynsym_needed = elf.elf_get_section_upper_bound (SHT_DYNSYM); 
	if (dynsym_needed <= 0) {
		fprintf(stderr, "elf_get_section_upper_bound: SHT_DYNSYM\n"); 
		return -1;
	}

	esymbol **symbol_table = NULL;
	symbol_table = (esymbol **) malloc (dynsym_needed); 
	if (symbol_table == NULL) {
		perror("malloc"); 
		return -1;
	}
	long number_of_symbols = elf.elf_canonicalize_symbol (symbol_table, SHT_DYNSYM); 

	if (number_of_symbols < 0) {
		fprintf(stderr, "elf_canonicalize_symtab"); 
		return -1;
	}

	//do something
	/*
	for(long i=0; i<number_of_symbols; i++)
	{ 
		char *name = symbol_table[i]->name;
		//bfd_vma value = symbol_table[i]->value;
		if (symbol_table[i]->type != type) { 
			continue; 
		}
		printf("[idx:%ld] name:%s\n", i, name);
	}
	*/

	for(long i=0; i<number_of_relsym; i++)
	{ 
		if(rel_table[i]->type != R_X86_64_JUMP_SLOT) {
			continue;
		}
		char *name = symbol_table[rel_table[i]->symid]->name;

#ifdef	HAVE_BFD_DEMANGLE
		char *demangle_name = bfd_demangle(NULL, name, DMGL_PARAMS | DMGL_ANSI);
		if (demangle_name != NULL) {
			name = demangle_name;
		}
#else
		//demangle
		int status;
		char *demangle_name = abi::__cxa_demangle(name, NULL, NULL, &status);
		if (status == 0) {
			name = demangle_name;
		}
#endif
		rels[name].realname = symbol_table[rel_table[i]->symid]->name;
		rels[name].pgot = (void *)rel_table[i]->offset;
		//printf("[idx:%ld] name:%s, pgot:0x%lx, addr:0x%lx\n", 
		//	i, name, rels[name].pgot, rels[name].realname.c_str());

		if (demangle_name != NULL) {
			free(demangle_name);
		}
	}

	//free
	if (rel_table) {
		free(rel_table);
	}
	if (symbol_table) {
		free(symbol_table);
	}

	elf.elf_close();
	return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
