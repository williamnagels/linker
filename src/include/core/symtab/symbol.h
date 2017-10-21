#pragma once
#include "src/include/core/symtab/symbol_parse_strategy.h"
namespace N_Core
{
	namespace N_SymTab
	{
		class Symbol
		{
		public:
			Symbol() {}
			Symbol(Symbol&& rhs)
			{
				_content = rhs._content;
				_symbol_parse_strategy.reset();
				_symbol_parse_strategy.swap(rhs._symbol_parse_strategy);
			}

			Symbol(N_Core::BinaryBlob& content); ///< Construct from 2 binary blobs. 
			~Symbol();

		private:
			BinaryBlob _content; ///< Memory where symbol is stored.
			std::unique_ptr<N_SymTab::SymbolParseStrategy> _symbol_parse_strategy; ///< Will be 64-bit or 32-bit variant.
		};
	}
}

