#pragma once
#include <cstdint>
namespace N_Core
{
	namespace N_Relocation
	{

		// S = symbol value
		// A = addend
		// P = offset
		enum Type : uint32_t
		{
			R_X86_64_NONE = 0,
			R_X86_64_64=1, //S + A
			R_X86_64_PC32=2, //S + A - P
		};


		struct Elf32_Rel
		{
			uint32_t r_offset;
			uint32_t r_info;
		};
		struct Elf32_Rela
		{
			uint32_t r_offset;
			uint32_t r_info;
			int32_t r_addend;
		};

		struct Elf64_Rel
		{
			uint64_t r_offset;
			uint64_t r_info;
		};

		struct Elf64_Rela
		{
			uint64_t r_offset;
			uint64_t r_info;
			int64_t r_addend;
		};
	}
}