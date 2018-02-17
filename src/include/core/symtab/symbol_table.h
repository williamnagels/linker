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
		template <typename T, typename C>
		class Table
		{
		private:
			std::reference_wrapper<const C> _container;
			OptionalNonOwningMemory _names;
			void build_table(BinaryBlob blob)
			{
				auto number_of_entries = blob.size() / sizeof(T);

				for (auto i = 0; i < number_of_entries; i++)
				{
					uint8_t* begin = blob.begin() + i * sizeof(T);
					uint8_t* end = begin + sizeof(T);

					_symbols.emplace_back(*this, BinaryBlob(begin, end), _names);
				}
			}


		public:


			using SymbolTy = typename Symbol<T, Table>;
			SymbolTy static create_symbol(BinaryBlob header, BinaryBlob content) { SymbolTy(header, content); }
			using InternalStorageTy = std::vector<SymbolTy>;
			InternalStorageTy _symbols; ///< list of sections assigned to this table.

			typename InternalStorageTy::iterator begin() { return _symbols.begin(); }
			typename InternalStorageTy::iterator end() { return _symbols.end(); }
			typename InternalStorageTy::const_iterator begin() const { return _symbols.begin(); }
			typename InternalStorageTy::const_iterator end() const { return _symbols.end(); }

			Table(C const& container, BinaryBlob blob, OptionalNonOwningMemory symbol_names):
				_container(container)
				,_names(symbol_names)
			{
				build_table(blob);
			}
		};
		template <typename T, typename C>
		std::ostream& operator<<(std::ostream& stream, Table<T, C> const& table)
		{
			std::for_each(table.begin(), table.end(), [&](auto const& symbol) {stream << symbol; });
			return stream;
		}
	}
}
