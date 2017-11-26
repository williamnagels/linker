#pragma once
#include "src/include/core/header/header.h"
#include "src/include/core/general.h"

#include <string>
#include <optional>
#include <memory>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/iterator.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/tuple.hpp>
#include <boost/type_erasure/same_type.hpp>
#include <boost/type_erasure/member.hpp>
namespace te = boost::type_erasure;

BOOST_TYPE_ERASURE_MEMBER((has_member_store), store, 1)

using Linkable = te::any<boost::mpl::vector<
	te::copy_constructible<>,
	te::relaxed
>>;

namespace N_Core
{
	template <typename T>
	class Elf
	{
	public:
		// Name of source elf or elf to create.
		std::string _file_name;

		// Shared ptr because an output elf containing sections of this elf will need
		// to extend the lifetime of the memory mapped region.
		std::shared_ptr<boost::interprocess::mapped_region> _region;

		// The header is templated because based on type type of elf(64 bit vs 32bit) another memory map (different 
		// member variables) is selected.
		N_Header::Header<T> _header;


		Elf(std::shared_ptr<boost::interprocess::mapped_region>&& mapped_region) :
			_region(std::move(mapped_region))
			, _header(BinaryBlob(reinterpret_cast<uint8_t*>(_region->get_address()), reinterpret_cast<uint8_t*>(_region->get_address()) + _region->get_size()))
		{
		}

		// Create new elf from existing elf. elf will be stored under file_name.
		Elf(Elf const& elf, std::string file_name) :
			_region(elf._region)
			, _header(_header)
			, _file_name(file_name)
		{

		}
		
	};

	// Build elf from path. Will map file into memory.
	Linkable create_elf(std::string const& path_to_elf)
	{
		boost::interprocess::file_mapping m_file(path_to_elf.c_str(), boost::interprocess::read_only);
		auto&& memory_region = std::make_shared<boost::interprocess::mapped_region>(m_file, boost::interprocess::read_only);

		uint8_t* base_address = static_cast<uint8_t*>((*memory_region).get_address());
		uint8_t* end_address = static_cast<uint8_t*>(base_address + memory_region->get_size());
		
		// Doesn't matter if Bit32 or Bit64 is chosen here as we are only interested in wether or not the file
		// is a 64-bit or 32-bit elf. Note that in both cases the indicating variable is at the same offset
		// thus both memory maps can be applied.

		//N_Core::N_Header::Header<N_Core::Bit32>(BinaryBlob(base_address, reinterpret_cast<uint8_t*>(_region->get_address()) + _region->get_size()))
		if (N_Core::N_Header::Header<N_Core::Bit32>(BinaryBlob(base_address,end_address)).is_64bit_header())
		{
			return Elf<N_Core::Bit64>(std::move(memory_region));
		}
		else
		{
			return Elf<N_Core::Bit32>(std::move(memory_region));
		}

	}

	// @brief create elf from an existing elf.
	// 
	template <typename T>
	Linkable create_elf(N_Core::Elf<T> elf, std::string const& path_to_elf)
	{
		return N_Core::Elf<T>(elf, path_to_elf);
	}

	template <typename T>
	void dump(std::ostream& stream, Elf<T> elf)
	{
		dump(elf._header);
	}


}
