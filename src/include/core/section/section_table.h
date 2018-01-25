#pragma once
#include "src/include/core/section/helpers.h"
#include "src/include/core/section/section_member_types.h"
#include "src/include/core/section/section.h"
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
		template <typename T>
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
			Index add_section(Section<T>&& section, Index index = Index::Wildcard)
			{ 
				if (is_wildcard(index))
				{
					return add_section_to_back(std::move(section));
				}
				
				return add_section_at_index(std::move(section), index);
			}

			// @brief Swap section at index1 with section at index2
			//
			// swaps the header entry of index1 with the header entry of index2
			// This does not move around the content of the section only moves the header entry.
			//
			// @throws std::invalid_argument if index1 and index2 are invalid (does not
			// identify a section).
			//
			void swap_section(Index index1, Index index2)
			{
				if (!are_valid_indices<DoesNotSupportWildCard>(index1, index2))
				{
					throw std::invalid_argument(invalid_section_index);
				}

				decltype(_sections)::iterator iterator1 = std::begin(_sections);
				decltype(_sections)::iterator iterator2 = std::begin(_sections);
				std::advance(iterator1, index1);
				std::advance(iterator2, index2);

				std::iter_swap(iterator1, iterator2);
			}

			// @brief Remove section identified by index from the section table
			// 
			// @param index		section to remove. 0 based. remove_section(0) removes the first section from the table
			// @param policy	what the elf should look like after the section is removed. See enum for descriptin of options.
			//
			// @throws std::invalid_argument if index is invalid (larger than amount of sections).
			//
			void remove_section(Index index, SectionRemovalPolicy policy)
			{
				if (!is_valid_index<DoesNotSupportWildCard>(index))
				{
					throw std::range_error(invalid_section_index);
				}

				if (policy == SectionRemovalPolicy::COMPACT)
				{
					auto offset_to_subtract = _sections.at(index).get_size_in_file();

					std::for_each(
						_sections.begin() + (index + 1)
						, _sections.end()
						, [=](auto& section) { section.set_offset(section.get_offset() - offset_to_subtract); }
					);
				}

				auto element_to_delete = _sections.begin() + index;
				_sections.erase(_sections.begin() + index);
			}
			std::vector< Section<T> > _sections; ///< list of sections assigned to this table.

			~Table() {}

			// @brief Create a section table based on an existing section table.
			// 
			// @param existing_table	blueprint for the new table.
			//
			// Will move sections from existing_table to the newly created table.
			//
			Table(Table&& other_table)
			{
				_sections = std::move(other_table)._sections;
			}

			// @brief Create a section table based on an existing section table.
			// 
			// @param existing_table	blueprint for the new table.
			//
			// Will move sections from existing_table to the newly created table.
			//
			Table& operator=(Table&& other_table)
			{
				_sections = std::move(other_table)._sections;
				return *this;
			}

			// @brief Create a section table based on an existing section table.
			// 
			// @param existing_table	blueprint for the new table.
			//
			// Will deep copy sections from existing_table to the newly created table.
			//
			Table(Table const& other_table)
			{
				for (auto const& section : other_table._sections)
				{
					_sections.emplace_back(section);
				}
			}

			// @brief Create a section table based on an existing section table.
			// 
			// @param existing_table	blueprint for the new table.
			//
			// Will deep copy sections from existing_table to the newly created table.
			//
			Table& operator=(Table const& other_table)
			{
				for (auto const& section : other_table._sections)
				{
					_sections.emplace_back(section->deep_copy());
				}
				return *this;
			}

			// @brief Create a new table to store sections in.
			// 
			explicit Table() {}

			Section<T>& get_section_at_index(Index index) { return _sections.at(index); }

			Section<T> const& get_section_at_index(Index index) const { return _sections.at(index); }

			Section<T>  const& get_section_by_offset(uint64_t offset)const
			{
				auto it = std::find_if(
					std::begin(_sections),
					std::end(_sections),
					[](auto const& section) {return section.get_offset() == offset; });

				if (it == std::end(_sections))
				{
					throw std::invalid_argument("No sections found with offset");
				}

			}

			Index get_section_index_by_offset(uint64_t offset) const
			{
				for (auto i = 0; i < std::size(_sections); i++)
				{
					if (get_section_at_index(i).get_offset() == offset)
					{
						return i;
					}
				}
				throw std::invalid_argument("No section found with offset");
			}

	private:
			Index add_section_to_back(Section<T>&& section)
			{
				_sections.push_back(std::move(section));

				return _sections.size() - 1;
			}
			Index add_section_at_index(Section<T>&& section, Index index)
			{
				decltype(_sections)::iterator iterator = std::begin(_sections);
				std::advance(iterator, index);

				_sections.insert(iterator, std::move(section));

				return index;
			}
			struct SupportsWildcard {};
			struct DoesNotSupportWildCard {};
			template <typename V>
			std::enable_if_t<std::is_same_v<V, DoesNotSupportWildCard>, bool> is_valid_index(Index index)
			{
				return index < _sections.size();
			}

			template <typename V>
			std::enable_if_t<std::is_same_v<V, SupportsWildcard>, bool> is_valid_index(Index index)
			{
				return is_valid_index<DoesNotSupportWildCard>(index) || (index == Index::Wildcard);
			}

			template <typename V, typename ...G>
			bool are_valid_indices(G... indices)
			{
				bool rc;
				for (auto&& x : { indices... })
				{
					rc |= is_valid_index<V>(x);
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
		template <typename T>
		void dump(std::ostream& stream, Table<T> const& table)
		{
			std::streampos start_of_section_table = stream.tellp();
			int section_index = 0;
			for (auto const& section : table._sections)
			{
				stream.seekp(start_of_section_table);
				dump(stream, section, section_index);
				section_index++;
			}
		}
	}
}