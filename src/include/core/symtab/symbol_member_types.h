#pragma once
#include <cstdint>
namespace N_Core
{
	namespace N_Symbol
	{
		struct Elf32_Sym
		{
			uint32_t  st_name;
			uint32_t  st_value;
			uint32_t  st_size;
			uint8_t   st_info;
			uint8_t   st_other;
			uint16_t  st_shndx;
		};

		struct Elf64_Sym
		{
			uint32_t  st_name;
			uint8_t   st_info;
			uint8_t   st_other;
			uint16_t  st_shndx;
			uint64_t  st_value;
			uint64_t  st_size;
		};
	}
}