#pragma once
#include "src/include/core/general.h"
#include "src/include/core/symtab/symbol_member_types.h"
namespace N_Core
{
	namespace N_Symbol
	{
		template <typename V, typename C>
		class Symbol
		{
		private:
			C const& _container;
			using T = std::conditional_t<std::is_same_v<V, Bit64>, Elf64_Sym, Elf32_Sym >;
			MMap::Container<T> _content;
		public:
			Symbol(C const& container, BinaryBlob range):_container(container),_content(range.begin()){}
			Symbol(Symbol&&) = delete;
			Symbol(Symbol const&) = delete;
			MMap::Container<T>& get_content() { return _content; }
			MMap::Container<T> const& get_content() const { return _content; }

			uint64_t get_name()const { return  get(_content, &T::st_name); }
			uint64_t get_info()const { return  get(_content, &T::st_info); }
			uint64_t get_other()const { return  get(_content, &T::st_other); }
			uint64_t get_section_index()const { return  get(_content, &T::st_shndx); }
			uint64_t get_value() const { return get(_content, &T::st_value); }
			uint64_t get_size() const { return get(_content, &T::st_size); }
			C const& get_parent()const { return _container; }
			std::optional<std::string> get_name_as_string() const 
			{ 
				Index index = get_parent().get_parent().get_link();

				if (index)
				{
					auto const& linked_content = get_parent().get_parent().get_parent()[index];
					
					uint8_t const* base = &(*std::get<0>(linked_content.get_interpreted_content()).begin());
					return std::string(reinterpret_cast<char const*>(base+get_name()));
				}
				
				return std::optional<std::string>();
			}
		};

		template <typename T, typename C>
		std::ostream& operator<<(std::ostream& stream, Symbol<T, C> const& symbol)
		{
			stream << symbol.get_content();
			return stream;
		}
	}
}

