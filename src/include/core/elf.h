#pragma once
#include "src/include/core/section/section.h"
#include "src/include/core/header/header.h"
#include "src/include/core/general.h"


#include <string>
#include <optional>
#include <memory>
#include <fstream>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>


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
	// At that point using pimpl different parsing implementations are initialized.
	// The memory layout for most entities (section,segment, symbol....) differs between 32-bit and 
	// 64-bit elfs.
	//
	// 2. R-value support
	// ===============================================================================================
	// If copying of an elf is required try using move semantics. Because of the
	// nature of the pimpl idiom, elfs are more efficiently moved instead of copied.
	// Copying requires a deep copy of the allocated pointer while
	// moving can move the allocated implementation pointer from one elf to another.
	//
	class Elf
	{
	public:
		// Shared ptr because an output elf containing sections of this elf will need
		// to extend the lifetime of the memory mapped region.
		std::shared_ptr<boost::interprocess::mapped_region> _region;

		// The header is templated because based on type type of elf(64 bit vs 32bit) another memory map (different 
		// member variables) is selected.
		std::unique_ptr<N_Header::HeaderA> _header;

		N_Section::Table _section_table;

		// Construct a new elf.
		explicit Elf(N_Core::N_Header::Class class_of_elf = N_Core::N_Header::Class::ELFCLASS64);

		// Construct an elf from memory mapped region and a file name.
		// You probably do not want to use directly but instead use the free functions: create_elf
		template <typename T>
		explicit Elf(T&& mapped_region, std::enable_if_t<std::is_same_v<std::shared_ptr<boost::interprocess::mapped_region>, std::decay_t<T>>, int> a = 0):
			_region(std::forward<T>(mapped_region))
			,_header(std::move(N_Header::create_header(BinaryBlob(reinterpret_cast<uint8_t*>(_region->get_address()), reinterpret_cast<uint8_t*>(_region->get_address()) + _region->get_size()))))
			,_section_table(std::move(N_Section::create_section_table(*this)))
		{
		}

		// Construct an elf from an existing elf and write to file on disk.
		template<typename T, std::enable_if_t<std::is_same_v<Elf, std::decay_t<T>>, int> a = 0>
		explicit Elf(T&& elf) :
			_region(std::forward<T>(elf)._region)
			,_header(std::forward<T>(elf)._header->deep_copy())
			,_section_table(std::forward<T>(elf)._section_table)
		{
		}
		
	};
	
	// @brief create elf from an existing elf
	// 
	// @param existing_elf	Elf to use as blueprint for new elf.
	//
	template <typename T, std::enable_if_t<std::is_same_v<Elf, std::decay_t<T>>, int> a = 0>
	N_Core::Elf create_elf(T&& existing_elf)
	{
		return N_Core::Elf(std::forward<T>(existing_elf));
	}

	// @brief create a new elf without blueprint
	// 
	// @param class_to_use	Is the elf to create 2-bit or 64-bit elf?
	//
	N_Core::Elf create_elf(N_Core::N_Header::Class class_to_use);

	// @brief create elf from a file on disk.
	// 
	// @param path	Location of the elf on disk.
	//
	// @throws boost::interprocess::interprocess_exception if file not found
	//
	template <typename T, std::enable_if_t< std::is_convertible_v<T, char const*const>, int> a = 0 >
	N_Core::Elf create_elf(T&& path)
	{
		boost::interprocess::file_mapping m_file(path, boost::interprocess::read_only);
		auto&& memory_region = std::make_shared<boost::interprocess::mapped_region>(m_file, boost::interprocess::read_only);

		return N_Core::Elf(std::move(memory_region));
	}

	// @brief write an elf to a file.
	// 
	// @param path	Destination location of the elf.
	// @param elf	The elf to write to disk.
	// 
	// Will overwrite existing file if present.
	template <typename T, std::enable_if_t< std::is_convertible_v<T, char const*const>, int> a = 0>
	void dump_to_file(T&& path, Elf const& elf)
	{
		std::ofstream output_file;
		output_file.open(std::forward<T>(path), std::ios::out | std::ios::binary);
		dump(output_file, elf);
		output_file.close();
	}

	// @brief Helper function to dump elfs to an output stream.
	// 
	// @param stream	Output stream to dump elf to.
	// @param elf		The elf to write to disk.
	// 
	void dump(std::ostream& stream, Elf const& elf);
}
