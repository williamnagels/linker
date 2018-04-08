#pragma once
#include "src/include/core/general.h"
#include "src/include/core/section/section.h"
#include "src/include/core/segment/member_types.h"
#include <list>
#include <functional>


namespace N_Core
{
    namespace N_Segment
    {
		template <typename V, typename C>
        struct Segment
        {
            using T = std::conditional_t<std::is_same_v<V, Bit64>, N_Segment::Elf64_Phdr, N_Segment::Elf32_Phdr >;
            MMap::Container<T> _header_entry;
            Segment(uint64_t virtual_address, uint64_t offset){}
            std::list<std::reference_wrapper<N_Core::N_Section::Section<V, C>>> _sections;

            uint64_t get_offset() const { return  get(_header_entry, &T::p_offset); }
            Type get_type() const{return  get(_header_entry, &T::p_type); }
            uint64_t get_virtual_address() const{return  get(_header_entry, &T::p_vaddr);}
            uint64_t get_physical_address() const{return  get(_header_entry, &T::p_paddr);}           
            uint64_t get_file_size() const{return  get(_header_entry, &T::p_filesz);}           
            uint64_t get_memory_size() const{return  get(_header_entry, &T::p_memsz);}    
            Flags get_flags() const{return  get(_header_entry, &T::p_flags);}    
            uint64_t get_alignment() const{return  get(_header_entry, &T::p_align);}           
            void set_offset(uint64_t offset){}
        };
    }
}
