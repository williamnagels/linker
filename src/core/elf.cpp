#include "src/include/core/elf.h"

namespace N_Core
{
	N_Core::Elf create_elf(N_Core::N_Header::Class class_to_use)
	{
		return N_Core::Elf(class_to_use);
	}

	void dump(std::ostream& stream, N_Core::Elf const& elf)
	{
		dump(stream, *elf._header);
	}

	// @brief create elf from a file on disk.
	// 
	N_Core::Elf create_elf(std::string const& path_to_elf)
	{
		boost::interprocess::file_mapping m_file(path_to_elf.c_str(), boost::interprocess::read_only);
		auto&& memory_region = std::make_shared<boost::interprocess::mapped_region>(m_file, boost::interprocess::read_only);

		return N_Core::Elf(std::move(memory_region));
	}
}
