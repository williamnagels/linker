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
		// Template parameter 'T' identifies if it is a 32-bit or 64-bit header.
		// Expecting Elf64_Ehdr or Elf32_Ehdr
		//
		template <typename T>
		class Header
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

			uint8_t get_magic_byte_0() const  { return get(_header_content, &T::e_magic_byte_0); }
			void set_magic_byte_0(uint8_t byte)  { set(_header_content, &T::e_magic_byte_0, byte); }
			uint8_t get_magic_byte_1() const  { return get(_header_content, &T::e_magic_byte_1); }
			void set_magic_byte_1(uint8_t byte)  {  set(_header_content, &T::e_magic_byte_1, byte); }
			uint8_t get_magic_byte_2() const  { return  get(_header_content, &T::e_magic_byte_2); }
			void set_magic_byte_2(uint8_t byte)  {  set(_header_content, &T::e_magic_byte_2, byte); }
			uint8_t get_magic_byte_3() const  { return  get(_header_content, &T::e_magic_byte_3); }
			void set_magic_byte_3(uint8_t byte)  {  set(_header_content, &T::e_magic_byte_3, byte); }
			N_Core::N_Header::Class get_class() const  { return  get(_header_content, &T::e_class); }
			void set_class(N_Core::N_Header::Class class_to_set)  {  set(_header_content, &T::e_class, class_to_set); }
			uint8_t get_data() const  { return  get(_header_content, &T::e_data); }
			void set_data(uint8_t data)  {  set(_header_content, &T::e_data, data); }
			uint8_t get_file_version() const  { return  get(_header_content, &T::e_file_version); }
			void set_file_version(uint8_t version)  {  set(_header_content, &T::e_file_version, version); }
			N_Core::N_Header::OS_ABI get_OS_ABI() const  { return  get(_header_content, &T::e_OS_ABI); };
			void set_OS_ABI(N_Core::N_Header::OS_ABI ABI)  {  set(_header_content, &T::e_OS_ABI, ABI); }
			uint8_t get_ABI_version() const  { return  get(_header_content, &T::e_ABI_version); };
			void set_ABI_version(uint8_t ABI_version)  {  set(_header_content, &T::e_ABI_version, ABI_version); }

			uint8_t get_padding_byte_0() const  { return   get(_header_content, &T::e_padding_byte_0); };
			void set_padding_byte_0(uint8_t byte)  {  set(_header_content, &T::e_padding_byte_0, byte); };
			uint8_t get_padding_byte_1() const  { return   get(_header_content, &T::e_padding_byte_1); };
			void set_padding_byte_1(uint8_t byte)  {  set(_header_content, &T::e_padding_byte_1, byte); };
			uint8_t get_padding_byte_2() const  { return   get(_header_content, &T::e_padding_byte_2); };
			void set_padding_byte_2(uint8_t byte)  {  set(_header_content, &T::e_padding_byte_2, byte); };
			uint8_t get_padding_byte_3() const  { return   get(_header_content, &T::e_padding_byte_3); };
			void set_padding_byte_3(uint8_t byte)  {  set(_header_content, &T::e_padding_byte_3, byte); };
			uint8_t get_padding_byte_4() const  { return   get(_header_content, &T::e_padding_byte_4); };
			void set_padding_byte_4(uint8_t byte)  {  set(_header_content, &T::e_padding_byte_4, byte); };
			uint8_t get_padding_byte_5() const  { return   get(_header_content, &T::e_padding_byte_5); };
			void set_padding_byte_5(uint8_t byte)  {  set(_header_content, &T::e_padding_byte_5, byte); };
			uint8_t get_padding_byte_6() const  { return   get(_header_content, &T::e_padding_byte_6); };
			void set_padding_byte_6(uint8_t byte)  {  set(_header_content, &T::e_padding_byte_6, byte); };

			N_Core::N_Header::Type get_type() const  { return  get(_header_content, &T::e_type); };
			void set_type(N_Core::N_Header::Type type)  {  set(_header_content, &T::e_type, type); };
			Machine get_machine() const  { return  get(_header_content, &T::e_machine); };
			void set_machine(Machine machine)  {  set(_header_content, &T::e_machine, machine); };
			uint32_t get_version() const  { return  get(_header_content, &T::e_version); };
			void set_version(uint32_t version)  {  set(_header_content, &T::e_version, version); };
			uint64_t get_entry() const  { return  get(_header_content, &T::e_entry); };
			void set_entry(uint64_t entry)  {  set(_header_content, &T::e_entry, entry); };
			uint64_t get_program_header_offset() const  { return  get(_header_content, &T::e_phoff); };
			void set_program_header_offset(uint64_t offset)  {  set(_header_content, &T::e_phoff, offset); };
			uint64_t get_section_header_offset() const  { return  get(_header_content, &T::e_shoff); };
			void set_section_header_offset(uint64_t offset)   {  set(_header_content, &T::e_shoff, offset); };
			uint32_t get_flags() const  { return  get(_header_content, &T::e_flags); };
			void set_flags(uint32_t flags)   {  set(_header_content, &T::e_flags, flags); };
			uint16_t get_elf_header_size()const  { return  get(_header_content, &T::e_ehsize); };
			void set_elf_header_size(uint16_t size)   {  set(_header_content, &T::e_ehsize, size); };
			uint16_t get_program_header_entry_size()const  { return  get(_header_content, &T::e_phentsize); };
			void set_program_header_entry_size(uint16_t size)  {  set(_header_content, &T::e_phentsize, size); };
			uint16_t get_program_header_number_of_entries()const  { return  get(_header_content, &T::e_phnum); };
			void set_program_header_number_of_entries(uint16_t number)   {  set(_header_content, &T::e_phnum, number); };
			uint16_t get_section_header_entry_size() const  { return  get(_header_content, &T::e_shentsize); };
			void set_section_header_entry_size(uint16_t size)    {  set(_header_content, &T::e_shentsize, size); };
			uint16_t get_section_header_number_of_entries()const  { return  get(_header_content, &T::e_shnum); };
			void set_section_header_number_of_entries(uint16_t number)  {  set(_header_content, &T::e_shnum, number); };
			uint16_t get_section_index_that_contains_strings()const  { return  get(_header_content, &T::e_shstrndx); };
			void set_section_index_that_contains_strings(uint16_t index)    {  set(_header_content, &T::e_shstrndx, index); };

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
				_header_content(header_memory_blob.begin())
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
		template <typename T>
		void dump(std::ostream& stream, Header<T> const& header)
		{
			stream << header._header_content;
		}
	}
}