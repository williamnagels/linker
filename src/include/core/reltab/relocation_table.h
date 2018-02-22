#pragma once
#include "src/include/core/reltab/relocation.h"
#include "src/include/core/reltab/relocation_member_types.h"
#include <vector>
#include <memory>
#include <optional>
#include <functional>



namespace N_Core
{
	namespace N_Relocation
	{
		template <typename T, typename C, typename A>
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

					_relocations.emplace_back(*this, BinaryBlob(begin, end));
				}
			}


		public:

			C const& get_parent()const { return _container; }
			using RelocationTy = typename Relocation<T, Table, A>;
			using InternalStorageTy = std::list<RelocationTy>;
			using Iterator = typename InternalStorageTy::iterator;
			using ConstIterator = typename InternalStorageTy::const_iterator;
			InternalStorageTy _relocations; ///< list of sections assigned to this table.

			Iterator begin() { return _relocations.begin(); }
			Iterator end() { return _relocations.end(); }
			ConstIterator begin() const { return _relocations.begin(); }
			ConstIterator end() const { return _relocations.end(); }

			Table(C const& container, BinaryBlob blob) :
				_container(container)
			{
				build_table(blob);
			}
			Table(Table const&) = delete;
			Table(Table&&) = delete;
		};
		template <typename T, typename C, typename A>
		std::ostream& operator<<(std::ostream& stream, Table<T, C, A> const& table)
		{
			std::for_each(table.begin(), table.end(), [&](auto const& relocation) {stream << relocation; });
			return stream;
		}
	}
}
