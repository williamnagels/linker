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
			using SectionTy = Section<V, Table>;
			using InternalStorageTy = std::list<SectionTy>;

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
			struct SymbolTableIdentifier
			{
				bool operator()(SectionTy const& section)
				{
					return section.get_type() == Type::SHT_SYMTAB;
				}
			};
			using ConstCodeSectionIterator = boost::filter_iterator<CodeSectionIdentifier, typename InternalStorageTy::const_iterator>;
			using ConstDataSectionIterator = boost::filter_iterator<DataSectionIdentifier, typename InternalStorageTy::const_iterator>;
			using CodeSectionIterator = boost::filter_iterator<CodeSectionIdentifier, typename InternalStorageTy::iterator>;
			using DataSectionIterator = boost::filter_iterator<DataSectionIdentifier, typename InternalStorageTy::iterator>;
			using ConstSymbolTableIterator = boost::filter_iterator<SymbolTableIdentifier, typename InternalStorageTy::const_iterator>;
			using SymbolTableIterator = boost::filter_iterator<SymbolTableIdentifier, typename InternalStorageTy::iterator>;
		
			using SymbolIterator = Iterator<typename SymbolTableIterator, typename SectionTy::SymbolTableTy::Iterator>;
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
			
			ConstSymbolTableIterator begin_symbol_table() const {return ConstSymbolTableIterator(SymbolTableIdentifier{}, begin(), end());}
			ConstSymbolTableIterator end_symbol_table() const { return ConstSymbolTableIterator(SymbolTableIdentifier{}, end(), end()); }
			SymbolTableIterator begin_symbol_table() { return SymbolTableIterator(SymbolTableIdentifier{}, begin(), end()); }
			SymbolTableIterator end_symbol_table() { return SymbolTableIterator(SymbolTableIdentifier{}, end(), end()); }

			SymbolIterator begin_symbol() { return SymbolIterator(begin_symbol_table(), end_symbol_table()); }
			SymbolIterator end_symbol() { return SymbolIterator(end_symbol_table()); }

			~Table() {}
			Table(Table const&) = delete;
			Table(Table&&) = delete;

			explicit Table() {}
			
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

					_sections.emplace_back(*this, BinaryBlob(begin_header, end_header), memory_region);
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
				auto iterator = _sections.begin();
				std::advance(iterator, index);
				return *iterator;
			}

			// @brief Get section at an index
			// 
			// @param index	0 is the first section.
			//
			// @throws std::invalid_argument if no section with the index exists.
			//
			SectionTy const& operator[](Index index) const { return const_cast<Table*>(this)->operator[](index); }
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