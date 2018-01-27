#pragma once
#include "src/include/core/symtab/symbol.h"
#include <vector>
#include <memory>
#include "src/include/core/symtab/symbol_member_types.h"
namespace N_Core
{
	namespace N_Symbol
	{
		template <typename T>
		class Table
		{

		public:
			using SymbolTy = typename Symbol<T>;
			Table() {}

		private:
			std::vector<SymbolTy> _entries; ///< index in the vector is the 'symbol index' as used in elf.
		};

	}
}
