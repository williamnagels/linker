#pragma once
#include "src/include/core/symtab/symbol.h"
#include <vector>
#include <memory>
namespace N_Core
{
	namespace N_SymTab
	{
		class SymbolTable
		{
		private:
			std::vector<std::unique_ptr<SymbolParseStrategy>> _entries; ///< index in the vector is the 'symbol index' as illustrated in elf.
		public:

		};
	}
}
