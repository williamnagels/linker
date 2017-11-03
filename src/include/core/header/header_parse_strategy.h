#pragma once
#include "src/include/core/header/header_parse_strategy.h"
#include "src/include/core/header/header_member_types.h"
#include "src/include/core/read_write_blob.h"
#include <memory>
#include <stdint.h>
namespace N_Core
{
	namespace N_Header
	{
		struct Elf32_Ehdr
		{
			uint8_t			e_magic_byte_0;
			uint8_t			e_magic_byte_1;
			uint8_t			e_magic_byte_2;
			uint8_t			e_magic_byte_3;
			uint8_t			e_class;
			uint8_t			e_data;
			uint8_t			e_file_version;
			uint8_t			e_OS_ABI;
			uint8_t			e_ABI_version;
			uint8_t			e_padding[7];
			uint16_t        e_type;
			uint16_t        e_machine;
			uint32_t        e_version;
			uint32_t        e_entry;
			uint32_t        e_phoff;
			uint32_t        e_shoff;
			uint32_t        e_flags;
			uint16_t        e_ehsize;
			uint16_t        e_phentsize;
			uint16_t        e_phnum;
			uint16_t        e_shentsize;
			uint16_t        e_shnum;
			uint16_t        e_shstrndx;
		};

		struct Elf64_Ehdr 
		{
			uint8_t			e_magic_byte_0;
			uint8_t			e_magic_byte_1;
			uint8_t			e_magic_byte_2;
			uint8_t			e_magic_byte_3;
			uint8_t			e_class;
			uint8_t			e_data;
			uint8_t			e_file_version;
			uint8_t			e_OS_ABI;
			uint8_t			e_ABI_version;
			uint8_t			e_padding[7];
			uint16_t		e_type;
			uint16_t        e_machine;
			uint32_t        e_version;
			uint64_t        e_entry;
			uint64_t        e_phoff;
			uint64_t        e_shoff;
			uint32_t        e_flags;
			uint16_t        e_ehsize;
			uint16_t        e_phentsize;
			uint16_t        e_phnum;
			uint16_t        e_shentsize;
			uint16_t        e_shnum;
			uint16_t        e_shstrndx;
		};
		class HeaderParseStrategy
		{
		public:
			virtual ~HeaderParseStrategy() { }
			virtual Type get_type() = 0;
			virtual void set_type(Type type) = 0;
			virtual Class get_class() = 0;
			virtual void set_class(Class class_to_set) = 0;
			virtual Machine get_machine() = 0;
			virtual void set_machine(Machine machine) = 0;
			virtual Version get_version() = 0;
			virtual void set_version(Version version) = 0;
			virtual OS_ABI get_OS_ABI() = 0;
			virtual void set_OS_ABI(OS_ABI ABI) = 0;
			virtual uint64_t get_ABI_version() = 0;
			virtual void set_ABI_version(uint64_t ABI_version) = 0;

			virtual uint64_t get_entry() = 0;
			virtual void set_entry(uint64_t entry) = 0;
			virtual uint64_t get_program_header_offset() = 0;
			virtual void set_program_header_offset(uint64_t program_header_offset) = 0;
			virtual uint64_t get_section_header_offset() = 0;
			virtual void set_section_header_offset(uint64_t section_header_offset) = 0;
			virtual uint64_t get_flags() = 0;
			virtual void set_flags(uint64_t flags) = 0;
			virtual uint64_t get_header_size() = 0;
			virtual void set_header_size(uint64_t header_size) = 0;
			virtual uint64_t get_size_of_program_header_table() = 0;
			virtual void set_size_of_program_header_table(uint64_t size) = 0;
			virtual uint64_t get_size_of_section_header_table() = 0;
			virtual void set_size_of_section_header_table(uint64_t size) = 0;

			virtual uint64_t get_number_of_entries_in_program_header_table() = 0;
			virtual void set_number_of_entries_in_program_header_table(uint64_t) = 0;

			virtual uint64_t get_number_of_entries_in_section_header_table() = 0;
			virtual void set_number_of_entries_in_section_header_table(uint64_t) = 0;

			virtual uint64_t get_index_of_section_that_contains_section_names() = 0;
			virtual void set_index_of_section_that_contains_section_names(uint64_t) = 0;

			virtual bool are_magic_bytes_correct() = 0;
		};

