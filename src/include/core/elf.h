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
		std::unique_ptr<N_Header::HeaderA> _header;

		template <typename T, typename T2>
		Elf(T&& mapped_region, T2&& file_name, std::enable_if_t<std::is_same_v<std::shared_ptr<boost::interprocess::mapped_region>, std::decay_t<T>>, int> a = 0):
			_file_name(std::forward<T2>(file_name))
			,_region(std::forward<T>(mapped_region))
			,_header(nullptr)
		{
			BinaryBlob blob = BinaryBlob(reinterpret_cast<uint8_t*>(_region->get_address()), reinterpret_cast<uint8_t*>(_region->get_address()) + _region->get_size());

			if (N_Core::N_Header::Header<N_Core::N_Header::Elf32_Ehdr>(blob).is_64bit_header())
			{
				_header = std::make_unique<N_Header::Header<N_Header::Elf64_Ehdr>>(blob);
			}
			else
			{
				_header = std::make_unique<N_Header::Header<N_Header::Elf32_Ehdr>>(blob);
			}
		}

		// Create new elf from existing elf. elf will be stored under file_name.
		template<typename T, typename T2>
		Elf(T&& elf, T2&& file_name, std::enable_if_t<std::is_same_v<Elf, std::decay_t<T>>, int> a = 0) :
			Elf(std::forward<T>(elf)._region, std::forward<T2>(file_name))
		{
		}
		
	};
	
	// @brief create elf from a file on disk.
	// 
	Elf create_elf(std::string const& path_to_elf)
	{
		boost::interprocess::file_mapping m_file(path_to_elf.c_str(), boost::interprocess::read_only);
		auto&& memory_region = std::make_shared<boost::interprocess::mapped_region>(m_file, boost::interprocess::read_only);
		
		return Elf(std::move(memory_region), path_to_elf);
	}

	// @brief create elf from an existing elf.
	// 
	N_Core::Elf create_elf(N_Core::Elf elf, std::string const& path_to_elf)
	{
		return N_Core::Elf(elf, path_to_elf);
	}

	/*
	template <typename T>
	void dump(std::ostream& stream, Elf<T> elf)
	{
		dump(elf._header);
	}*/


}
