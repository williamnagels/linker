#pragma once
#include "src/include/core/symtab/symbol.h"
#include <vector>
#include <memory>
namespace N_Core
{
	namespace N_Section
	{
		class Section;
	}

	namespace N_SymTab
	{
		class SymbolTable
		{
		private:
			std::vector<Symbol> _entries; ///< index in the vector is the 'symbol index' as used in elf.
		public:
			SymbolTable() {}

			friend SymbolTable create_symbol_table_from_section(N_Section::Section&); ///< If section is of type symtab will construct a new symboltable.
			friend void add_symbol_to_existing_symbol_table(SymbolTable& table, Symbol&& symbol); ///< Move some symbol into the table. Original symbol should no longer be used.
		};

	}
}
