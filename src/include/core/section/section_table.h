#pragma once
#include "src/include/core/section/helpers.h"
#include "src/include/core/section/section_member_types.h"
#include "src/include/core/section/section.h"
#include "src/include/core/header/header.h"
#include "src/include/core/general.h"
#include <vector>
#include <memory>
#include <utility>

#include <boost/iterator/filter_iterator.hpp>

namespace N_Core
{
	template <typename V>
	class Elf;

	namespace N_Section
	{

		// @brief Collection of sections forms the section table.
		// 
		// This class is essentially a wrapper around std::vector
		// to store a variable amount of sections.
		//
		// Note: since sections are stored in a std::vector addresses
		// of section objects may change if the capacity of the
		// vector changes. Do not store references/pointers to sections objects.
		//
		template <typename V>
		class Table
		{
			using T = std::conditional_t<std::is_same_v<V, Bit64>, N_Section::Elf64_Shdr, N_Section::Elf32_Shdr >;
			using SectionTy = Section<V>;
			using InternalStorageTy = std::vector<SectionTy>;
			InternalStorageTy _sections; ///< list of sections assigned to this table.	

			struct CodeSectionIdentifier 
			{
				bool operator()(SectionTy const& section) 
				{ 
					return section.get_type() == Type::SHT_PROGBITS && section.get_flags().SHF_ALLOC && section.get_flags().SHF_EXECINSTR;
				}
			};
			struct DataSectionIdentifier 
			{
				bool operator()(SectionTy const& section)
				{
					return section.get_type() == Type::SHT_PROGBITS && section.get_flags().SHF_ALLOC && !section.get_flags().SHF_EXECINSTR;
				}
			};
			using ConstCodeSectionIterator = boost::filter_iterator<CodeSectionIdentifier, typename InternalStorageTy::const_iterator>;
			using ConstDataSectionIterator = boost::filter_iterator<DataSectionIdentifier, typename InternalStorageTy::const_iterator>;
			using CodeSectionIterator = boost::filter_iterator<CodeSectionIdentifier, typename InternalStorageTy::iterator>;
			using DataSectionIterator = boost::filter_iterator<DataSectionIdentifier, typename InternalStorageTy::iterator>;
		
		public:
			// @brief Get numbers of sections in the section table.
			//
			// @returns number of sections in the section table
			//
			std::size_t size() const { return _sections.size(); }

			typename InternalStorageTy::iterator begin() { return _sections.begin(); }
			typename InternalStorageTy::iterator end() { return _sections.end(); }
			typename InternalStorageTy::const_iterator begin() const { return _sections.begin(); }
			typename InternalStorageTy::const_iterator end() const { return _sections.end(); }
			
			ConstCodeSectionIterator begin_code() const { return ConstCodeSectionIterator(CodeSectionIdentifier{}, begin(), end()); }
			ConstCodeSectionIterator end_code() const { return ConstCodeSectionIterator(CodeSectionIdentifier{}, end(), end()); }
			CodeSectionIterator begin_code() { return CodeSectionIterator(CodeSectionIdentifier{}, begin(), end()); }
			CodeSectionIterator end_code() { return CodeSectionIterator(CodeSectionIdentifier{}, end(), end()); }
			ConstDataSectionIterator begin_data() const { return ConstDataSectionIterator(DataSectionIdentifier{}, begin(), end()); }
			ConstDataSectionIterator end_data() const { return ConstDataSectionIterator(DataSectionIdentifier{}, end(), end()); }
			DataSectionIterator begin_data() { return DataSectionIterator(DataSectionIdentifier{}, begin(), end()); }
			DataSectionIterator end_data() { return DataSectionIterator(DataSectionIdentifier{}, end(), end()); }
			
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
			Index add_section(SectionTy&& section, Index index = Index::Wildcard)
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
				if (!is_valid_index<typename DoesNotSupportWildCard>(_sections, index))
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

			explicit Table() {}
			explicit Table(std::size_t number_of_entries) 
			{
				for (auto i = 0; i < number_of_entries; i++)
				{
					add_section({});
				}
			}

			// @brief Create a new table to store sections in.
			// 
			explicit Table(N_Header::Header<V> const& header, BinaryBlob memory_region)
			{
				auto number_of_entries = header.get_section_header_number_of_entries();
				auto start_of_table = header.get_section_header_offset();
				auto size_of_entry = header.get_section_header_entry_size();


				for (auto i = 0; i < number_of_entries; i++)
				{
					auto header_of_section_entry = start_of_table + size_of_entry * i;
					auto begin_header = memory_region.begin() + header_of_section_entry;
					auto end_header = begin_header + size_of_entry;

					Index index = add_section(SectionTy(BinaryBlob(begin_header, end_header), memory_region));
				}
				for (auto i = 0; i < number_of_entries; i++)
				{
					Index linked_section_index = _sections[i].get_link();

					if (linked_section_index)
					{
						_sections[i].set_linked_section(_sections[linked_section_index]);
					}
				}
				


			}

			// @brief Get section at an index
			// 
			// @param index	0 is the first section.
			//
			// @throws std::invalid_argument if no section with the index exists.
			//
			SectionTy& operator[](Index index)
			{
				if (!are_valid_indices<typename DoesNotSupportWildCard>(_sections, index))
				{
					throw std::invalid_argument(invalid_section_index);
				}
				return _sections.at(index);
			}

			// @brief Get section at an index
			// 
			// @param index	0 is the first section.
			//
			// @throws std::invalid_argument if no section with the index exists.
			//
			SectionTy const& operator[](Index index) const { return const_cast<Table<T>*>(this)->operator[](index); }


	private:
			Index add_section_to_back(SectionTy&& section)
			{
				_sections.emplace_back(std::move(section));
				return _sections.size() - 1;
			}
			Index add_section_at_index(SectionTy&& section, Index index)
			{
				decltype(_sections)::iterator iterator = std::begin(_sections);
				std::advance(iterator, index);

				_sections.insert(iterator, std::move(section));

				return index;
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
		std::ostream& operator<<(std::ostream& stream, Table<T> const& table)
		{
			std::streampos start_of_section_table = stream.tellp();
			int section_index = 0;
			for (auto const& section : table)
			{
				stream.seekp(start_of_section_table + std::streamoff((section_index++) * sizeof(T)), std::ios::beg);
				stream << section;
				return stream;
			}
		}
	}
}