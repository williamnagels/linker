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
		stream.seekp(elf._header->get_section_header_offset());
		dump(stream, elf._section_table);
	}

	bool N_Core::Elf::is_memory_mapped() const noexcept { return _region.get(); };

	BinaryBlob N_Core::Elf::get_memory_mapped_region() const
	{
		if (!is_memory_mapped())
		{
			throw std::exception("Cannot");
		}

		return BinaryBlob(reinterpret_cast<uint8_t*>(_region->get_address()), reinterpret_cast<uint8_t*>(_region->get_address()) + _region->get_size());
	}

	N_Core::Elf::Elf(N_Core::N_Header::Class class_of_elf) :
		_region(nullptr)
		, _header(N_Header::create_header(class_of_elf))
		, _section_table()
	{
	}

}
