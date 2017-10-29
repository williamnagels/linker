#include "src/include/core/symtab/symbol.h"
#include "src/include/core/strategy_generator.h"
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
			_symbol_parse_strategy = create_strategy_from_size(PossibleGeneratorTypes<SymbolParseStrategy, TSymbolParseStrategy<Elf32_Sym>, TSymbolParseStrategy<Elf64_Sym>>(), _content);
		}

	}
}