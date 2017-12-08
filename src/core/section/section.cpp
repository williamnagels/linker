#include "src/include/core/section/section.h"
#include "src/include/core/symtab/symbol_table.h"
#include "src/include/core/elf.h"
namespace N_Core
{
	namespace N_Section
	{/*
		Section::Section(N_Core::BinaryBlob& header, N_Core::BinaryBlob& content):
			_header(header),
			_content(content)
		{
			
			switch (_header.size())
			{
			case 0x80:
				_header_parse_strategy = std::make_unique<THeaderParseStrategy<Elf64_Shdr>>(header);
				break;
			case 0x70:
				_header_parse_strategy = std::make_unique<THeaderParseStrategy<Elf32_Shdr>>(header);
				break;
			default:
				throw std::invalid_argument("Size of header blob is of unexpected size.");
			}

			switch (_header_parse_strategy->get_type())
			{
			case N_Section::SHT_SYMTAB:
				_parsed_content = N_SymTab::create_symbol_table_from_section(*this);
				break;
			
			case N_Section::SHT_RELA:
				break;

			case N_Section::SHT_PROGBITS:
				_parsed_content = header;
				break;
			default:
				throw std::invalid_argument("Size of header blob is of unexpected size.");
			}
		}*/


		std::unique_ptr<ASection> create_section(N_Core::BinaryBlob content_blob, N_Core::BinaryBlob header_blob)
		{
			switch (header_blob.size())
			{
			case sizeof(Elf32_Shdr) :
				return std::make_unique<Section<Elf32_Shdr>>(header_blob, content_blob);
			case sizeof(Elf64_Shdr) :
				return std::make_unique<Section<Elf32_Shdr>>(header_blob, content_blob);
			default:
				throw std::invalid_argument("Blob is of unexpected size");
			}

		}
		Table create_section_table(N_Core::Elf const& elf)
		{
			Table table;
			auto number_of_entries = elf._header->get_section_header_number_of_entries();

			auto offset = 0;
			auto start_of_table = elf._header->get_section_header_offset();
			auto size_of_entry = elf._header->get_section_header_entry_size();
			for (auto i = 0; i < number_of_entries; i++)
			{
				auto header_of_section_entry = start_of_table + size_of_entry * i;
				auto begin = reinterpret_cast<uint8_t*>(elf._region->get_address()) + header_of_section_entry;
				auto end = begin + size_of_entry;
				table.add_section(boost::make_iterator_range(begin, end));
			}

			return table;
		}

	}
};