		template<class T>
		class THeaderParseStrategy : public HeaderParseStrategy
		{
		private:
			ReadWriteBlob<T> _read_write_blob;
		public:
			using InnerType = T;

			THeaderParseStrategy(N_Core::BinaryBlob& header) :
				_read_write_blob(header) 
			{
			}

			~THeaderParseStrategy() override { }
			Type get_type() { return static_cast<Type>(_read_write_blob.get(&T::e_type)); }
			void set_type(Type type) { _read_write_blob.set(&T::e_type, type); }
			Class get_class() { return static_cast<Class>(_read_write_blob.get(&T::e_class)); }
			void set_class(Class class_to_set) { _read_write_blob.set(&T::e_class, class_to_set); }
			Machine get_machine() { return static_cast<Machine>(_read_write_blob.get(&T::e_machine)); }
			void set_machine(Machine machine) { _read_write_blob.set(&T::e_machine, machine); }
			Version get_version() { return static_cast<Version>(_read_write_blob.get(&T::e_version)); }
			void set_version(Version version) { _read_write_blob.set(&T::e_version, version); }

			OS_ABI get_OS_ABI() { return static_cast<OS_ABI>(_read_write_blob.get(&T::e_OS_ABI)); }
			void set_OS_ABI(OS_ABI ABI) { _read_write_blob.set(&T::e_OS_ABI, ABI); }

			uint64_t get_ABI_version() { return static_cast<OS_ABI>(_read_write_blob.get(&T::e_ABI_version)); }
			void set_ABI_version(uint64_t ABI_version) { _read_write_blob.set(&T::e_ABI_version, ABI_version); }

			uint64_t get_entry() { return _read_write_blob.get(&T::e_entry); }
			void set_entry(uint64_t entry) { _read_write_blob.set(&T::e_entry, entry); }
			uint64_t get_program_header_offset() { return _read_write_blob.get(&T::e_phoff); }
			void set_program_header_offset(uint64_t program_header_offset) { _read_write_blob.set(&T::e_phoff, program_header_offset); }
			uint64_t get_section_header_offset() { return _read_write_blob.get(&T::e_shoff); }
			void set_section_header_offset(uint64_t section_header_offset) { _read_write_blob.set(&T::e_shoff, section_header_offset); }
			uint64_t get_flags() { return _read_write_blob.get(&T::e_flags); }
			void set_flags(uint64_t flags) { _read_write_blob.set(&T::e_flags, flags); }
			uint64_t get_header_size() { return _read_write_blob.get(&T::e_ehsize); }
			void set_header_size(uint64_t header_size) { _read_write_blob.set(&T::e_ehsize, header_size); };

			uint64_t get_size_of_program_header_table() { return _read_write_blob.get(&T::e_phentsize); }
			void set_size_of_program_header_table(uint64_t size) { _read_write_blob.set(&T::e_phentsize, size); }
			uint64_t get_size_of_section_header_table() { return _read_write_blob.get(&T::e_shentsize); }
			void set_size_of_section_header_table(uint64_t size) { _read_write_blob.set(&T::e_shentsize, size); }


			uint64_t get_number_of_entries_in_program_header_table() { return _read_write_blob.get(&T::e_phnum); }
			void set_number_of_entries_in_program_header_table(uint64_t number_of_entries) { _read_write_blob.set(&T::e_phnum, number_of_entries); }

			uint64_t get_number_of_entries_in_section_header_table() { return _read_write_blob.get(&T::e_shnum); }
			void set_number_of_entries_in_section_header_table(uint64_t number_of_entries) { _read_write_blob.set(&T::e_shnum, number_of_entries); }
			
			uint64_t get_index_of_section_that_contains_section_names() { return _read_write_blob.get(&T::e_shstrndx); }
			void set_index_of_section_that_contains_section_names(uint64_t section_index) { _read_write_blob.set(&T::e_shstrndx, section_index); }


			bool are_magic_bytes_correct() 
			{
				return _read_write_blob.get(&T::e_magic_byte_0) == 0x7F &&
					_read_write_blob.get(&T::e_magic_byte_1) == 'E' &&
					_read_write_blob.get(&T::e_magic_byte_2) == 'L' &&
					_read_write_blob.get(&T::e_magic_byte_3) == 'F';
			}


		};
	}
}