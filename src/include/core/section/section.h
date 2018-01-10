#pragma once
#include "src/include/core/addressable.h"
#include "src/include/core/general.h"
#include "src/include/core/symtab/symbol_table.h"
#include "src/include/core/section/section_member_types.h"

#include "cow.h"

#include <variant>
#include <functional>
#include <numeric>
namespace N_Core
{
	class Elf;
	namespace N_Header { class HeaderA;}


	namespace N_Section
	{
		static const std::string invalid_section_index= "No section could be found with the index";

		// @brief Section identifier.
		// 
		// Identifies a unique section in the section table. Used in various API functions.
		//
		// Note: First section has index 0.
		//
		struct Index
		{
			uint16_t _value;
			static const decltype(_value) Wildcard = std::numeric_limits<decltype(_value)>::max();
			operator decltype(_value)(){return _value;}

			Index(decltype(_value) value):
				_value(value){}
		};
		// @brief Wether or not the index is the wildcard index.
		// 
		// Used in the API to check if the index of some section is the wildcard section.
		//
		bool is_wildcard(Index index);

		// @brief Public API of a section.
		// 
		// This class exposes many setters & getters that can be used to change
		// the value of basically all members of a section as described by the ELF standard.
		//
		class ASection
		{
		public:
			virtual ~ASection() {}
			virtual MMap::Container<uint8_t>const& get_content() const = 0;
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

		// @brief Create a section from already allocated memory.
		// 
		// @param header	Address range where the header entry of this section is loaded into memory.
		// @param elf_blob	Full address range of the elf (needed to look up the content).
		//
		// @returns the section
		//
		// @note In no way does this function take ownership of the data at the memory regions
		//   of the content and section header. As long as the section exists the memory must contain
		//   the section content.
		//
		std::unique_ptr<ASection> create_section(N_Core::BinaryBlob elf_blob, N_Core::BinaryBlob header_blob);

		// @brief Create an empty section.
		// 
		// @param is_64_bit		Create 32-bit or 64-bit header for section.
		//
		// @returns the section
		//
		std::unique_ptr<ASection> create_section(bool is_64_bit);

		// @brief Set offset for an entity.
		// 
		// Offset is the location of an entity in the ELF.
		// Types that can provide a location are mainly segments and ELFs.
		//
		// For example: an entity can be part of segment but must not be.
		// Sections outside of a segment are valid in the ELF standard and
		// are allowed to exist. Segments require an offset inside the ELF container.
		//
		// The container will provide a good offset for the locatable considering 	
		// its size (e.g. section size) and aligment. 
		// 
		// @param entity	Entity that needs to have its offset set.
		// @param container	Entity that can be used to provide an offset for the entity.
		//
		//
		template<typename Locatable, typename Container>
		void add_entity_to_container(Locatable&& entity, Container const& container)
		{
			//uint64_t offset = container.get_offset_for_region(locatable->get_size(), locatable->get_aligment());
			//locatable->set_offset(offset);
			//container.entities_contained.push_back(std::move(entity));

			container.absorb(entity);

		}

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
			// @param index		index of section to add. See create_section(...), 
			//   use N_Section::WildcardIndex if index doesnt matter;
			//
			// @param section	section to add to the table
			//
			// Note 1: if a section already exists for the index, that section
			// has its index increased.
			//
			// Note 2: the ordering of section has nothing to do with the
			// placement of sections in the elf. See get_offset() and get_size()
			// to find the location of the section in the file.
			//
			void add_section(std::unique_ptr<ASection>&& section, Index index = Index::Wildcard);

			// @brief Swap section at index1 with section at index2
			//
			// swaps the header entry of index1 with the header entry of index2
			// This does not move around the content of the section only moves the header entry.
			//
			// @throws std::invalid_argument if index1 and index2 are invalid (does not
			// identify a section).
			//
			void swap_section(Index index1, Index index2);

			// @brief Remove section identified by index from the section table
			// 
			// @param index		section to remove. 0 based. remove_section(0) removes the first section from the table
			// @param policy	what the elf should look like after the section is removed. See enum for descriptin of options.
			//
			// @throws std::invalid_argument if index is invalid (larger than amount of sections).
			//
			void remove_section(Index index, SectionRemovalPolicy policy);

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

		private:
			void add_section_to_back(std::unique_ptr<ASection>&& section);
			void add_section_at_index(std::unique_ptr<ASection>&& section, Index index);

			struct SupportsWildcard {};
			struct DoesNotSupportWildCard {};
			template <typename T>
			std::enable_if_t<std::is_same_v<T, DoesNotSupportWildCard>, bool> is_valid_index(Index index)
			{
				return index < _sections.size();
			}

			template <typename T>
			std::enable_if_t<std::is_same_v<T, SupportsWildcard>, bool> is_valid_index(Index index)
			{
				return is_valid_index<DoesNotSupportWildCard>(index) || (index == Index::Wildcard);
			}

			template <typename T, typename ...G>
			bool are_valid_indices(G... indices)
			{
				bool rc;
				for (auto&& x : { indices... })
				{
					rc |= is_valid_index<T>(x);
				}
				return rc;
			}
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
