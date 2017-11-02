#pragma once
#include "src/include/core/tree.hh"
#include "src/include/core/node.h"
#include "src/include/core/header/header.h"
#include "src/include/core/general.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>
namespace N_Core
{
	class Elf
	{
	public:
		std::string _file_name;
		boost::interprocess::mapped_region _region;
		
		N_Header::Header _header;
		
		
		Elf(boost::interprocess::mapped_region&& mapped_region) :
			_region(std::move(mapped_region)),
			_header(N_Header::create_header_from_memory_blob(BinaryBlob(reinterpret_cast<uint8_t*>(_region.get_address()), reinterpret_cast<uint8_t*>(_region.get_address()) + _region.get_size())))
		{
		}


		friend Elf create_elf_from_path_to_file(std::string const& path_to_elf);
	};

	Elf create_elf_from_path_to_file(std::string const& path_to_elf)
	{
		boost::interprocess::file_mapping m_file(path_to_elf.c_str(), boost::interprocess::read_only);
		boost::interprocess::mapped_region region(m_file, boost::interprocess::read_only);
		return Elf(std::move(region)); //Mapped file will be closed when the Elf obj is destructed.
	}


}
