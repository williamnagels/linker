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
			uint8_t			e_OSABI;
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
			uint8_t			e_OSABI;
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
			virtual Type get_type() = 0;
			virtual void set_type(Type type) = 0;
			virtual Class get_class() = 0;
			virtual void set_class(Class class_to_set) = 0;
		};

		template<class T>
		class THeaderParseStrategy : public HeaderParseStrategy
		{
		private:
			ReadWriteBlob<T> _read_write_blob;
		public:
			using InnerType = T;

			THeaderParseStrategy(N_Core::BinaryBlob& header) :
				_read_write_blob(header) {}

			Type get_type() { return static_cast<Type>(_read_write_blob.get(&T::e_type)); }
			void set_type(Type type) { _read_write_blob.set(&T::e_type, type); }
			virtual Class get_class() { return static_cast<Class>(_read_write_blob.get(&T::e_class)); }
			virtual void set_class(Class class_to_set) { _read_write_blob.set(&T::e_class, class_to_set); }
		};
	}
}