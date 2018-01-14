#include "src/include/core/elf.h"
#include "src/include/core/section/section.h"
#include <algorithm>
#include <iostream>
namespace N_Core
{
	void N_Core::Elf::remove_section(uint16_t index, N_Core::N_Section::SectionRemovalPolicy policy)
	{
		auto offset_to_subtract = _section_table._sections.at(index)->get_size_in_file();
		
		//throws if index is not valid that's why offset must first be retrieved.
		_section_table.remove_section(index, policy); 

		_header->set_section_header_number_of_entries(_header->get_section_header_number_of_entries() - 1);

		if (policy == N_Section::SectionRemovalPolicy::COMPACT)
		{
			_header->set_section_header_offset(_header->get_section_header_offset() - offset_to_subtract);
		}


		
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



	void N_Core::Elf::create_section_table()
	{
		auto number_of_entries = _header->get_section_header_number_of_entries();
		auto start_of_table = _header->get_section_header_offset();
		auto size_of_entry = _header->get_section_header_entry_size();

		if (is_memory_mapped())
		{
			for (auto i = 0; i < number_of_entries; i++)
			{
				auto header_of_section_entry = start_of_table + size_of_entry * i;
				auto begin_header = get_memory_mapped_region().begin() + header_of_section_entry;
				auto end_header = begin_header + size_of_entry;

				auto header_range = boost::make_iterator_range(begin_header, end_header);
				auto section = N_Section::create_section(get_memory_mapped_region(), header_range);
				_section_table.add_section(std::move(section));
			}
		}
		else
		{
			for (auto i = 0; i < number_of_entries; i++)
			{
				_section_table.add_section(N_Section::create_section(_header->get_class() == N_Core::N_Header::Class::ELFCLASS64));
			}
		}
	}
}
