#include "src/include/core/section/section.h"
#include "src/include/core/symtab/symbol_table.h"
#include "src/include/core/elf.h"
#include "src/include/core/pimpl_deducer.h"
#include <iostream>
namespace N_Core
{
	namespace N_Section
	{
		

		namespace
		{
			template <typename T>
			void dump_if_correct_type(std::ostream& stream, std::unique_ptr<ASection> const& section, int section_index)
			{
				auto ptr = dynamic_cast<Section<T> const*>(section.get());
				if (ptr)
				{
					stream.seekp(std::streamoff(section_index * sizeof(T)), std::ios::cur);
					stream << ptr->_header_entry;
					if (ptr->get_size_in_file() != 0)
					{
						stream.seekp(ptr->get_offset());
						N_Core::dump(stream, ptr->_content_blob);
					}
				}
			}
		}

		void dump(std::ostream& stream, Table const& table)
		{
			std::streampos start_of_section_table = stream.tellp();
			int section_index = 0;
			for (auto const& section : table._sections)
			{
				stream.seekp(start_of_section_table);
				dump_if_correct_type<Elf32_Shdr>(stream, section, section_index);
				dump_if_correct_type<Elf64_Shdr>(stream, section, section_index);
				section_index++;
			}
		}

		std::unique_ptr<ASection> create_section(N_Core::BinaryBlob elf_blob, N_Core::BinaryBlob header_blob)
		{
			switch (header_blob.size())
			{
			case sizeof(Elf32_Shdr) :
				return std::make_unique<Section<Elf32_Shdr>>(header_blob, elf_blob);
			case sizeof(Elf64_Shdr) :
				return std::make_unique<Section<Elf64_Shdr>>(header_blob, elf_blob);
			default:
				throw std::invalid_argument("Blob is of unexpected size");
			}

		}


		std::unique_ptr<ASection> create_section(bool is_64_bit)
		{
			if (is_64_bit)
			{
				return  std::make_unique<Section<Elf64_Shdr>>();
			}
			else
			{
				return std::make_unique<Section<Elf32_Shdr>>();
			}
		}

		Table::Table(Table&& other_table)
		{
			_sections = std::move(other_table)._sections;
		}

		Table::Table(Table const& other_table)
		{

			for (auto const& section : other_table._sections)
			{
				_sections.emplace_back(section->deep_copy());
			}
		}

		void Table::remove_section(uint16_t section_index, SectionRemovalPolicy policy)
		{
			if (section_index >= _sections.size())
			{
				throw std::range_error("No section could be found with the index");
			}

			if (policy == SectionRemovalPolicy::COMPACT)
			{
				auto offset_to_subtract = _sections.at(section_index)->get_size_in_file();

				std::for_each(
					_sections.begin() + (section_index + 1)
					, _sections.end()
					, [=](auto const& section) { section->set_offset(section->get_offset() - offset_to_subtract); }
				);
			}

			auto element_to_delete = _sections.begin() + section_index;
			_sections.erase(_sections.begin() + section_index);
		}

		Table create_section_table(N_Core::Elf const& elf)
		{
			Table table;
			 
			if (elf._header->get_section_header_number_of_entries() == 0)
			{
				return table;
			}
			auto number_of_entries = elf._header->get_section_header_number_of_entries();

			auto start_of_table = elf._header->get_section_header_offset();
			auto size_of_entry = elf._header->get_section_header_entry_size();

			if (elf.is_memory_mapped())
			{
				for (auto i = 0; i < number_of_entries; i++)
				{
					auto header_of_section_entry = start_of_table + size_of_entry * i;
					auto begin_header = elf.get_memory_mapped_region().begin() + header_of_section_entry;
					auto end_header = begin_header + size_of_entry;

					auto header_range = boost::make_iterator_range(begin_header, end_header);
					auto section = create_section(elf.get_memory_mapped_region(), header_range);
					table.add_section(std::move(section));
				}
			}
			else
			{
				for (auto i = 0; i < number_of_entries; i++)
				{
					table.add_section(create_section(elf._header->get_class() ==  N_Core::N_Header::Class::ELFCLASS64));
				}
			}


			return table;
		}

	}
};