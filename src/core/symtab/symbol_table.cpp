#include "src/include/core/symtab/symbol_table.h"
#include "src/include/core/section/section.h"
#include "src/include/core/symtab/symbol.h"

namespace N_Core
{
	namespace N_SymTab
	{
		/*
		SymbolTable create_symbol_table_from_section(N_Section::Section& section)
		{

			if (section.get_type() == N_Section::SHT_SYMTAB)
			{
				throw std::invalid_argument("Section is not of type symbtab, cannot create symbol table.");
			}

			SymbolTable symbol_table;

			BinaryBlob binary_blob = section.get_content();
			uint64_t size_of_one_symbol = section.get_entry_size();

			for (auto it = binary_blob.begin(); it != binary_blob.end(); it += size_of_one_symbol)
			{
				auto blob = BinaryBlob(it, it + size_of_one_symbol);

				Symbol symbol(blob);

				add_symbol_to_existing_symbol_table(symbol_table, std::move(symbol));
			}

			return symbol_table;
		}

		void add_symbol_to_existing_symbol_table(SymbolTable& table, Symbol&& symbol)
		{
			table._entries.push_back(std::move(symbol));
		}*/
	}
}