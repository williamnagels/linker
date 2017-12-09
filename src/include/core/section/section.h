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
	namespace N_Header { class HeaderA; }
	namespace N_Section
	{
		class ASection
		{
		public:
			virtual ~ASection() {}
			virtual BinaryBlob get_content() const = 0;
			virtual uint64_t get_name() const = 0;
			virtual Type get_type() const = 0;
			virtual Flags get_flags() const = 0;
			virtual uint64_t get_address()const = 0;
			virtual uint64_t get_offset()const = 0;
			virtual uint64_t get_size()const = 0;
			virtual uint64_t get_link() const = 0;
			virtual uint64_t get_info()const = 0;
			virtual uint64_t get_address_alignment()const = 0;
			virtual uint64_t get_entry_size()const = 0;

			virtual std::unique_ptr<ASection> deep_copy() const& = 0;
			virtual std::unique_ptr<ASection> deep_copy() && = 0;
		};

		std::unique_ptr<ASection> create_section(N_Core::BinaryBlob content_blob, N_Core::BinaryBlob header_blob);
		class Table
		{

		public:

			ASection const& get_section(uint16_t index) { return *_sections.at(index); }
			void add_section(N_Core::BinaryBlob elf_blob, N_Core::BinaryBlob header_blob) 
			{
				_sections.push_back(create_section(elf_blob, header_blob));
			}
			std::vector<std::unique_ptr<ASection>> _sections;
			
			//template <typename T>
			Table(Table&& other_table)
			{
				_sections = std::move(other_table)._sections;
			}

			Table(Table const& other_table)
			{

				for (auto const& section : other_table._sections)
				{
					_sections.emplace_back(section->deep_copy());
				}
			}
			explicit Table() {}
		};
		


		void dump(std::ostream& stream, Table const& table, N_Core::N_Header::HeaderA const&);
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
		public:
			ReadWriteBlob<T> _content; ///< Memory blob with some map applied to it.
			BinaryBlob _header_blob;///< 32 or 64-bit header depending on the elf it is contained in.
			BinaryBlob _content_blob; ///< 32 or 64-bit header depending on the elf it is contained in.

			BinaryBlob get_content_from_header(N_Core::BinaryBlob content)
			{
				auto a = get_offset();
				auto b = get_size();
				return BinaryBlob(reinterpret_cast<uint8_t*>(content.begin()+get_offset()), reinterpret_cast<uint8_t*>(content.begin()+get_offset() + get_size()));
			}

			template <typename T>
			explicit Section(T&& section, std::enable_if_t<std::is_same_v<std::decay_t<T>, Section>, int> = 0) :
				_content(std::forward<T>(section)._content)
				,_header_blob(section._header_blob)
				,_content_blob(section._content_blob)
			{
			}

			explicit Section(N_Core::BinaryBlob header, N_Core::BinaryBlob elf_blob) :
				_content(header)
				,_header_blob(header)
				,_content_blob(get_content_from_header(elf_blob))
			{
			}

			uint64_t Section::get_name()const override { return _content.get(&T::sh_name); }
			Type Section::get_type() const override { return _content.get(&T::sh_type); }
			Flags Section::get_flags()const override { return static_cast<Flags>(_content.get(&T::sh_flags)); }
			uint64_t Section::get_address()const override { return _content.get(&T::sh_addr); }
			uint64_t Section::get_offset()const override { return _content.get(&T::sh_offset); }
			uint64_t Section::get_size()const override { return _content.get(&T::sh_size); }
			uint64_t Section::get_link()const override { return _content.get(&T::sh_link); }
			uint64_t Section::get_info()const override { return _content.get(&T::sh_info); }
			uint64_t Section::get_address_alignment()const override { return _content.get(&T::sh_addralign); }
			uint64_t Section::get_entry_size()const override { return _content.get(&T::sh_entsize); }
			BinaryBlob get_content() const override { return _content_blob; }
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
