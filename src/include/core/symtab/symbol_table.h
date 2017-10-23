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

			/*@brief Create new symbol table for some section.
			*
			* Will parse content of the section and attempt to create a symboltable from it.
			* @throws std::invalid_argument if section is not of type SYM_TAB.
			*
			*/
			friend SymbolTable create_symbol_table_from_section(N_Section::Section&); 
			
			/*@brief Move an existing symbol into the table.
			*
			* Will move an existing symbol into the symbol table. Adding the symbol to the symbol table.
			* The original symbol is no longer usable but not defunct.
			* 
			*/
			friend void add_symbol_to_existing_symbol_table(SymbolTable& table, Symbol&& symbol); 
		};

	}
}
