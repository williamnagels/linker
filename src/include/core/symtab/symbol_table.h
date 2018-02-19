#pragma once
#include "src/include/core/symtab/symbol.h"
#include "src/include/core/symtab/symbol_member_types.h"
#include <vector>
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
		private:
			C const& _container;
			void build_table(BinaryBlob blob)
			{
				auto number_of_entries = blob.size() / sizeof(T);

				for (auto i = 0; i < number_of_entries; i++)
				{
					uint8_t* begin = blob.begin() + i * sizeof(T);
					uint8_t* end = begin + sizeof(T);

					_symbols.emplace_back(*this, BinaryBlob(begin, end));
				}
			}


		public:

			C const& get_parent()const { return _container; }
			using SymbolTy = typename Symbol<T, Table>;
			SymbolTy static create_symbol(BinaryBlob header, BinaryBlob content) { SymbolTy(header, content); }
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
		};
		template <typename T, typename C>
		std::ostream& operator<<(std::ostream& stream, Table<T, C> const& table)
		{
			std::for_each(table.begin(), table.end(), [&](auto const& symbol) {stream << symbol; });
			return stream;
		}
	}
}
