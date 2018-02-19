#pragma once
#include "src/include/core/general.h"
#include "src/include/core/section/section_member_types.h"
#include "src/include/core/symtab/symbol_table.h"
#include <variant>
#include <optional>
#include <functional>
namespace N_Core
{
	namespace N_Section
	{
		/*@brief ELF Section representation.
		*
		* Contains section header and reference to the content of the section.
		*
		*/
		template <typename V, typename C>
		class Section
		{
		public: 
			using T = std::conditional_t<std::is_same_v<V, Bit64>, N_Section::Elf64_Shdr, N_Section::Elf32_Shdr >;
			using SymbolTableTy = N_Symbol::Table< std::conditional_t< std::is_same_v<V, Bit64>, N_Symbol::Elf64_Sym, N_Symbol::Elf32_Sym>, Section>;
			using InterpretedContentTy = std::variant<MMap::Container<uint8_t>, SymbolTableTy>;
			using ContainerTy = C;
		private:
			void create_interpreted_content(InterpretedContentTy &elem, BinaryBlob elf_blob)
			{
				auto content_blob = get_content_from_header(elf_blob);
				switch (get_type())
				{
				case N_Section::Type::SHT_SYMTAB:
					elem.emplace<SymbolTableTy>(*this, content_blob);
					break;
				default:
					elem.emplace<MMap::Container<uint8_t>>(content_blob.begin(), content_blob.end());
				}
			}
			InterpretedContentTy create_interpreted_content(BinaryBlob elf_blob)
			{
				auto content_blob = get_content_from_header(elf_blob);
				switch (get_type())
				{
				case N_Section::Type::SHT_SYMTAB:
					return SymbolTableTy(*this, content_blob);					
				default:
					return MMap::Container<uint8_t>(content_blob.begin(), content_blob.end());
				}
			}
			// @brief Get address range where the content of the section is stored.
			// 
			// Helper function used when constructing a section from memory mapped elf.
			//
			// @param elf_blob	Full elf memory region to which the section belongs.
			//
			// @returns memory region where the section content is stored.
			//
			BinaryBlob get_content_from_header(N_Core::BinaryBlob elf_blob)
			{
				return BinaryBlob(reinterpret_cast<uint8_t*>(elf_blob.begin() + get_offset()), reinterpret_cast<uint8_t*>(elf_blob.begin() + get_offset() + get_size()));
			}
			ContainerTy const& _container;
			MMap::Container<T> _header_entry;
			InterpretedContentTy _interpreted_content;

		public:
			// @brief Create a section for a memory mapped elf.
			// 
			// @param header	address range where the header entry of this section is loaded into memory.
			// @param elf_blob	full address range of the elf.
			//
			// The full memory range of the elf is required because the content may be stored anywhere
			// in the address range and is unknown until the header has been parsed.
			//
			explicit Section(ContainerTy const& container, N_Core::BinaryBlob header, N_Core::BinaryBlob elf_blob) :
				_container(container)
				,_header_entry(header.begin())
				, _interpreted_content()
			{
				create_interpreted_content(_interpreted_content, elf_blob);
			}
			Section(Section const&) = delete;
			Section(Section&&) = delete;

			uint64_t get_name()const  { return  get(_header_entry, &T::sh_name); }
			void set_name(uint64_t offset) { set(_header_entry, &T::sh_name, offset); };
			Type get_type() const  { return  get(_header_entry, &T::sh_type); }
			Flags get_flags()const { auto i = get(_header_entry, &T::sh_flags); return *reinterpret_cast<Flags*>(&i); }
			uint64_t get_address()const  { return  get(_header_entry, &T::sh_addr); }
			uint64_t get_offset()const  { return  get(_header_entry, &T::sh_offset); }
			void set_offset(uint64_t offset)  { return  set(_header_entry, &T::sh_offset, offset); };
			uint64_t get_size() const  { return  get(_header_entry, &T::sh_size); }
			void set_size(uint64_t size) { return  set(_header_entry, &T::sh_size, size); }
			uint64_t get_link()const  { return  get(_header_entry, &T::sh_link); }
			uint64_t get_info()const  { return  get(_header_entry, &T::sh_info); }
			uint64_t get_address_alignment()const  { return  get(_header_entry, &T::sh_addralign); }
			uint64_t get_entry_size()const  { return  get(_header_entry, &T::sh_entsize); }
			uint64_t get_size_in_file() const  { return (get_type() != SHT_NOBITS) ? get_size() : 0; }
			InterpretedContentTy const& get_interpreted_content() const { return _interpreted_content; }
			InterpretedContentTy & get_interpreted_content() { return _interpreted_content; }
			C const& get_parent()const { return _container; }
			template <typename T, typename C>
			friend std::ostream& operator<<(std::ostream& stream, Section<T, C> const& section);
		};


		namespace
		{
			template <typename T, typename C>
			struct ostream_visitor
			{
				std::ostream& _stream;
				ostream_visitor(std::ostream& stream) :_stream(stream) {}
				void operator()(MMap::Container<uint8_t> const& content) const { _stream << content; }
				void operator()(typename Section<T, C>::SymbolTableTy const& symbol_table) const { _stream << symbol_table;}
			};
		}

		// @brief Dump section to stream.
		// 
		// @param stream	stream to which to dump the section.
		// @param section	Section to dump.
		//
		// This expects the current position in the ostream to be
		// where the section header entry starts in the file (offset).
		//
		// Caller can make no guess on what position the stream is
		// at, when this function returns.
		//
		// Primarly used to save a section to a file.
		//
		template <typename T, typename C>
		std::ostream& operator<<(std::ostream& stream, Section<T, C> const& section)
		{
			stream << section._header_entry;
			if (section.get_size_in_file())
			{
				stream.seekp(section.get_offset());

				ostream_visitor<T, C> visitor(stream);
				std::visit(visitor, section._interpreted_content);
			}

			return stream;
		}
	}

};
