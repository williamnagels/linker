#pragma once
#include "src/include/core/symtab/symbol.h"
#include "src/include/core/symtab/symbol_member_types.h"
#include <list>
#include <memory>
#include <optional>
#include <functional>



namespace N_Core
{
	namespace N_Symbol
	{
		template <typename T, typename C>
		class Table
		{
		public:

			C const& get_parent()const { return _container; }
			using SymbolTy = Symbol<T, Table>;
			using InternalStorageTy = std::list<SymbolTy>;
			using Iterator = typename InternalStorageTy::iterator;
			using ConstIterator = typename InternalStorageTy::const_iterator;
			InternalStorageTy _symbols; ///< list of sections assigned to this table.

			Iterator begin() { return _symbols.begin(); }
			Iterator end() { return _symbols.end(); }
			ConstIterator begin() const { return _symbols.begin(); }
			ConstIterator end() const { return _symbols.end(); }

			Table(C const& container, BinaryBlob blob):
				_container(container)
			{
				build_table(blob);
			}
			Table(Table const&) = delete;
		    Table(Table&&) = delete;
		private:
			C const& _container;
			void build_table(BinaryBlob blob)
			{
				std::size_t size_of_one_symbol = sizeof(typename SymbolTy::T);
				std::size_t size_of_symbol_table = blob.size();

				for (auto i = 0; i < size_of_symbol_table; i+=size_of_one_symbol)
				{
					uint8_t* begin = blob.begin() + i;
					uint8_t* end = begin + size_of_one_symbol;

					_symbols.emplace_back(*this, BinaryBlob(begin, end));
				}
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
