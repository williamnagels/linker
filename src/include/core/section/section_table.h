#pragma once
#include "src/include/core/section/helpers.h"
#include "src/include/core/section/section_member_types.h"
#include "src/include/core/section/asection.h"
#include <vector>
#include <memory>
#include <utility>

namespace N_Core
{
	template <typename V>
	class Elf;

	namespace N_Section
	{
		//class ASection;
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

			~Table();

			// @brief Create a section table based on an existing section table.
			// 
			// @param existing_table	blueprint for the new table.
			//
			// Will move sections from existing_table to the newly created table.
			//
			Table(Table&& existing_table);
			Table& operator=(Table&&);
			// @brief Create a section table based on an existing section table.
			// 
			// @param existing_table	blueprint for the new table.
			//
			// Will deep copy sections from existing_table to the newly created table.
			//
			Table(Table const& existing_table);
			Table& operator=(Table const&);
			// @brief Create a new table to store sections in.
			// 
			explicit Table() {}

			ASection& get_section_at_index(Index index);
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
	}
}