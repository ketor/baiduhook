/***************************************************************************
 * 
 * Copyright (c) 2011 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 

#include "AsmHelper.h"

#include <string.h>
#include <dis-asm.h>
#include <errno.h>

struct AsmHelper::InstInfo AsmHelper::_inst_info;

int AsmHelper::Copy(bfd_byte *buf, int bufsize, bfd *abfd, void *src, unsigned int minsize, int *src_dis_len)
{
	int dis_len = 0;
	struct disassemble_info disasm_info;

	init_disassemble_info (&disasm_info, NULL, (fprintf_ftype) empty_fprintf);
	disasm_info.mach = bfd_get_mach(abfd);
	disasm_info.buffer_vma = (bfd_vma)src;
	disasm_info.buffer_length = bufsize;
	disasm_info.buffer = (bfd_byte *)src;

	//用以检查是否存在相对地址
	disasm_info.read_memory_func = buffer_read_memory;
	disasm_info.print_address_func = print_address;

	disassembler_ftype print_insn = disassembler(abfd);
	int count;
	bfd_byte *pc = (bfd_byte *)src;
	bfd_byte *curdst = (bfd_byte *)buf;
	for (; (bfd_vma)pc < (bfd_vma)src + bufsize; pc += count) {
		_inst_info.riprel = 0;
		_inst_info.count = 0;

		count = print_insn((bfd_vma)pc, &disasm_info);
		if (count < 0) {
			break;
		}
		if (_inst_info.riprel) {
			if (count > sizeof(int)) {
				memcpy(curdst, pc, count);
				int offset;
				int i=0;
				for (; i<_inst_info.count; i++) {
					unsigned int tmprel = *(unsigned int *)_inst_info.codep[i] + (bfd_vma)pc + count;
					if (tmprel == _inst_info.realaddr) {
						offset = _inst_info.codep[i] - pc;
						break;
					}
				}
				if (i != _inst_info.count) {
					*(unsigned int *)(curdst+offset) = _inst_info.realaddr - (bfd_vma)curdst - count;
					curdst += count;
				}
			} else if (count == 2) {	/*< jmp\je\jne short */
				int i = 0;
				//jmp short
				if (pc[0] == 0xeb) {
					curdst[i++] = 0xe9;
				} else {
					curdst[i++] = 0x0f;
					curdst[i++] = pc[0] + 0x10;
				}
				int ins_len = i + sizeof(int);
				unsigned int rel32 = _inst_info.realaddr - (bfd_vma)curdst - ins_len;
				*(unsigned int *)(curdst+i) = rel32;
				curdst += ins_len;
			} else {
				fprintf(stderr, " error!!\n");
			}
		} else {
			memcpy(curdst, pc, count);
			curdst += count;
		}
		dis_len += count;

		if (dis_len >= minsize) {
			break;
		}
	}
	*src_dis_len = dis_len;
	return curdst - (bfd_byte *)buf;
}

int AsmHelper::FarJmp(bfd_byte *buf, int bufsize, bfd_vma jmpaddr)
{
	//jmp 0(%rip)
	//跳转到紧接着该指令的地址
	//32位下是个指针，需要把ff25后的地址重写
	bfd_byte jmpcode[] = {0xff, 0x25, 0x00, 0x00, 0x00, 0x00};
#if __WORDSIZE != 64
	bfd_vma pointer = (bfd_vma)buf + sizeof(jmpcode);
#endif

	int needsize = sizeof(jmpcode) + sizeof(jmpaddr);
	if (bufsize < needsize) {
		fprintf(stderr, "[%s] no enough buffer\n", __func__);
		return -1;
	}
	bfd_byte *p = (bfd_byte *)buf;
	memcpy(p, jmpcode, sizeof(jmpcode));
#if __WORDSIZE != 64
	memcpy(p + 2, &pointer, sizeof(pointer));
#endif
	memcpy(p + sizeof(jmpcode), &jmpaddr, sizeof(jmpaddr));
	return needsize;
}


void AsmHelper::print_address (bfd_vma addr, struct disassemble_info *info)
{
	char buf[30];

	sprintf_vma (buf, addr);
	(*info->fprintf_func) (info->stream, "0x%s", buf);

	_inst_info.riprel = 1;
	_inst_info.realaddr = addr;
}

int AsmHelper::buffer_read_memory (bfd_vma memaddr,
	bfd_byte *myaddr,
	unsigned int length,
	struct disassemble_info *info)
{
	unsigned int opb = info->octets_per_byte;
	unsigned int end_addr_offset = length / opb;
	unsigned int max_addr_offset = info->buffer_length / opb;
	unsigned int octets = (memaddr - info->buffer_vma) * opb;

	if (memaddr < info->buffer_vma
		|| memaddr - info->buffer_vma + end_addr_offset > max_addr_offset)
		/* Out of bounds.  Use EIO because GDB uses it.  */
		return EIO;
	memcpy (myaddr, info->buffer + octets, length);
	if (length == sizeof(int)) {
		//printf("[opb:%u][length:%d]\n", opb, length);
		_inst_info.codep[_inst_info.count++] = info->buffer + octets;
	}

	return 0;
}











/* vim: set expandtab ts=4 sw=4 sts=4 tw=100 */
