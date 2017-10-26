#pragma once
#include "src/include/core/tree.hh"
#include "src/include/core/node.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>
namespace N_Core
{
	class Elf
	{
		std::string _file_name;
		tree<N_Core::Node> _tree;
		
		template<typename T>
		Elf(T&& mapped_region):
			_region(std::forward<T>(mapped_region))
		{}
		boost::interprocess::mapped_region _region;

		friend void create_elf_from_memory_map(std::string const& path_to_elf);
	};

	void create_elf_from_memory_map(std::string const& path_to_elf)
	{
		//Create a file mapping
		boost::interprocess::file_mapping m_file(path_to_elf.c_str(), boost::interprocess::read_only);
		boost::interprocess::mapped_region region(m_file, boost::interprocess::read_only);

		Elf elf(std::move(region));
	}


}
