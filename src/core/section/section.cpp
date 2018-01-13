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
						stream << ptr->_content;
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


	}
};