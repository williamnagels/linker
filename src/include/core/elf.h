#pragma once

#include "src/include/core/header/header.h"
#include "src/include/core/section/section.h"
#include "src/include/core/segment/segment.h"
#include "src/include/core/general.h"
#include "src/include/core/section/filters.h"

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/iterator/filter_iterator.hpp>

#include <string>
#include <fstream>
#include <list>
namespace N_Core
{
	// @brief Class representing an elf loaded from file or a custom created elf.
	//
	// 
	template <typename V>
	class Elf
	{
	public:
		using HeaderTy = N_Header::Header<V>;
		using SectionTy = N_Section::Section<V, Elf>;
		using SegmentTy = N_Segment::Segment<V, Elf>;
		using SectionTableTy = std::list<SectionTy>;
		using SegmentTableTy = std::list<SegmentTy>;
		// This shared ptr keeps the memory mapped elf in memory until
		// it is destructed (and other elfs sharing the counter).
		// This pointer is only set when elf is sourced by an elf on disk.
		// it will be nullptr if an elf is created using custom (not on disk) structs.
		// When moving an elf the region will be moved.
		// When copying an elf the region will be shared.
		boost::interprocess::file_mapping _file;
		std::shared_ptr<boost::interprocess::mapped_region> _region;
	public:
		typename SectionTableTy::iterator begin() { return _section_table.begin(); }
		typename SectionTableTy::iterator end() { return _section_table.end(); }
		typename SectionTableTy::const_iterator begin() const { return _section_table.begin(); }
		typename SectionTableTy::const_iterator end() const { return _section_table.end(); }

		HeaderTy _header; ///< Header of the elf. 
		SectionTableTy _section_table; ///< All sections in this elf.	
		SegmentTableTy _segment_table; ///< All segments in this elf.


		SegmentTy& create_new_segment(uint64_t internal_offset)
		{
			_segment_table.emplace_back(internal_offset);
			return _segment_table.back();
		}

		// @brief Returns true if the elf is loaded from disk
		//
		bool is_memory_mapped() const noexcept 
		{ 
			return _region.get(); 
		};

		uint64_t search_for_symbol(std::string const& name)
		{
			return 0;
		}
		// @brief Returns the region in memory to which the elf is mapped
		// 
		// @throws std::exception if the elf is not memory mapped
		//	use is_memory_mapped() to check if the elf is mapped or not
		//
		BinaryBlob get_memory_mapped_region() const
		{
			if (!is_memory_mapped())
			{
				throw std::invalid_argument("Cannot");
			}
			uint8_t* begin_address = reinterpret_cast<uint8_t*>(_region->get_address());
			return BinaryBlob(begin_address, begin_address + _region->get_size());
		}

		// @brief Construct a new elf.
		//
		explicit Elf() :
			_region(nullptr)
			, _header()
			, _section_table()
		{
		}

		// @brief Construct an elf from memory mapped region and a file name.
		explicit Elf(std::string path):
			_file(path.c_str(), boost::interprocess::read_only)
			, _region(std::make_shared<boost::interprocess::mapped_region>(_file, boost::interprocess::read_only))
			, _header(get_memory_mapped_region())
			, _section_table()
		{
			auto number_of_entries = _header.get_section_header_number_of_entries();
			auto start_of_table = _header.get_section_header_offset();
			auto size_of_entry = _header.get_section_header_entry_size();

			for (auto i = 0; i < number_of_entries; i++)
			{
				auto header_of_section_entry = start_of_table + size_of_entry * i;
				auto begin_header = get_memory_mapped_region().begin() + header_of_section_entry;
				auto end_header = begin_header + size_of_entry;

				_section_table.emplace_back(*this, BinaryBlob(begin_header, end_header), get_memory_mapped_region());
			}
		}

