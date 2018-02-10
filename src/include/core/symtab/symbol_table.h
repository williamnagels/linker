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
			SymbolTy static create_symbol(BinaryBlob header, BinaryBlob content) { SymbolTy(header, content); }
			using InternalStorageTy = std::vector<SymbolTy>;
			InternalStorageTy _sections; ///< list of sections assigned to this table.

			typename InternalStorageTy::iterator begin() { return _entries.begin(); }
			typename InternalStorageTy::iterator end() { return _entries.end(); }
			typename InternalStorageTy::const_iterator begin() const { return _entries.begin(); }
			typename InternalStorageTy::const_iterator end() const { return _entries.end(); }

			Table(BinaryBlob blob) {}

			friend std::ostream& operator<<(std::ostream& stream, Table const& table)
			{
				return stream;
			}
		private:
			std::vector<SymbolTy> _entries; ///< index in the vector is the 'symbol index' as used in elf.
		};

	}
}
