#pragma once
#include "src/include/core/section/section_table.h"
#include "src/include/core/header/header.h"
#include "src/include/core/section/section.h"
#include "src/include/core/general.h"

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <string>
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
		using HeaderTy = N_Header::Header<V>;
		using SectionTableTy = N_Section::Table<V>;
		// This shared ptr keeps the memory mapped elf in memory until
		// it is destructed (and other elfs sharing the counter).
		// This pointer is only set when elf is sourced by an elf on disk.
		// it will be nullptr if an elf is created using custom (not on disk) structs.
		// When moving an elf the region will be moved.
		// When copying an elf the region will be shared.
		boost::interprocess::file_mapping _file;
		std::shared_ptr<boost::interprocess::mapped_region> _region;

		constexpr bool is_64bit()
		{
			return std::is_same_v<V, Bit64>;
		}
	public:
		HeaderTy _header;
        SectionTableTy _section_table;

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
		template <typename T, std::enable_if_t< std::is_convertible_v<T, char const*const>, int> a = 0 >
		explicit Elf(T&& path):
			_file(path, boost::interprocess::read_only)
			, _region(std::make_shared<boost::interprocess::mapped_region>(_file, boost::interprocess::read_only))
			, _header(get_memory_mapped_region())
			, _section_table(_header, get_memory_mapped_region())
		{
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
	/*template <typename V, typename T, std::enable_if_t< std::is_convertible_v<T, char const*const>, int> a = 0 >
	N_Core::Elf<V> create_elf(T&& path)
	{
		boost::interprocess::file_mapping m_file(path, boost::interprocess::read_only);
		auto&& memory_region = std::make_shared<boost::interprocess::mapped_region>(m_file, boost::interprocess::read_only);

		return N_Core::Elf<V>(std::move(memory_region));
	}*/

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

	// @brief Helper function to dump elfs to an output stream.
	// 
	// @param stream	Output stream to dump elf to.
	// @param elf		The elf to write to output stream.
	// 
	template <typename V>
	std::ostream& operator<<(std::ostream& stream, N_Core::Elf<V> const& elf)
	{
		stream << elf._header;
		stream.seekp(elf._header.get_section_header_offset());
		stream << elf._section_table;
		return stream;
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
}
