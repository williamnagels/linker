#pragma once
#include "src/include/core/addressable.h"
#include "src/include/core/general.h"
#include "src/include/core/symtab/symbol_table.h"
#include "src/include/core/section/section_member_types.h"
#include "src/include/core/section/asection.h"
#include "src/include/core/locatable.h"

#include "cow.h"

#include <variant>
#include <functional>
#include <numeric>


namespace N_Core
{
	template <typename V>
	class Elf;
	namespace N_Header { class HeaderA;}

	namespace N_Section
	{
		/*@brief ELF Section representation.
		*
		* Contains section header and reference to the content of the section.
		*
		*
		*/
		template <typename T>
		class Section: public ASection
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

			// @brief Construct a sectionf rom an existing section.
			// 
			// @param existing_section existing section to base the new section of
			//
			template <typename T>
			explicit Section(T&& existing_section, std::enable_if_t<std::is_same_v<std::decay_t<T>, Section>, int> = 0) :
				_header_entry(std::forward<T>(section)._header_entry)
				,_content(std::forward<T>(section)._content)
			{
			}

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

			uint64_t get_name()const override { return  get(_header_entry, &T::sh_name); }
			Type get_type() const override { return  get(_header_entry, &T::sh_type); }
			Flags get_flags()const override { return static_cast<Flags>( get(_header_entry, &T::sh_flags)); }
			uint64_t get_address()const override { return  get(_header_entry, &T::sh_addr); }
			uint64_t get_offset()const override { return  get(_header_entry, &T::sh_offset); }
			void set_offset(uint64_t offset) override { return  set(_header_entry, &T::sh_offset, offset); };
			uint64_t get_size()const override { return  get(_header_entry, &T::sh_size); }
			uint64_t get_link()const override { return  get(_header_entry, &T::sh_link); }
			uint64_t get_info()const override { return  get(_header_entry, &T::sh_info); }
			uint64_t get_address_alignment()const override { return  get(_header_entry, &T::sh_addralign); }
			uint64_t get_entry_size()const override { return  get(_header_entry, &T::sh_entsize); }
			MMap::Container<uint8_t> const& get_content() const override { return _content;}
			uint64_t get_size_in_file() const override { return (get_type() != SHT_NOBITS) ? get_size() : 0; }
			std::unique_ptr<ASection> deep_copy() const& override { return std::make_unique<Section>(*this);}
			std::unique_ptr<ASection> deep_copy() && override { return std::make_unique<Section>(std::move(*this));}
		};
	}

};
