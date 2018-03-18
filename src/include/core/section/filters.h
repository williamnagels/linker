#pragma once
#include "src/include/core/section/section.h"

namespace N_Core
{
	namespace N_Section
	{
		namespace N_Filters
		{
			// Functor to filter out all sections that contain a relocation table
			// if index is not the wildcard; all relocations will be returned. else
			// Only the relocation tables that apply to the section with specified index
			// will be returned.
			template <typename ElfTy>
			struct RelocationTable
			{
				Index _index;
				RelocationTable(Index index):_index(index){}

				bool operator()(typename ElfTy::SectionTy const& section)
				{
					if (section.get_type() == N_Section::Type::SHT_REL || section.get_type() == N_Section::Type::SHT_RELA)
					{
						if (is_wildcard(_index))
						{
							return true;
						}	

						return section.get_info() == _index;
					}

					return false;
				};
			};
			
			// Returns all sections that contain a symbol table.
			//template <typename ElfTy>
			struct SymbolTable
			{
				SymbolTable(){}

				template <typename SectionTy>
				bool operator()(SectionTy const& section)
				{
					return (section.get_type() == N_Section::Type::SHT_SYMTAB);
				};
			};			
		}
	}
}