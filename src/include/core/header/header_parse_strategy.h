#pragma once
#include "src/include/core/header/header_parse_strategy.h"
#include "src/include/core/read_write_blob.h"
#include <memory>
#include <stdint.h>
namespace N_Core
{
	namespace N_Header
	{
		///using EI_NIDENT = (uint8_t)16;

		struct Elf32_Ehdr
		{
			uint8_t			e_ident[16];
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
			uint8_t			e_ident[16];
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
			virtual uint64_t get_name() = 0;
		};

		template<class T>
		class THeaderParseStrategy : public HeaderParseStrategy
		{
		private:
			ReadWriteBlob<T> _read_write_blob;
		public:
			THeaderParseStrategy(N_Core::BinaryBlob& header) :
				_read_write_blob(header) {}

			uint64_t get_name() { return _read_write_blob.get(&T::sh_name); }
			void set_name(uint64_t name) { _read_write_blob.set(&T::sh_name, name); }
		};
	}
}