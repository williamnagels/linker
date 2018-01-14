#pragma once
#include "src/include/core/section/section_table.h"
#include "src/include/core/header/header.h"
#include "src/include/core/general.h"

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <string>
#include <optional>
#include <memory>
#include <fstream>

namespace N_Core
{
	// @brief Class representing an elf loaded from file or a custom created elf.
	//
	// 0. Creating an elf.
	// ================================================================================================
	// The 'create_elf(...)' free functions are prefered instead of using the ctors directly.
	// Following overloads are available to create elfs:
	//  1. file path
	//  2. class type (create elf without existing file. class=32-bit or 64-bit)
	//  3. elf instance (duplicate functionality).
	//
	// 1. 32-bit vs 64-bit.
	// ================================================================================================
	// This class can be used to represent 32-bit and 64-bit elfs. The difference
	// is detected when the header object is created in the elf constructor.
	// At that point different parsing implementations are initialized using the pimpl idiom.
	// The memory layout for most entities (section,segment, symbol....) differs between 32-bit and 
	// 64-bit elfs.
	//
	// 2. R-value support
	// ===============================================================================================
	// If copying of an elf is required try using move semantics. Because of the
	// nature of the pimpl idiom, elfs are more efficiently moved instead of copied.
	// Copying requires a deep copy of the allocated pointer while
	// moving can move the implementation pointer from one elf to another.
	//
	template <typename V>
	class Elf
	{
	private:
		// This shared ptr keeps the memory mapped elf in memory until
		// it is destructed (and other elfs sharing the counter).
		// This pointer is only set when elf is sourced by an elf on disk.
		// it will be nullptr if an elf is created using custom (not on disk) structs.
		// When moving an elf the region will be moved.
		// When copying an elf the region will be shared.
		std::shared_ptr<boost::interprocess::mapped_region> _region;

		// @brief Create section table
		// 
		// Method works both for memory mapped elfs and customly created elfs. 
		// The amount of sections as defined in the header will be created.
		//
		void create_section_table()
		{
			auto number_of_entries = _header.get_section_header_number_of_entries();
			auto start_of_table = _header.get_section_header_offset();
			auto size_of_entry = _header.get_section_header_entry_size();

			if (is_memory_mapped())
			{
				for (auto i = 0; i < number_of_entries; i++)
				{
					auto header_of_section_entry = start_of_table + size_of_entry * i;
					auto begin_header = get_memory_mapped_region().begin() + header_of_section_entry;
					auto end_header = begin_header + size_of_entry;

					auto header_range = boost::make_iterator_range(begin_header, end_header);
					auto section = N_Section::create_section(get_memory_mapped_region(), header_range);
					_section_table.add_section(std::move(section));
				}
			}
			else
			{
				for (auto i = 0; i < number_of_entries; i++)
				{
					_section_table.add_section(N_Section::create_section(_header.get_class() == N_Core::N_Header::Class::ELFCLASS64));
				}
			}
		}

		constexpr bool is_64bit()
		{
			return std::is_same_v<V, Bit64>;
		}
	public:
		// The header is templated because based on type type of elf(64 bit vs 32bit) a different
		// memory map (different member variables) is selected.
		//
		N_Header::Header<
			std::conditional_t<
				std::is_same_v<V, Bit64>
				, N_Header::Elf64_Ehdr
				, N_Header::Elf32_Ehdr
			>
		> _header;

		N_Section::Table _section_table;

		// @brief Returns true if the elf is loaded from disk
		//
		bool is_memory_mapped() const noexcept 
		{ 
			return _region.get(); 
		};

		// @brief Returns the region in memory to which the elf is mapped
		// 
		// @throws std::exception if the elf is not memory mapped
		//	use is_memory_mapped() to check if the elf is mapped or not
		//
		BinaryBlob get_memory_mapped_region() const
		{
			if (!is_memory_mapped())
			{
				throw std::exception("Cannot");
			}

			return BinaryBlob(reinterpret_cast<uint8_t*>(_region->get_address()), reinterpret_cast<uint8_t*>(_region->get_address()) + _region->get_size());
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
		// You probably do not want to use directly but instead use the free functions: create_elf
		//
		template <typename T, std::enable_if_t<std::is_same_v<std::shared_ptr<boost::interprocess::mapped_region>, std::decay_t<T>>, int> a = 0>
		explicit Elf(T&& mapped_region) :
			_region(std::forward<T>(mapped_region))
			, _header(get_memory_mapped_region())
			, _section_table()
		{
			create_section_table();
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


		// @brief Remove section identified by index from the elf.
		// 
		// Will also update elf header to reflect the changes.	
		//
		// @param index		section to remove. 0 based. remove_section(0) removes the first section from the elf.
		// @param policy	what the elf should look like after the section is removed. See enum for descriptin of options.
		//
		// @throws std::range_error if index is invalid (larger than amount of sections).
		//
		void remove_section(uint16_t index, N_Core::N_Section::SectionRemovalPolicy policy)
		{
			auto offset_to_subtract = _section_table._sections.at(index)->get_size_in_file();

			//throws if index is not valid that's why offset must first be retrieved.
			_section_table.remove_section(index, policy);

			_header.set_section_header_number_of_entries(_header.get_section_header_number_of_entries() - 1);

			if (policy == N_Section::SectionRemovalPolicy::COMPACT)
			{
				_header.set_section_header_offset(_header.get_section_header_offset() - offset_to_subtract);
			}
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

	// @brief create elf from a file on disk.
	// 
	// @param path	Location of the elf on disk.
	//
	// @throws boost::interprocess::interprocess_exception if file not found
	//
	// @returns elf as stored in the file.
	//
	template <typename V, typename T, std::enable_if_t< std::is_convertible_v<T, char const*const>, int> a = 0 >
	N_Core::Elf<V> create_elf(T&& path)
	{
		boost::interprocess::file_mapping m_file(path, boost::interprocess::read_only);
		auto&& memory_region = std::make_shared<boost::interprocess::mapped_region>(m_file, boost::interprocess::read_only);

		return N_Core::Elf<V>(std::move(memory_region));
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
		dump(output_file, elf);
		output_file.close();
	}

	// @brief Helper function to dump elfs to an output stream.
	// 
	// @param stream	Output stream to dump elf to.
	// @param elf		The elf to write to output stream.
	// 
	template <typename V>
	void dump(std::ostream& stream, N_Core::Elf<V> const& elf)
	{
		dump(stream, elf._header);
		stream.seekp(elf._header.get_section_header_offset());
		dump(stream, elf._section_table);
	}

}
