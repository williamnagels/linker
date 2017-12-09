#include "src/include/core/section/section.h"
#include "src/include/core/symtab/symbol_table.h"
#include "src/include/core/elf.h"
namespace N_Core
{
	namespace N_Section
	{
		
	//@brief dump header to stream
	//
	void dump(std::ostream& stream, Table const& table, N_Core::N_Header::HeaderA const& header)
	{
		auto i = 0;
		for (auto const& section : table._sections)
		{
			stream.seekp(header.get_section_header_offset() + i*header.get_section_header_entry_size());
			auto ptr32 = dynamic_cast<Section<Elf32_Shdr> const*>(section.get());
			auto content_offset = 0;
			if (ptr32)
			{
				dump(stream, ptr32->_content);
				stream.seekp(ptr32->get_offset());
				N_Core::dump(stream, ptr32->_content_blob);
			}
			auto ptr64 = dynamic_cast<Section<Elf64_Shdr> const*>(section.get());
			if (ptr64)
			{

				dump(stream, ptr64->_content);
				stream.seekp(ptr64->get_offset());
				N_Core::dump(stream, ptr64->_content_blob);
			}

			

			i++;
			//throw std::invalid_argument("Cannot deduce section type in table");
		}
	}
		/*
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
			for (auto i = 0; i < number_of_entries; i++)
			{
				auto header_of_section_entry = start_of_table + size_of_entry * i;
				auto begin_header = elf.get_memory_mapped_region().begin() + header_of_section_entry;
				auto end_header = begin_header + size_of_entry;

				auto header_range = boost::make_iterator_range(begin_header, end_header);
				table.add_section(elf.get_memory_mapped_region(), header_range);
			}

			return table;
		}

	}
};