#pragma once
#include "src/include/core/tree.hh"
#include "src/include/core/node.h"
#include "src/include/core/header/header.h"
#include "src/include/core/general.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>
#include <optional>
#include <memory>
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
	has_member_store<void(int)>,
	te::relaxed
>>;

namespace N_Core
{
	class Elf
	{
	public:
		std::string _file_name;

		// Shared ptr because an output elf containing sections of this elf will need
		// to extend the lifetime of the memory mapped region.
		std::shared_ptr<boost::interprocess::mapped_region> _region; 
		
		N_Header::Header _header;
		
		Elf(std::shared_ptr<boost::interprocess::mapped_region>&& mapped_region) :
			_region(std::move(mapped_region))
			, _header(N_Header::create_header_from_memory_blob(BinaryBlob(reinterpret_cast<uint8_t*>(_region->get_address()), reinterpret_cast<uint8_t*>(_region->get_address()) + _region->get_size())))
		{
		}

		void store(int) {}

		template<typename T>
		Elf(T&& t):
			Elf(std::forward<std::shared_ptr<boost::interprocess::mapped_region>>(t._region))
		{
			_file_name = std::forward<std::string>(t._file_name);
		}

		Elf(Elf const&):
			_header(N_Header::create_header_from_memory_blob(BinaryBlob(reinterpret_cast<uint8_t*>(_region->get_address()), reinterpret_cast<uint8_t*>(_region->get_address()) + _region->get_size())))
		{

		}

		friend Elf create_elf_from_existing_elf(Elf, std::string const& path_to_elf);
		friend Elf create_elf_from_path_to_file(std::string const& path_to_elf);
	};

	Elf create_elf_from_path_to_file(std::string const& path_to_elf)
	{
		boost::interprocess::file_mapping m_file(path_to_elf.c_str(), boost::interprocess::read_only);
		auto&& memory_region = std::make_shared<boost::interprocess::mapped_region>(m_file, boost::interprocess::read_only);

		return Elf(std::forward<std::shared_ptr<boost::interprocess::mapped_region>>(memory_region)); //Mapped file will be closed when the Elf obj is destructed.
	}

	Elf create_elf_from_existing_elf(Elf existing_elf, std::string const& path_to_elf)
	{
		auto& elf = Elf(std::forward<Elf>(existing_elf));
		elf._file_name = path_to_elf;

		return elf;
	}
}
