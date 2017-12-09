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
	// 2. 32-bit vs 64-bit.
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
		explicit Elf(N_Core::N_Header::Class class_of_elf= N_Core::N_Header::Class::ELFCLASS64):
			_region(nullptr)
			,_header()
			,_section_table()
		{
			if (class_of_elf== N_Core::N_Header::Class::ELFCLASS64)
			{
				_header = std::make_unique<N_Header::Header<N_Header::Elf64_Ehdr>>();
			}
			else
			{
				_header = std::make_unique<N_Header::Header<N_Header::Elf32_Ehdr>>();
			}
		}

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
	template <typename T, std::enable_if_t<std::is_same_v<Elf, std::decay_t<T>>, int> a = 0>
	N_Core::Elf create_elf(T&& existing_elf)
	{
		return N_Core::Elf(std::forward<T>(existing_elf));
	}

	// @brief create elf from an existing elf
	// 
	N_Core::Elf create_elf(N_Core::N_Header::Class class_to_use);

	// @brief create elf from a file on disk.
	// 
	Elf create_elf(std::string const& path_to_elf);

	// @brief create elf from an existing elf
	// 
	template <typename T>
	void dump_to_file(T&& path, Elf const& elf)
	{
		std::ofstream output_file;
		output_file.open(std::forward<T>(path), std::ios::out | std::ios::binary);
		dump(output_file, elf);
		output_file.close();
	}
	void dump(std::ostream& stream, Elf const& elf);
}
