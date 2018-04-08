#pragma once
#include <cstdint>
namespace N_Core
{
	namespace N_Segment
	{

        struct Flags
        {
            uint8_t PF_X : 1;
            uint8_t PF_W : 1;
            uint8_t PF_R : 1;
        };

        enum Type : uint32_t
        {
            PT_NULL = 0,
            PT_LOAD = 1,
            PT_DYNAMIC = 2,
            PT_INTERP = 3,
            PT_NOTE = 4,
            PT_SHLIB = 5,
            PT_PHDR = 6
        };

        struct Elf32_Phdr
        {
                Type          p_type;
                uint32_t      p_offset;
                uint32_t      p_vaddr;
                uint32_t      p_paddr;
                uint32_t      p_filesz;
                uint32_t      p_memsz;
                uint32_t      p_flags;
                uint32_t      p_align;
        };

        struct Elf64_Phdr
        {
                Type          p_type;
                uint32_t      p_flags;
                uint64_t      p_offset;
                uint64_t      p_vaddr;
                uint64_t      p_paddr;
                uint64_t      p_filesz;
                uint64_t      p_memsz;
                uint64_t      p_align;
        };
    }
}