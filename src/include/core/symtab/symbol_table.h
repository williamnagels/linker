#pragma once
#include "src/include/core/symtab/symbol.h"
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include "src/include/core/symtab/symbol_member_types.h"


namespace N_Core
{
	namespace N_Symbol
	{
		template <typename T>
		class Table
		{
		private:
			void build_table(BinaryBlob blob)
			{
				auto number_of_entries = blob.size() / sizeof(T);

				for (auto i = 0; i < number_of_entries; i++)
				{
					uint8_t* begin = blob.begin() + i * sizeof(T);
					uint8_t* end = begin + sizeof(T);

					_symbols.emplace_back(BinaryBlob(begin, end));
				}
			}


		public:

			using NameBufferTy = std::optional < std::reference_wrapper<MMap::Container<uint8_t>>;
			NameBufferTy _names;

			using SymbolTy = typename Symbol<T>;
			SymbolTy static create_symbol(BinaryBlob header, BinaryBlob content) { SymbolTy(header, content); }
			using InternalStorageTy = std::vector<SymbolTy>;
			InternalStorageTy _symbols; ///< list of sections assigned to this table.

			typename InternalStorageTy::iterator begin() { return _entries.begin(); }
			typename InternalStorageTy::iterator end() { return _entries.end(); }
			typename InternalStorageTy::const_iterator begin() const { return _entries.begin(); }
			typename InternalStorageTy::const_iterator end() const { return _entries.end(); }

			Table(BinaryBlob blob, MMap::Container<uint8_t> const& linked_section):
				,Table(blob)
				,_names(linked_section)
			{
				build_table(blob);
			}
			Table(BinaryBlob blob) 
			{
				build_table(blob);
			}

	
		private:
			std::vector<SymbolTy> _entries; ///< index in the vector is the 'symbol index' as used in elf.
		};
		template <typename T>
		std::ostream& operator<<(std::ostream& stream, Table<T> const& table)
		{
			std::for_each(table.begin(), table.end(), [&](auto const& symbol) {stream << symbol; });
			return stream;
		}
	}
}
