#include "src/include/core/symtab/symbol.h"

namespace N_Core
{
	namespace N_SymTab
	{
		Symbol::Symbol()
		{

		}

		Symbol::~Symbol()
		{

		}

		Symbol::Symbol(Symbol&& rhs)
		{
			_content = rhs._content;
			_symbol_parse_strategy.reset(rhs._symbol_parse_strategy.get());
			rhs._symbol_parse_strategy.reset();
		}

		Symbol& Symbol::operator=(Symbol&& rhs)
		{
			_content = rhs._content;
			_symbol_parse_strategy.reset(rhs._symbol_parse_strategy.get());
			rhs._symbol_parse_strategy.reset();
			return *this;
		}

		Symbol::Symbol(N_Core::BinaryBlob& content) :
			_content(content)
		{
			switch (_content.size())
			{
			case 0x80:
				_symbol_parse_strategy = std::make_unique<TSymbolParseStrategy<Elf32_Sym>>(_content);
				break;
			case 0x70:
				_symbol_parse_strategy = std::make_unique<TSymbolParseStrategy<Elf64_Sym>>(_content);
				break;
			default:
				throw std::invalid_argument("Size of symbol blob is of unexpected size.");
			}
		}

	}
}