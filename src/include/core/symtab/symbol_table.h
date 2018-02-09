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
			MMap::Container<uint8_t> _content;
		public:
			using SymbolTy = typename Symbol<T>;

			MMap::Container<uint8_t> const& get_content() const { return _content; }
			MMap::Container<uint8_t>& get_content() { return _content; }

			Table(BinaryBlob blob):
				_content(blob.begin(), blob.end()){}

		private:
			std::vector<SymbolTy> _entries; ///< index in the vector is the 'symbol index' as used in elf.
		};

	}
}
