#pragma once
#include "src/include/core/general.h"
#include "src/include/core/header/header_member_types.h"

#include "cow.h"

#include <memory>
#include <type_traits>
#include <ostream>


namespace N_Core
{
	namespace N_Header
	{
		// Error message thrown when parsing an header which does not start with the correct magic bytes.
		const std::string wrong_magic_bytes_message = "Wrong magic bytes in elf header. Did not find /7FELF. Is the file an elf?";

		// @brief Public API of a header.
		// 
		// This class exposes many setters & getters that can be used to change
		// the value of basically all members as described by the ELF standard.
		//
		// 0. Headers are created automatically
		// ================================================================================================
		// Headers are created automatically when loading an elf from disk or creating a custom elf.
		// A free function exists create_header(BinaryBlob) which will create a header for a memory mapped elf
		// which is called when loading an elf from disk. 
		//
		class HeaderA
		{
		public:
			virtual ~HeaderA() {}
			virtual uint8_t get_magic_byte_0() const = 0;
			virtual void set_magic_byte_0(uint8_t byte) =0;
			virtual uint8_t get_magic_byte_1() const =0;
			virtual void set_magic_byte_1(uint8_t byte)=0;
			virtual uint8_t get_magic_byte_2()const =0;
			virtual void set_magic_byte_2(uint8_t byte)=0;
			virtual uint8_t get_magic_byte_3() const =0;
			virtual void set_magic_byte_3(uint8_t byte)=0;
			virtual N_Core::N_Header::Class get_class() const = 0;
			virtual void set_class(N_Core::N_Header::Class) = 0;
			virtual uint8_t get_data() const = 0;
			virtual void set_data(uint8_t) = 0;
			virtual uint8_t get_file_version() const = 0;
			virtual void set_file_version(uint8_t) = 0;
			virtual N_Core::N_Header::OS_ABI get_OS_ABI()const = 0;
			virtual void set_OS_ABI(N_Core::N_Header::OS_ABI) = 0;
			virtual uint8_t get_ABI_version() const = 0;
			virtual void set_ABI_version(uint8_t) = 0;
			virtual uint8_t get_padding_byte_0() const = 0;
			virtual void set_padding_byte_0(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_1() const = 0;
			virtual void set_padding_byte_1(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_2() const = 0;
			virtual void set_padding_byte_2(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_3() const = 0;
			virtual void set_padding_byte_3(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_4()const = 0;
			virtual void set_padding_byte_4(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_5() const = 0;
			virtual void set_padding_byte_5(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_6()const = 0;
			virtual void set_padding_byte_6(uint8_t byte) = 0;
			virtual N_Core::N_Header::Type get_type() const = 0;
			virtual void set_type(N_Core::N_Header::Type) = 0;
			virtual Machine get_machine() const = 0;
			virtual void set_machine(Machine) = 0;
			virtual uint32_t get_version() const = 0;
			virtual void set_version(uint32_t) = 0;
			virtual uint64_t get_entry() const = 0;
			virtual void set_entry(uint64_t) = 0;
			virtual uint64_t get_program_header_offset() const = 0;
			virtual void set_program_header_offset(uint64_t) = 0;
			virtual uint64_t get_section_header_offset() const = 0;
			virtual void set_section_header_offset(uint64_t ) = 0;
			virtual uint32_t get_flags()const = 0;
			virtual void set_flags(uint32_t) = 0;
			virtual uint16_t get_elf_header_size()const = 0;
			virtual void set_elf_header_size(uint16_t) = 0;
			virtual uint16_t get_program_header_entry_size() const = 0;
			virtual void set_program_header_entry_size(uint16_t ) = 0;
			virtual uint16_t get_program_header_number_of_entries() const = 0;
			virtual void set_program_header_number_of_entries(uint16_t) = 0;
			virtual uint16_t get_section_header_entry_size() const = 0;
			virtual void set_section_header_entry_size(uint16_t) = 0;
			virtual uint16_t get_section_header_number_of_entries() const = 0;
			virtual void set_section_header_number_of_entries(uint16_t) = 0;
			virtual uint16_t get_section_index_that_contains_strings() const = 0;
			virtual void set_section_index_that_contains_strings(uint16_t) = 0;
			virtual std::unique_ptr<HeaderA> deep_copy() const& = 0;
			virtual std::unique_ptr<HeaderA> deep_copy() && = 0;
		};

		// @brief Implementation of the header API
		// 
		// This class exposes many setters & getters that can be used to change
		// the value of basically all members as described by the ELF standard.
		//
		// Template parameter 'T' identifies if it is a 32-bit or 64-bit header.
		// Expecting Elf64_Ehdr or Elf32_Ehdr
		//
		template <typename T>
		class Header: public HeaderA
		{
		private:	

			// @brief Checks if first 4 bytes of an elf are set to 0x7F, 'E', 'L' and 'F'
			// 
			// @returns true if magic bytes are correct.
			//
			bool are_magic_bytes_correct() const
			{
				return  get_magic_byte_0() == 0x7F &&
				 get_magic_byte_1() == 'E' &&
				 get_magic_byte_2() == 'L' &&
				 get_magic_byte_3() == 'F';
			}

			// @brief Set the magic bytes to the correct values also set class byte(32-bit or 64-bit elf) correct.
			// 
			void make_compliant()
			{
				
				if (is_64bit_header())
				{
					set_class(Class::ELFCLASS64);
				}
				else
				{
					set_class(Class::ELFCLASS32);
				}
				set_magic_byte_0(0x7F);
				set_magic_byte_1('E');
				set_magic_byte_2('L');
				set_magic_byte_3('F');
			}

		public:		
			MMap::Container<T> _header_content; ///< Memory blob with some map applied to it.

			uint8_t get_magic_byte_0() const override { return get(_header_content, &T::e_magic_byte_0); }
			void set_magic_byte_0(uint8_t byte) override { set(_header_content, &T::e_magic_byte_0, byte); }
			uint8_t get_magic_byte_1() const override { return get(_header_content, &T::e_magic_byte_1); }
			void set_magic_byte_1(uint8_t byte) override {  set(_header_content, &T::e_magic_byte_1, byte); }
			uint8_t get_magic_byte_2() const override { return  get(_header_content, &T::e_magic_byte_2); }
			void set_magic_byte_2(uint8_t byte) override {  set(_header_content, &T::e_magic_byte_2, byte); }
			uint8_t get_magic_byte_3() const override { return  get(_header_content, &T::e_magic_byte_3); }
			void set_magic_byte_3(uint8_t byte) override {  set(_header_content, &T::e_magic_byte_3, byte); }
			N_Core::N_Header::Class get_class() const override { return  get(_header_content, &T::e_class); }
			void set_class(N_Core::N_Header::Class class_to_set) override {  set(_header_content, &T::e_class, class_to_set); }
			uint8_t get_data() const override { return  get(_header_content, &T::e_data); }
			void set_data(uint8_t data) override {  set(_header_content, &T::e_data, data); }
			uint8_t get_file_version() const override { return  get(_header_content, &T::e_file_version); }
			void set_file_version(uint8_t version) override {  set(_header_content, &T::e_file_version, version); }
			N_Core::N_Header::OS_ABI get_OS_ABI() const override { return  get(_header_content, &T::e_OS_ABI); };
			void set_OS_ABI(N_Core::N_Header::OS_ABI ABI) override {  set(_header_content, &T::e_OS_ABI, ABI); }
			uint8_t get_ABI_version() const override { return  get(_header_content, &T::e_ABI_version); };
			void set_ABI_version(uint8_t ABI_version) override {  set(_header_content, &T::e_ABI_version, ABI_version); }

			uint8_t get_padding_byte_0() const override { return   get(_header_content, &T::e_padding_byte_0); };
			void set_padding_byte_0(uint8_t byte) override {  set(_header_content, &T::e_padding_byte_0, byte); };
			uint8_t get_padding_byte_1() const override { return   get(_header_content, &T::e_padding_byte_1); };
			void set_padding_byte_1(uint8_t byte) override {  set(_header_content, &T::e_padding_byte_1, byte); };
			uint8_t get_padding_byte_2() const override { return   get(_header_content, &T::e_padding_byte_2); };
			void set_padding_byte_2(uint8_t byte) override {  set(_header_content, &T::e_padding_byte_2, byte); };
			uint8_t get_padding_byte_3() const override { return   get(_header_content, &T::e_padding_byte_3); };
			void set_padding_byte_3(uint8_t byte) override {  set(_header_content, &T::e_padding_byte_3, byte); };
			uint8_t get_padding_byte_4() const override { return   get(_header_content, &T::e_padding_byte_4); };
			void set_padding_byte_4(uint8_t byte) override {  set(_header_content, &T::e_padding_byte_4, byte); };
			uint8_t get_padding_byte_5() const override { return   get(_header_content, &T::e_padding_byte_5); };
			void set_padding_byte_5(uint8_t byte) override {  set(_header_content, &T::e_padding_byte_5, byte); };
			uint8_t get_padding_byte_6() const override { return   get(_header_content, &T::e_padding_byte_6); };
			void set_padding_byte_6(uint8_t byte) override {  set(_header_content, &T::e_padding_byte_6, byte); };

			N_Core::N_Header::Type get_type() const override { return  get(_header_content, &T::e_type); };
			void set_type(N_Core::N_Header::Type type) override {  set(_header_content, &T::e_type, type); };
			Machine get_machine() const override { return  get(_header_content, &T::e_machine); };
			void set_machine(Machine machine) override {  set(_header_content, &T::e_machine, machine); };
			uint32_t get_version() const override { return  get(_header_content, &T::e_version); };
			void set_version(uint32_t version) override {  set(_header_content, &T::e_version, version); };
			uint64_t get_entry() const override { return  get(_header_content, &T::e_entry); };
			void set_entry(uint64_t entry) override {  set(_header_content, &T::e_entry, entry); };
			uint64_t get_program_header_offset() const override { return  get(_header_content, &T::e_phoff); };
			void set_program_header_offset(uint64_t offset) override {  set(_header_content, &T::e_phoff, offset); };
			uint64_t get_section_header_offset() const override { return  get(_header_content, &T::e_shoff); };
			void set_section_header_offset(uint64_t offset)  override {  set(_header_content, &T::e_shoff, offset); };
			uint32_t get_flags() const override { return  get(_header_content, &T::e_flags); };
			void set_flags(uint32_t flags)  override {  set(_header_content, &T::e_flags, flags); };
			uint16_t get_elf_header_size()const override { return  get(_header_content, &T::e_ehsize); };
			void set_elf_header_size(uint16_t size)  override {  set(_header_content, &T::e_ehsize, size); };
			uint16_t get_program_header_entry_size()const override { return  get(_header_content, &T::e_phentsize); };
			void set_program_header_entry_size(uint16_t size) override {  set(_header_content, &T::e_phentsize, size); };
			uint16_t get_program_header_number_of_entries()const override { return  get(_header_content, &T::e_phnum); };
			void set_program_header_number_of_entries(uint16_t number)  override {  set(_header_content, &T::e_phnum, number); };
			uint16_t get_section_header_entry_size() const override { return  get(_header_content, &T::e_shentsize); };
			void set_section_header_entry_size(uint16_t size)   override {  set(_header_content, &T::e_shentsize, size); };
			uint16_t get_section_header_number_of_entries()const override { return  get(_header_content, &T::e_shnum); };
			void set_section_header_number_of_entries(uint16_t number) override {  set(_header_content, &T::e_shnum, number); };
			uint16_t get_section_index_that_contains_strings()const override { return  get(_header_content, &T::e_shstrndx); };
			void set_section_index_that_contains_strings(uint16_t index)   override {  set(_header_content, &T::e_shstrndx, index); };

			std::unique_ptr<HeaderA> deep_copy() const& override { return std::make_unique<Header<T>>(*this);}
			std::unique_ptr<HeaderA> deep_copy() && override { return std::make_unique<Header<T>>(std::move(*this));}


			// @brief Create new header.
			// 
			// Will update magic bytes and class (see description template param 'T').
			//
			explicit Header():
				_header_content()
			{
				make_compliant();
			}

			// @brief Create a new header from a memory mapped elf.
			// 
			// @throws std::invalid_argument if the magic bytes are not set.
			//
			explicit Header(N_Core::BinaryBlob header_memory_blob) :
				_header_content(header_memory_blob.begin(), header_memory_blob.end())
			{
				if (!are_magic_bytes_correct())
				{
					throw std::invalid_argument(wrong_magic_bytes_message);
				}
			}

			// @brief Based on type deduces if the elf is 64-bit.
			//
			// @returns true if 64-bit header
			//
			constexpr inline bool is_64bit_header()
			{
				return std::is_same_v<T, Elf64_Ehdr>;
			}
		};

		// @brief Dump a header to an ostream.
		// 
		// @param	stream	ostream to dump the header to.
		// @param	header	header to dump.
		//
		void dump(std::ostream& stream, HeaderA const& header);

		// @brief Create header from some elf mapped to memory.
		// 
		// @param memory_mapped_elf	Memory region which contains memory mapped elf.
		//
		// It is assumed that the header of the elf STARTS at byte 0 of the BinaryBlob region.
		//
		// @returns pimpl to parse the header and get/set members.
		//
		std::unique_ptr<N_Header::HeaderA> create_header(BinaryBlob memory_mapped_elf);

		// @brief Create a new 32-bit or 64-bit header.
		// 
		// @param class_to_create_header	Indicates if a 32-bit or 64-bit header has to be created.
		//
		// @returns pimpl to parse the header and get/set members.
		//
		std::unique_ptr<N_Header::HeaderA> create_header(Class class_to_create_header);

		// @brief Create a new header based and use existing header instance as blueprint.
		// 
		// @param existing_header	Blueprint for new header
		//
		// @returns pimpl to parse the header and get/set members.
		//
		template <typename T, std::enable_if_t<std::is_same_v<std::decay_t<T>, std::unique_ptr<N_Header::HeaderA>>, int> = 0>
		std::unique_ptr<N_Header::HeaderA> create_header(T&& existing_header)
		{
			return existing_header->deep_copy();
		}
	}
}