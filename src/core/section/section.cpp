#include "src/include/core/section/section.h"
#include "src/include/core/section/header_parse_strategy.h"
#include "src/include/core/symtab/symbol_table.h"
namespace N_Core
{
	namespace N_Section
	{
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
		}

		Section::~Section()
		{

		}

		uint64_t Section::get_name() { return _header_parse_strategy->get_name(); }
		Type Section::get_type() { return _header_parse_strategy->get_type(); }
		Flags Section::get_flags() { return _header_parse_strategy->get_flags(); }
		uint64_t Section::get_address() { return _header_parse_strategy->get_address(); }
		uint64_t Section::get_offset() { return _header_parse_strategy->get_offset(); }
		uint64_t Section::get_size() { return _header_parse_strategy->get_size(); }
		uint64_t Section::get_link() { return _header_parse_strategy->get_link(); }
		uint64_t Section::get_info() { return _header_parse_strategy->get_info(); }
		uint64_t Section::get_address_alignment() { return _header_parse_strategy->get_address_alignment(); }
		uint64_t Section::get_entry_size() { return _header_parse_strategy->get_entry_size(); }
	}
};