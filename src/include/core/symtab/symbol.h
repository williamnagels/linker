#pragma once
#include "src/include/core/symtab/symbol_parse_strategy.h"
namespace N_Core
{
	namespace N_SymTab
	{
		class Symbol
		{
		public:
			Symbol();
			~Symbol();
			Symbol(N_Core::BinaryBlob& content); ///< Construct from a binary blob.

			Symbol(Symbol&& rhs);
			Symbol& operator=(Symbol&& rhs);
			Symbol(Symbol const& rhs) = delete; ///< No deep copy of the parse strat exists.
			Symbol& operator=(Symbol const& rhs) = delete;

			
			template <typename T>
			friend Symbol create_new()
			{
				Symbol symbol;
				symbol._symbol_parse_strategy = std::make_unique<T>();
				symbol._symbol_parse_strategy->set_info(0); // Force allocation.
				return symbol;
			}

		private:
			BinaryBlob _header_entry; ///< Memory where symbol is stored.
			std::unique_ptr<N_SymTab::SymbolParseStrategy> _symbol_parse_strategy; ///< Will be 64-bit or 32-bit variant.
		
		};
	}
}

