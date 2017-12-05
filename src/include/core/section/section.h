#pragma once
#include "src/include/core/addressable.h"
#include "src/include/core/general.h"
#include "src/include/core/symtab/symbol_table.h"
#include "src/include/core/section/section_member_types.h"

#include <variant>
#include <functional>

namespace N_Core
{


	namespace N_Section
	{
		template <typename T>
		class Table
		{
		public:
		};

		void create_section_table(N_Core::BinaryBlob blob)
		{

		}

		class HeaderParseStrategy;
		
		/*@brief ELF Section representation.
		*
		* Contains section header and reference to the content of the section.
		*
		*
		*/
		class Section
		{
		public:
			Section(N_Core::BinaryBlob& header, N_Core::BinaryBlob& content); ///< Construct from 2 binary blobs. One for content another one for header.
			~Section();

			std::variant<N_Core::BinaryBlob, N_Core::N_SymTab::SymbolTable> _parsed_content;

			uint64_t get_name();
			Type get_type();
			Flags get_flags();
			uint64_t get_address();
			uint64_t get_offset();
			uint64_t get_size();
			uint64_t get_link();
			uint64_t get_info();
			uint64_t get_address_alignment();
			uint64_t get_entry_size();

			BinaryBlob get_content() { return _content; }
		private:
			BinaryBlob& _header;///< 32 or 64-bit header depending on the elf it is contained in.
			BinaryBlob& _content; ///< 32 or 64-bit header depending on the elf it is contained in.
			std::unique_ptr<HeaderParseStrategy> _header_parse_strategy; ///Will be 64-bit or 32-bit variant.
			N_Core::VirtualAddressChangedSignal _virtual_address_changed_signal; ///< Connect slot to this signal to receive updates about VMA changes.

		
		};
	}

};