		// @brief Construct an elf from an existing elf and write to file on disk.
		//
		template<typename T, std::enable_if_t<std::is_same_v<Elf, std::decay_t<T>>, int> a = 0>
		explicit Elf(T&& elf) :
			_region(std::forward<T>(elf)._region)
			, _header(std::forward<T>(elf)._header)
			, _section_table(std::forward<T>(elf)._section_table)
		{

		}

		SectionTy const& get_section_at(int index) const
		{
			return const_cast<Elf*>(this)->get_section_at(index);
		}
		SectionTy& get_section_at(int index)
		{
			auto it = std::begin(_section_table);
			std::advance(it, index);
			return *it;
		}

		// @brief Helper function to dump elfs to an output stream.
		// 
		// @param stream	Output stream to dump elf to.
		// @param elf		The elf to write to output stream.
		// 
		template <typename VStream>
		friend std::ostream& operator<<(std::ostream& stream, N_Core::Elf<VStream> const& elf)
		{
			//dump header at offset 0
			stream << elf._header;

			// dump sections; step 1 section entry header size back since first step 
			// is increasing stream pos by its size.
			// stream.seekp(elf._header.get_section_header_offset()- sizeof(typename N_Core::Elf<V>::SectionTy::T));

			// stream << "out";
			// for (auto const& section : elf._section_table)
			// {
			// 	stream.seekp(std::streamoff(sizeof(typename N_Core::Elf<V>::SectionTy::T)), std::ios::cur);
			// 	std::streampos current_table_entry_position = stream.tellp();
			// 	stream << section;
			// 	stream.seekp(current_table_entry_position, std::ios::beg);
			// }

			// dump segments
			stream.seekp(elf._header.get_program_header_offset()- sizeof(typename N_Core::Elf<V>::SegmentTy::T));
			
			for (auto const& segment : elf._segment_table)
			{		
				stream.seekp(std::streamoff(sizeof(typename N_Core::Elf<V>::SegmentTy::T)), std::ios::cur);
				std::streampos current_table_entry_position = stream.tellp();
				stream << segment;
				stream.seekp(current_table_entry_position, std::ios::beg);
			}
			return stream;
		}
	};

	// @brief create elf from an existing elf
	// 
	// @param existing_elf	Elf to use as blueprint for new elf.
	//
	// @returns		new elf based on existing elf.
	//
	template <typename T, typename V, std::enable_if_t<std::is_same_v<Elf<V>, std::decay_t<T>>, int> a = 0>
	N_Core::Elf<V> create_elf(T&& existing_elf)
	{
		return N_Core::Elf(std::forward<T>(existing_elf));
	}

	// @brief write an elf to a file.
	// 
	// @param path	Destination location of the elf.
	// @param elf	The elf to write to disk.
	// 
	// Will overwrite existing file if present.
	// Will create file if it does not exist.
	//
	template <typename T, typename V>//, std::enable_if_t< std::is_convertible_v<T, char const*const>, int> a = 0
	void dump_to_file(T&& path, Elf<V> const& elf)
	{
		std::ofstream output_file;
		output_file.open(std::forward<T>(path), std::ios::out | std::ios::binary);
		output_file << elf;
		output_file.close();
	}


	// @brief Get name of a section
	// 
	// @param elf				Elf from which the section name should be retrieved.
	// @param index_to_lookup	Index of the section to retrieve.
	// 
	template<typename ElfTy>
	std::string get_name(N_Core::Elf<ElfTy> const& elf, N_Core::Index index_to_lookup)
	{
		N_Core::Index index = elf._header.get_section_index_that_contains_strings();
	
		auto const& buffer = elf._section_table[index].get_content();
		std::size_t offset = elf._section_table[index_to_lookup].get_name();

		return std::string(reinterpret_cast<char const*>(&buffer[offset]));
	}

	struct ConvertSymbolToSection
	{
		ConvertSymbolToSection(){}

		template<typename T>
		auto const& operator()(T const& symbol)
		{
			Index index = Index(symbol.get_section_index());
			return symbol.get_parent().get_parent().get_parent().get_section_at(index);			
		}
	};	

}
