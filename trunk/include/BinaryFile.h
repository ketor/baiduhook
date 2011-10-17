/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 

#ifndef  __BINARYFILE_H_
#define  __BINARYFILE_H_

#include <string>
#include <map>
#include <bfd.h>

#define DMGL_PARAMS  (1 << 0)   /* Include function args */
#define DMGL_ANSI    (1 << 1)   /* Include const, volatile, etc */

class BinaryFile
{
public:
	struct Symbol
	{
		void			*addr;
		unsigned long	size;
	};
	struct RelSym
	{
		void			*pgot;
		std::string		realname;
	};
	typedef std::map<std::string, Symbol> symbol_dict;
	typedef std::map<std::string, RelSym> relsym_dict;

	BinaryFile();
	~BinaryFile();

	int Init(char *filename);

	void Destroy();

	int getSymbol(char *symbol_name, Symbol &symbol);

	int getRelocs(char *symbol_name, RelSym &relsym);

	bfd * getBFD()
	{
		return _abfd;
	}
private:
	enum {
		SYM_FUNC = 1,
		SYM_OBJECT = 2
	};
	int InitSymbols(symbol_dict &symbols, int sym_flag);
	int InitRelsym(relsym_dict &rels);

	bfd				*_abfd;
	symbol_dict		_symbols;
	relsym_dict		_rels;
};


/*
//查找相近符号名
struct near_symbol : public std::binary_function<BinaryFile::symbol_dict::value_type, std::string, bool>
{
	bool operator()(const BinaryFile::symbol_dict::value_type &x, const std::string &name) const 
	{
		if (x.first.compare(0, name.size(), name) == 0) {
			return true;
		}
		return false;
	}
};

template<typename T>
struct near_symbol : public std::binary_function<T, char *, bool>
{
	//bool operator()(const BinaryFile::symbol_dict::value_type &x, const std::string &name) const 
	bool operator()(const T &x, const char * name) const 
	{
		if (strcmp(x.first.c_str(), name) == 0) {
			return true;
		}
		return false;
	}
};
*/




#endif  //__BINARYFILE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
