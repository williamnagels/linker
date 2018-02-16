#pragma once
#include "src/include/core/general.h"
#include "src/include/core/symtab/symbol_member_types.h"
namespace N_Core
{
	namespace N_Symbol
	{
		template <typename V>
		class Symbol
		{
		private:
			using T = std::conditional_t<std::is_same_v<V, Bit64>, Elf64_Sym, Elf32_Sym >;
			MMap::Container<T> _content;
		public:
			
			Symbol(BinaryBlob range):_content(range.begin()) {}

			MMap::Container<T>& get_content() { return _content; }
			MMap::Container<T> const& get_content() const { return _content; }

			uint64_t get_name()const { return  get(_header_entry, &T::st_name); }
			uint64_t get_info()const { return  get(_header_entry, &T::st_info); }
			uint64_t get_other()const { return  get(_header_entry, &T::st_other); }
			uint64_t get_section_index()const { return  get(_header_entry, &T::st_shndx); }
			uint64_t get_value() const { return get(_header_entry, &T::st_value); }
			uint64_t get_size() const { return get(_header_entry, &T::st_size); }
		};

		template <typename T>
		std::ostream& operator<<(std::ostream& stream, Symbol<T> const& symbol)
		{
			stream << symbol.get_content();
			return stream;
		}
	}
}

