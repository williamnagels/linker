#pragma once
#include "src/include/core/addressable.h"
#include "src/include/core/general.h"
#include "src/include/core/symtab/symbol_table.h"
#include "src/include/core/section/section_member_types.h"

#include <variant>
#include <functional>

namespace N_Core
{
	class Elf;

	namespace N_Section
	{
		class ASection
		{
		public:
			virtual ~ASection() {}
			virtual uint64_t get_name() = 0;
			virtual Type get_type() = 0;
			virtual Flags get_flags() = 0;
			virtual uint64_t get_address() = 0;
			virtual uint64_t get_offset() = 0;
			virtual uint64_t get_size() = 0;
			virtual uint64_t get_link() = 0;
			virtual uint64_t get_info() = 0;
			virtual uint64_t get_address_alignment() = 0;
			virtual uint64_t get_entry_size() = 0;

			virtual std::unique_ptr<ASection> deep_copy() const& = 0;
			virtual std::unique_ptr<ASection> deep_copy() && = 0;
		};

		
		class Table
		{

		public:
			ASection const& get_section(uint16_t index) { return *_sections.at(index); }
			void add_section(N_Core::BinaryBlob blob) {}
			std::vector<std::unique_ptr<ASection>> _sections;
			
			//template <typename T>
			Table(Table&& other_table)
			{
				_sections = std::move(other_table)._sections;
			}

			Table(Table const& other_table)
			{
				auto a = 0;
			}
			explicit Table() {}
		};
		

		std::unique_ptr<ASection> create_section(N_Core::BinaryBlob header_blob, N_Core::BinaryBlob content_blob);
		Table create_section_table(N_Core::Elf const& elf);


		/*@brief ELF Section representation.
		*
		* Contains section header and reference to the content of the section.
		*
		*
		*/
		template <typename T>
		class Section: public ASection
		{
		private:
			BinaryBlob _header_blob;///< 32 or 64-bit header depending on the elf it is contained in.
			BinaryBlob _content_blob; ///< 32 or 64-bit header depending on the elf it is contained in.

		public:
			ReadWriteBlob<T> _content; ///< Memory blob with some map applied to it.

			Section(Section const& v) {}

			explicit Section(N_Core::BinaryBlob header, N_Core::BinaryBlob content) :
				_header_blob(header),
				_content_blob(content)
			{
			}


			std::variant<N_Core::BinaryBlob, N_Core::N_SymTab::SymbolTable> _parsed_content;

			uint64_t Section::get_name() { return _content.get(&T::sh_name); }
			Type Section::get_type() { return _content.get(&T::sh_type); }
			Flags Section::get_flags() { return static_cast<Flags>(_content.get(&T::sh_flags)); }
			uint64_t Section::get_address() { return _content.get(&T::sh_addr); }
			uint64_t Section::get_offset() { return _content.get(&T::sh_offset); }
			uint64_t Section::get_size() { return _content.get(&T::sh_size); }
			uint64_t Section::get_link() { return _content.get(&T::sh_link); }
			uint64_t Section::get_info() { return _content.get(&T::sh_info); }
			uint64_t Section::get_address_alignment() { return _content.get(&T::sh_addralign); }
			uint64_t Section::get_entry_size() { return _content.get(&T::sh_entsize); }

			BinaryBlob get_content() { return _content; }
			std::unique_ptr<ASection> deep_copy() const& override
			{
				return std::make_unique<Section>(*this);
			}
			std::unique_ptr<ASection> deep_copy() && override
			{
				return std::make_unique<Section>(std::move(*this));
			}


			//N_Core::VirtualAddressChangedSignal _virtual_address_changed_signal; ///< Connect slot to this signal to receive updates about VMA changes.

		
		};
	}

};
