#include "src/include/core/symtab/symbol.h"
#include "src/include/core/pimpl_deducer.h"
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
			_header_entry = rhs._header_entry;
		}

		Symbol& Symbol::operator=(Symbol&& rhs)
		{
			_header_entry = rhs._header_entry;
			return *this;
		}

		Symbol::Symbol(N_Core::BinaryBlob& content) :
			_header_entry(content)
		{
		//	_symbol_parse_strategy = pimpl_deducer(PossibleGeneratorTypes<SymbolParseStrategy, TSymbolParseStrategy<Elf32_Sym>, TSymbolParseStrategy<Elf64_Sym>>(), _header_entry);
		}

	}
}