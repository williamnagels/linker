#pragma once
#include "src/include/core/general.h"
#include "src/include/core/section/section_member_types.h"

#include <variant>
#include <functional>
#include <numeric>


namespace N_Core
{

	namespace N_Section
	{
		/*@brief ELF Section representation.
		*
		* Contains section header and reference to the content of the section.
		*
		*
		*/
		template <typename T>
		class Section
		{
		private:

			// @brief Get address range where the content of the section is stored.
			// 
			// Helper function used when constructing a section from memory mapped elf.
			//
			// @param elf_blob	Full elf memory region to which the section belongs.
			//
			// @returns memory region where the section content is stored.
			//
			BinaryBlob get_content_from_header(N_Core::BinaryBlob elf_blob)
			{
				return BinaryBlob(reinterpret_cast<uint8_t*>(elf_blob.begin() + get_offset()), reinterpret_cast<uint8_t*>(elf_blob.begin() + get_offset() + get_size()));
			}

		public:
			MMap::Container<T> _header_entry;
			MMap::Container<uint8_t> _content;

			// @brief Create a section for a memory mapped elf.
			// 
			// @param header	address range where the header entry of this section is loaded into memory.
			// @param elf_blob	full address range of the elf.
			//
			// The full memory range of the elf is required because the content may be stored anywhere
			// in the address range and is unknown until the header has been parsed.
			//
			explicit Section(N_Core::BinaryBlob header, N_Core::BinaryBlob elf_blob) :
				_header_entry(header.begin())
				, _content(get_content_from_header(elf_blob).begin(), get_content_from_header(elf_blob).end())
			{
			}

			// @brief Create a new section.
			// 
			explicit Section():
				_header_entry()
				, _content()
			{

			}

			uint64_t get_name()const  { return  get(_header_entry, &T::sh_name); }
			void set_name(uint64_t offset) { set(_header_entry, &T::sh_name, offset); };
			Type get_type() const  { return  get(_header_entry, &T::sh_type); }
			Flags get_flags()const  { return static_cast<Flags>( get(_header_entry, &T::sh_flags)); }
			uint64_t get_address()const  { return  get(_header_entry, &T::sh_addr); }
			uint64_t get_offset()const  { return  get(_header_entry, &T::sh_offset); }
			void set_offset(uint64_t offset)  { return  set(_header_entry, &T::sh_offset, offset); };
			uint64_t get_size() const  { return  get(_header_entry, &T::sh_size); }
			void set_size(uint64_t size) { return  set(_header_entry, &T::sh_size, size); }
			uint64_t get_link()const  { return  get(_header_entry, &T::sh_link); }
			uint64_t get_info()const  { return  get(_header_entry, &T::sh_info); }
			uint64_t get_address_alignment()const  { return  get(_header_entry, &T::sh_addralign); }
			uint64_t get_entry_size()const  { return  get(_header_entry, &T::sh_entsize); }
			MMap::Container<uint8_t> const& get_content() const  { return _content;}
			uint64_t get_size_in_file() const  { return (get_type() != SHT_NOBITS) ? get_size() : 0; }
		};

		template <typename T>
		void dump(std::ostream& stream, Section<T> const& section, int section_index)
		{
			stream.seekp(std::streamoff(section_index * sizeof(T)), std::ios::cur);
			stream << section._header_entry;
			if (section.get_size_in_file())
			{
				stream.seekp(section.get_offset());
				stream << section._content;
			}
		}
		template <typename T>
		Section<T> create_section(N_Core::BinaryBlob elf_blob, N_Core::BinaryBlob header_blob)
		{
			return Section<T>(header_blob, elf_blob);
		}

		template <typename T, typename ItTy>
		void update(Section<T>& section, ItTy begin,ItTy end)
		{
			section._content.resize(end-begin);
			std::copy(begin, end, std::begin(section._content));
		}
	}

};
