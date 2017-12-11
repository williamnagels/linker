#include "src/include/core/elf.h"

#include <algorithm>
namespace N_Core
{
	void N_Core::Elf::remove_section(uint16_t index)
	{
		if (index >= _section_table._sections.size())
		{
			throw std::range_error("No section could be found with the index");
		}
		_header->set_section_header_number_of_entries(_header->get_section_header_number_of_entries() - 1);

		//move logic below to section table
		//For all sections after section with the index the offset must be reduced.
		auto offset_to_subtract = _section_table._sections.at(index)->get_size(); 

		_header->set_section_header_offset(_header->get_section_header_offset() - offset_to_subtract);
		std::for_each(
			_section_table._sections.begin() + (index+1)
			,_section_table._sections.end()
			,[=](auto const& section) { section->set_offset(section->get_offset() - offset_to_subtract);}
		);

		_section_table._sections.erase(_section_table._sections.begin() + index);
		
	}

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
