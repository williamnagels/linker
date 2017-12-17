#pragma once
#include <cstdint>
namespace N_Core
{
	namespace N_Section
	{
		enum SectionRemovalPolicy
		{
			/*
			When removing a section a gap is created. Offsets of following
			sections/entities (e.g. section table) are not corrected and the region
			that previously contained the section's content will contain zeroes.
			Size of the elf is only reduced by the size of the section header entry.
			*/
			GAP,
			/*
			The gap created by removal of a section is collapsed. 
			all sections following the section have their offset reduced by the size 
			of the removed section effectively reducing the size of the elf by
			size of the removed section + section header entry.
			*/
			COMPACT
		};

		enum Flags: uint64_t
		{
			SHF_WRITE = 0x1,
			SHF_ALLOC = 0x2,
			SHF_EXECINSTR = 0x4,
			SHF_MERGE = 0x10,
			SHF_STRINGS = 0x20,
			SHF_INFO_LINK = 0x40,
			SHF_LINK_ORDER = 0x80,
			SHF_OS_NONCONFORMING = 0x100,
			SHF_GROUP = 0x200,
			SHF_TLS = 0x400,
			SHF_MASKOS = 0x0ff00000,
			SHF_ORDERED = 0x40000000,
			SHF_EXCLUDE = 0x80000000,
			SHF_MASKPROC = 0xf0000000
		};



		enum Type : uint32_t
		{
			SHT_NULL = 0,
			SHT_PROGBITS = 1,
			SHT_SYMTAB = 2,
			SHT_STRTAB = 3,
			SHT_RELA = 4,
			SHT_HASH = 5,
			SHT_DYNAMIC = 6,
			SHT_NOTE = 7,
			SHT_NOBITS = 8,
			SHT_REL = 9,
			SHT_SHLIB = 10,
			SHT_DYNSYM = 11,
			SHT_INIT_ARRAY = 14,
			SHT_FINI_ARRAY = 15,
			SHT_PREINIT_ARRAY = 16,
			SHT_GROUP = 17,
			SHT_SYMTAB_SHNDX = 18,
			SHT_LOOS = 0x60000000,
			SHT_LOSUNW = 0x6ffffff7,
			SHT_SUNW_ANNOTATE = 0x6ffffff7,
			SHT_SUNW_DEBUGSTR = 0x6ffffff8,
			SHT_SUNW_DEBUG = 0x6ffffff9,
			SHT_SUNW_move = 0x6ffffffa,
			SHT_SUNW_COMDAT = 0x6ffffffb,
			SHT_SUNW_syminfo = 0x6ffffffc,
			SHT_SUNW_verdef = 0x6ffffffd,
			SHT_SUNW_verneed = 0x6ffffffe,
			SHT_SUNW_versym = 0x6fffffff,
			SHT_HISUNW = 0x6fffffff,
			SHT_HIOS = 0x6fffffff,
			SHT_LOPROC = 0x70000000,
			SHT_SPARC_GOTDATA = 0x70000000,
			SHT_HIPROC = 0x7fffffff,
			SHT_LOUSER = 0x80000000,
			SHT_HIUSER = 0xffffffff,
		};

		struct Elf64_Shdr 
		{
			uint32_t        sh_name;
			Type		    sh_type;
			uint64_t        sh_flags;
			uint64_t        sh_addr;
			uint64_t        sh_offset;
			uint64_t        sh_size;
			uint32_t        sh_link;
			uint32_t        sh_info;
			uint64_t        sh_addralign;
			uint64_t        sh_entsize;
		};
		struct Elf32_Shdr 
		{
			uint32_t        sh_name;
			Type			sh_type;
			uint32_t		sh_flags;
			uint32_t		sh_addr;
			uint32_t        sh_offset;
			uint32_t	    sh_size;
			uint32_t		sh_link;
			uint32_t		sh_info;
			uint32_t        sh_addralign;
			uint32_t        sh_entsize;
		};
	}
}