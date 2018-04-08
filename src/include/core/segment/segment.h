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
            uint64_t _internal_offset;
            Segment(uint64_t internal_offset):_internal_offset(internal_offset){}
            std::list<std::reference_wrapper<N_Core::N_Section::Section<V, C>>> _sections;

            void calculate_sizes_based_on_sizes_of_sections()
            {
                uint64_t size_in_file = _internal_offset;
                uint64_t size_in_memory = _internal_offset;
                for (auto const& section:_sections)
                {
                    size_in_file += section.get().get_size_in_file();
                    size_in_memory += section.get().get_size();
                }

                set_file_size(size_in_file);
                set_memory_size(size_in_memory);
            }

            uint64_t get_offset() const { return  get(_header_entry, &T::p_offset); }
            void set_offset(uint64_t offset) { set(_header_entry, &T::p_offset, offset); }
            
            Type get_type() const{return  get(_header_entry, &T::p_type); }
            void set_type(Type type) {set(_header_entry, &T::p_type, type);}

            uint64_t get_virtual_address() const{return  get(_header_entry, &T::p_vaddr);}
            void set_virtual_address(uint64_t va){return  set(_header_entry, &T::p_vaddr, va);}

            uint64_t get_physical_address() const{return  get(_header_entry, &T::p_paddr);}  
            void set_physical_address(uint64_t va){return  set(_header_entry, &T::p_paddr, va);}
            

            uint64_t get_file_size() const{return  get(_header_entry, &T::p_filesz);}  
            void set_file_size(uint64_t size){return  set(_header_entry, &T::p_filesz, size);} 

            uint64_t get_memory_size() const{return  get(_header_entry, &T::p_memsz);}   
            void set_memory_size(uint64_t size){return  set(_header_entry, &T::p_memsz, size);}

            Flags get_flags() const{return  get(_header_entry, &T::p_flags);}    
            void set_flags(Flags flags){return  set(_header_entry, &T::p_flags, reinterpret_cast<uint8_t&>(flags));}

            uint64_t get_alignment() const{return  get(_header_entry, &T::p_align);}     
            void set_alignment(uint64_t alignment){ set(_header_entry, &T::p_align,alignment);}     

            template <typename TStream, typename CStream>
			friend std::ostream& operator<<(std::ostream& stream, Segment<TStream, CStream> const& segment);

        };

        template <typename T, typename C>
		std::ostream& operator<<(std::ostream& stream, Segment<T, C> const& segment)
		{
			stream << segment._header_entry;
			// if (section.get_size_in_file())
			// {
			// 	stream.seekp(section.get_offset());

			// 	ostream_visitor<T, C> visitor(stream);
			// 	std::visit(visitor, section._interpreted_content);
			// }

			return stream;
		}
    }
}
