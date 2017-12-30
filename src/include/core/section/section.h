#pragma once
#include "src/include/core/addressable.h"
#include "src/include/core/general.h"
#include "src/include/core/symtab/symbol_table.h"
#include "src/include/core/section/section_member_types.h"

#include <variant>
#include <functional>

namespace N_Core
{
	class Elf;
	namespace N_Header { class HeaderA;}


	namespace N_Section
	{

		// @brief Public API of a section.
		// 
		// This class exposes many setters & getters that can be used to change
		// the value of basically all members of a section as described by the ELF standard.
		//
		class ASection
		{
		public:
			virtual ~ASection() {}
			virtual BinaryBlob get_content() const = 0;
			virtual uint64_t get_name() const = 0;
			virtual Type get_type() const = 0;
			virtual Flags get_flags() const = 0;
			virtual uint64_t get_address()const = 0;
			virtual uint64_t get_offset()const = 0;
			virtual void set_offset(uint64_t offset) = 0;
			virtual uint64_t get_size()const = 0;
			virtual uint64_t get_link() const = 0;
			virtual uint64_t get_info()const = 0;
			virtual uint64_t get_address_alignment()const = 0;
			virtual uint64_t get_entry_size()const = 0;
			virtual uint64_t get_size_in_file() const = 0;
			virtual std::unique_ptr<ASection> deep_copy() const& = 0;
			virtual std::unique_ptr<ASection> deep_copy() && = 0;
		};

		std::unique_ptr<ASection> create_section(N_Core::BinaryBlob elf_blob, N_Core::BinaryBlob header_blob);
		std::unique_ptr<ASection> create_section(bool is_64_bit);

		// @brief Collection of sections forms the section table.
		// 
		// This class is essentially a wrapper around std::vector
		// to store a variable amount of sections.
		//
		// Note: since sections are stored in a std::vector addresses
		// of section objects may change if the capacity of the
		// vector changes. Do not store references/pointers to sections objects.
		//
		class Table
		{

		public:
			// @brief Add section to the section table.
			// 
			// @param section	section to add to the table
			//
			// Create the section using one of the create_section(...) free functions.
			//
			template <typename T>
			void add_section(T&& section)
			{
				_sections.push_back(std::forward<T>(section));
			}

			// @brief Remove section identified by index from the section table
			// 
			// @param index		section to remove. 0 based. remove_section(0) removes the first section from the table
			// @param policy	what the elf should look like after the section is removed. See enum for descriptin of options.
			//
			// @throws std::range_error if index is invalid (larger than amount of sections).
			//
			void remove_section(uint16_t section_index, SectionRemovalPolicy policy);

			std::vector<std::unique_ptr<ASection>> _sections; ///< list of sections assigned to this table.
			
			// @brief Create a section table based on an existing section table.
			// 
			// @param existing_table	blueprint for the new table.
			//
			// Will move sections from existing_table to the newly created table.
			//
			Table(Table&& existing_table);
			
			// @brief Create a section table based on an existing section table.
			// 
			// @param existing_table	blueprint for the new table.
			//
			// Will deep copy sections from existing_table to the newly created table.
			//
			Table(Table const& existing_table);

			// @brief Create a new table to store sections in.
			// 
			explicit Table() {}
		};
		

		// @brief Dump a section table to a ostream.
		// 
		// @param stream	Stream to dump table to.
		//
		// @precondition	The cursor of the stream should already be correct for the section table
		//					entry. This means that the cursor is supposed to be pointing at position 
		//					'section table offset' as defined in the elf header. This function
		//					will modify the cursor to write section content.
		//
		// @precondition	The cursor may be anywhere. Do not assume anything about its position.
		//					
		void dump(std::ostream& stream, Table const& table);

		// @brief Create section table
		// 
		// @param elf		Elf that defines what the table will look like.
		//
		// Method works both for memory mapped elfs and customly created elfs. 
		// The amount of sections as defined in the header will be created.
		//
		// @returns created table.
		//
		Table create_section_table(N_Core::Elf const& elf);
		
		// @brief Create section table from existing section table
		// 
		// @param existing_table	
		//
		// @returns table with same sections as existing_table
		//
		template <typename T, std::add_pointer_t<std::enable_if_t<std::is_same_v<std::decay_t<T>, Table>>> =0>
		Table create_section_table(T&& existing_table)
		{
			return Table(std::forward<T>(existing_table));
		}

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
			COW_MemoryBlob<T> _header_entry;
			COW_MemoryBlob<uint8_t> _content;
			BinaryBlob _header_blob;///< 32 or 64-bit header depending on the elf it is contained in.
			BinaryBlob _content_blob; ///< Content of the section (e.g. code).


			// @brief Construct a sectionf rom an existing section.
			// 
			// @param existing_section existing section to base the new section of
			//
			template <typename T>
			explicit Section(T&& existing_section, std::enable_if_t<std::is_same_v<std::decay_t<T>, Section>, int> = 0) :
				_header_entry(std::forward<T>(section)._header_entry)
				,_header_blob(section._header_blob)
				,_content_blob(section._content_blob)
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
				_header_entry(header)
				,_header_blob(header)
				,_content_blob(get_content_from_header(elf_blob))
			{
			}

			// @brief Create a new section.
			// 
			explicit Section():
				_header_entry()
				, _header_blob()
				, _content_blob()
			{

			}

			uint64_t get_name()const override { return get(_header_entry, &T::sh_name); }
			Type get_type() const override { return get(_header_entry, &T::sh_type); }
			Flags get_flags()const override { return static_cast<Flags>(get(_header_entry, &T::sh_flags)); }
			uint64_t get_address()const override { return get(_header_entry, &T::sh_addr); }
			uint64_t get_offset()const override { return get(_header_entry, &T::sh_offset); }
			void set_offset(uint64_t offset) override { return set(_header_entry, &T::sh_offset, offset); };
			uint64_t get_size()const override { return get(_header_entry, &T::sh_size); }
			uint64_t get_link()const override { return get(_header_entry, &T::sh_link); }
			uint64_t get_info()const override { return get(_header_entry, &T::sh_info); }
			uint64_t get_address_alignment()const override { return get(_header_entry, &T::sh_addralign); }
			uint64_t get_entry_size()const override { return get(_header_entry, &T::sh_entsize); }
			BinaryBlob get_content() const override { return _content_blob; }
			uint64_t get_size_in_file() const override { return (get_type() != SHT_NOBITS) ? get_size() : 0; }
			std::unique_ptr<ASection> deep_copy() const& override { return std::make_unique<Section>(*this);}
			std::unique_ptr<ASection> deep_copy() && override { return std::make_unique<Section>(std::move(*this));}
		};
	}

};
