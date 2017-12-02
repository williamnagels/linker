#pragma once
#include "src/include/core/general.h"
#include "src/include/core/header/header_member_types.h"
#include "src/include/core/read_write_blob.h"

#include <memory>
#include <type_traits>
#include <ostream>


namespace N_Core
{
	namespace N_Header
	{
		const std::string wrong_magic_bytes_message = "Wrong magic bytes in elf header. Did not find /7FELF. Is the file an elf?";


		class HeaderA
		{
			virtual uint8_t get_magic_byte_0() = 0;
			virtual void set_magic_byte_0(uint8_t byte) =0;
			virtual uint8_t get_magic_byte_1() =0;
			virtual void set_magic_byte_1(uint8_t byte)=0;
			virtual uint8_t get_magic_byte_2() =0;
			virtual void set_magic_byte_2(uint8_t byte)=0;
			virtual uint8_t get_magic_byte_3()=0;
			virtual void set_magic_byte_3(uint8_t byte)=0;
			virtual N_Core::N_Header::Class get_class() = 0;
			virtual void set_class(N_Core::N_Header::Class) = 0;
			virtual uint8_t get_data() = 0;
			virtual void set_data(uint8_t) = 0;
			virtual uint8_t get_file_version() = 0;
			virtual void set_file_version(uint8_t) = 0;
			virtual N_Core::N_Header::OS_ABI get_OS_ABI() = 0;
			virtual void set_OS_ABI(N_Core::N_Header::OS_ABI) = 0;
			virtual uint8_t get_ABI_version() = 0;
			virtual void set_ABI_version(uint8_t) = 0;
			virtual uint8_t get_padding_byte_0() = 0;
			virtual void set_padding_byte_0(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_1() = 0;
			virtual void set_padding_byte_1(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_2() = 0;
			virtual void set_padding_byte_2(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_3() = 0;
			virtual void set_padding_byte_3(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_4() = 0;
			virtual void set_padding_byte_4(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_5() = 0;
			virtual void set_padding_byte_5(uint8_t byte) = 0;
			virtual uint8_t get_padding_byte_6() = 0;
			virtual void set_padding_byte_6(uint8_t byte) = 0;
			virtual N_Core::N_Header::Type get_type() = 0;
			virtual void set_type(N_Core::N_Header::Type) = 0;
			virtual Machine get_machine() = 0;
			virtual void set_machine(Machine) = 0;
			virtual uint32_t get_version() = 0;
			virtual void set_version(uint32_t) = 0;
			virtual uint64_t get_entry() = 0;
			virtual void set_entry(uint64_t) = 0;
			virtual uint64_t get_program_header_offset() = 0;
			virtual void set_program_header_offset(uint64_t) = 0;
			virtual uint64_t get_section_header_offset() = 0;
			virtual void set_section_header_offset(uint64_t ) = 0;
			virtual uint32_t get_flags() = 0;
			virtual void set_flags(uint32_t) = 0;
			virtual uint16_t get_elf_header_size() = 0;
			virtual void set_elf_header_size(uint16_t) = 0;
			virtual uint16_t get_program_header_entry_size() = 0;
			virtual void set_program_header_entry_size(uint16_t ) = 0;
			virtual uint16_t get_program_header_number_of_entries() = 0;
			virtual void set_program_header_number_of_entries(uint16_t) = 0;
			virtual uint16_t get_section_header_entry_size() = 0;
			virtual void set_section_header_entry_size(uint16_t) = 0;
			virtual uint16_t get_section_header_number_of_entries() = 0;
			virtual void set_section_header_number_of_entries(uint16_t) = 0;
			virtual uint16_t get_section_index_that_contains_strings() = 0;
			virtual void set_section_index_that_contains_strings(uint16_t) = 0;
		};


		template <typename T>
		class Header: public HeaderA
		{
		private:			
			bool are_magic_bytes_correct()
			{
				return _header_content.get(&T::e_magic_byte_0) == 0x7F &&
				_header_content.get(&T::e_magic_byte_1) == 'E' &&
				_header_content.get(&T::e_magic_byte_2) == 'L' &&
				_header_content.get(&T::e_magic_byte_3) == 'F';
			}

			ReadWriteBlob<T> _header_content; ///< Memory blob with some map applied to it.


		public:		
			uint8_t get_magic_byte_0() override { return _header_content.get(&T::e_magic_byte_0); }
			void set_magic_byte_0(uint8_t byte) override { _header_content.set(&T::e_magic_byte_0, byte); }
			uint8_t get_magic_byte_1() override { return _header_content.get(&T::e_magic_byte_1); }
			void set_magic_byte_1(uint8_t byte) override { _header_content.set(&T::e_magic_byte_1, byte); }
			uint8_t get_magic_byte_2() override { return _header_content.get(&T::e_magic_byte_2); }
			void set_magic_byte_2(uint8_t byte) override { _header_content.set(&T::e_magic_byte_2, byte); }
			uint8_t get_magic_byte_3() override { return _header_content.get(&T::e_magic_byte_3); }
			void set_magic_byte_3(uint8_t byte) override { _header_content.set(&T::e_magic_byte_3, byte); }
			N_Core::N_Header::Class get_class() override { return _header_content.get(&T::e_class); }
			void set_class(N_Core::N_Header::Class class_to_set) override { _header_content.set(&T::e_class, class_to_set); }
			uint8_t get_data()  override { return _header_content.get(&T::e_data); }
			void set_data(uint8_t data) override { _header_content.set(&T::e_data, data); }
			uint8_t get_file_version()  override { return _header_content.get(&T::e_file_version); }
			void set_file_version(uint8_t version) override { _header_content.set(&T::e_file_version, version); }
			N_Core::N_Header::OS_ABI get_OS_ABI() override { return _header_content.get(&T::e_OS_ABI); };
			void set_OS_ABI(N_Core::N_Header::OS_ABI ABI) override { _header_content.set(&T::e_OS_ABI, ABI); }
			uint8_t get_ABI_version() override { return _header_content.get(&T::e_ABI_version); };
			void set_ABI_version(uint8_t ABI_version) override { _header_content.set(&T::e_ABI_version, ABI_version); }

			uint8_t get_padding_byte_0() override { return  _header_content.get(&T::e_padding_byte_0); };
			void set_padding_byte_0(uint8_t byte) override { _header_content.set(&T::e_padding_byte_0, byte); };
			uint8_t get_padding_byte_1() override { return  _header_content.get(&T::e_padding_byte_1); };
			void set_padding_byte_1(uint8_t byte) override { _header_content.set(&T::e_padding_byte_1, byte); };
			uint8_t get_padding_byte_2() override { return  _header_content.get(&T::e_padding_byte_2); };
			void set_padding_byte_2(uint8_t byte) override { _header_content.set(&T::e_padding_byte_2, byte); };
			uint8_t get_padding_byte_3() override { return  _header_content.get(&T::e_padding_byte_3); };
			void set_padding_byte_3(uint8_t byte) override { _header_content.set(&T::e_padding_byte_3, byte); };
			uint8_t get_padding_byte_4() override { return  _header_content.get(&T::e_padding_byte_4); };
			void set_padding_byte_4(uint8_t byte) override { _header_content.set(&T::e_padding_byte_4, byte); };
			uint8_t get_padding_byte_5() override { return  _header_content.get(&T::e_padding_byte_5); };
			void set_padding_byte_5(uint8_t byte) override { _header_content.set(&T::e_padding_byte_5, byte); };
			uint8_t get_padding_byte_6() override { return  _header_content.get(&T::e_padding_byte_6); };
			void set_padding_byte_6(uint8_t byte) override { _header_content.set(&T::e_padding_byte_6, byte); };

			N_Core::N_Header::Type get_type() { return _header_content.get(&T::e_type); };
			void set_type(N_Core::N_Header::Type type) override { _header_content.set(&T::e_type, type); };
			Machine get_machine()override { return _header_content.get(&T::e_machine); };
			void set_machine(Machine machine) override { _header_content.set(&T::e_machine, machine); };
			uint32_t get_version()override { return _header_content.get(&T::e_version); };
			void set_version(uint32_t version) override { _header_content.set(&T::e_version, version); };
			uint64_t get_entry()override { return _header_content.get(&T::e_entry); };
			void set_entry(uint64_t entry) override { _header_content.set(&T::e_entry, entry); };
			uint64_t get_program_header_offset() override { return _header_content.get(&T::e_phoff); };
			void set_program_header_offset(uint64_t offset) override { _header_content.set(&T::e_phoff, offset); };
			uint64_t get_section_header_offset() override { return _header_content.get(&T::e_shoff); };
			void set_section_header_offset(uint64_t offset)  override { _header_content.set(&T::e_shoff, offset); };
			uint32_t get_flags() override { return _header_content.get(&T::e_flags); };
			void set_flags(uint32_t flags)  override { _header_content.set(&T::e_flags, flags); };
			uint16_t get_elf_header_size() override { return _header_content.get(&T::e_ehsize); };
			void set_elf_header_size(uint16_t size)  override { _header_content.set(&T::e_ehsize, size); };
			uint16_t get_program_header_entry_size() override { return _header_content.get(&T::e_phentsize); };
			void set_program_header_entry_size(uint16_t size) override { _header_content.set(&T::e_phentsize, size); };
			uint16_t get_program_header_number_of_entries() override { return _header_content.get(&T::e_phnum); };
			void set_program_header_number_of_entries(uint16_t number)  override { _header_content.set(&T::e_phnum, number); };
			uint16_t get_section_header_entry_size() override { return _header_content.get(&T::e_shentsize); };
			void set_section_header_entry_size(uint16_t size)   override { _header_content.set(&T::e_shentsize, size); };
			uint16_t get_section_header_number_of_entries() override { return _header_content.get(&T::e_shnum); };
			void set_section_header_number_of_entries(uint16_t number) override { _header_content.set(&T::e_shnum, number); };
			uint16_t get_section_index_that_contains_strings() override { return _header_content.get(&T::e_shstrndx); };
			void set_section_index_that_contains_strings(uint16_t index)   override { _header_content.set(&T::e_shstrndx, index); };

			//template <typename = typename std::enable_if_t<std::is_same_v<T, Bit32> || std::is_same_v<T, Bit64>> >
			Header(N_Core::BinaryBlob const& header_memory_blob) :
				_header_content(header_memory_blob)
			{
				if (!are_magic_bytes_correct())
				{
					throw std::invalid_argument(wrong_magic_bytes_message);
				}
			}

			Header(Header const& header) :
				_header_content(header._header_content)
			{

			}
			bool is_64bit_header()
			{
				return get_class() == Class::ELFCLASS64;
			}

			friend void dump(std::ostream& stream, N_Header::Header<T> header);
		};

		//@brief dump header to stream
		//
		//
		template <typename T>
		void dump(std::ostream& stream, N_Header::Header<T> header)
		{
			dump(stream, header._header_content);
		}
	}
}