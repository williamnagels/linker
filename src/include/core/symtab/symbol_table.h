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
		class Table;

		template <typename T, bool IsConst>
		class Iterator
		{
			uint16_t _current_index;
			Table<T>& _table;

		public:
			Iterator(uint16_t index) :_current_index(index) {}
			Iterator(Iterator const& o) :
				_current_index(o._current_index),
				_table(o._table) {};

			Iterator<T, IsConst>& operator++()
			{
				_current_index++;
				return *this;
			}
			Iterator<T, IsConst> operator++(int)
			{
				auto current_iterator = *this;
				++*this;
				return current_iterator;
			}

			using ValueType =
				std::conditional_t<
				IsConst
				, std::add_const_t<Symbol<T>>
				, Symbol<T>>;
			ValueType operator*()
			{
				auto begin_it = _table.get_content().begin();
				std::advance(begin_it, index);
				auto end_it = begin_it;
				std::advance(end_it, sizeof(T));
				return ValueType(begin_it, end_it);
			}
		};

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

			Iterator<T, true> begin() const
			{
				return Iterator<T, true>(0, *this);
			}
			Iterator<T, true> end() const 
			{
				return Iterator<T, true>(get_content().get_size(), *this);
			}
		};

	}
}
