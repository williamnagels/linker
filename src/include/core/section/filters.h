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

			struct RelocationTable
			{
				IndexList _indices;
				RelocationTable(IndexList indices):_indices(indices){}

				template <typename SectionTy>
				bool operator()(SectionTy const& section)
				{
					if (section.get_type() == N_Section::Type::SHT_REL || section.get_type() == N_Section::Type::SHT_RELA)
					{

						return std::find(_indices.begin(), _indices.end(), _index) != _indices.end();
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