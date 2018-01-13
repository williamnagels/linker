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
	}
};