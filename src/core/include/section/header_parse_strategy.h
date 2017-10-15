#pragma once
#include "src/core/include/section/types.h"
#include <stdint.h>
namespace N_Core
{
	namespace N_Section
	{

		class HeaderParseStrategy
		{
		public:
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
		};

		class HeaderParseStrategy64Bit: public HeaderParseStrategy
		{
			struct Elf64_Shdr {
				uint32_t        sh_name;
				uint32_t        sh_type;
				uint64_t        sh_flags;
				uint64_t        sh_addr;
				uint64_t        sh_offset;
				uint64_t        sh_size;
				uint32_t        sh_link;
				uint32_t        sh_info;
				uint64_t        sh_addralign;
				uint64_t        sh_entsize;
			};
			N_Core::BinaryBlob& _header;
		public:			
			HeaderParseStrategy64Bit(N_Core::BinaryBlob& blob);
			uint64_t get_name() { return reinterpret_cast<Elf64_Shdr*>(&(*_header.begin()))->sh_name; }
			Type get_type() { return static_cast<Type>(reinterpret_cast<Elf64_Shdr*>(&(*_header.begin()))->sh_type); }
			Flags get_flags() { return static_cast<Flags>(reinterpret_cast<Elf64_Shdr*>(&(*_header.begin()))->sh_flags); }
			uint64_t get_address() { return reinterpret_cast<Elf64_Shdr*>(&(*_header.begin()))->sh_addr;}
			uint64_t get_offset() { return reinterpret_cast<Elf64_Shdr*>(&(*_header.begin()))->sh_offset; }
			uint64_t get_size() { return reinterpret_cast<Elf64_Shdr*>(&(*_header.begin()))->sh_size; }
			uint64_t get_link() { return reinterpret_cast<Elf64_Shdr*>(&(*_header.begin()))->sh_link; }
			uint64_t get_info() { return reinterpret_cast<Elf64_Shdr*>(&(*_header.begin()))->sh_info; }
			uint64_t get_address_alignment() { return reinterpret_cast<Elf64_Shdr*>(&(*_header.begin()))->sh_addralign; }
			uint64_t get_entry_size() { return reinterpret_cast<Elf64_Shdr*>(&(*_header.begin()))->sh_entsize; }
		};

		class HeaderParseStrategy32Bit: public HeaderParseStrategy
		{
			struct Elf32_Shdr {
				uint32_t        sh_name;
				uint32_t		sh_type;
				uint32_t		sh_flags;
				uint32_t		sh_addr;
				uint32_t        sh_offset;
				uint32_t	    sh_size;
				uint32_t		sh_link;
				uint32_t		sh_info;
				uint32_t        sh_addralign;
				uint32_t        sh_entsize;
			};
			N_Core::BinaryBlob& _header;
		public:
			HeaderParseStrategy32Bit(N_Core::BinaryBlob& header);
			uint64_t get_name() { return reinterpret_cast<Elf32_Shdr*>(&(*_header.begin()))->sh_name; }
			Type get_type() { return static_cast<Type>(reinterpret_cast<Elf32_Shdr*>(&(*_header.begin()))->sh_type); }
			Flags get_flags() { return static_cast<Flags>(reinterpret_cast<Elf32_Shdr*>(&(*_header.begin()))->sh_flags); }
			uint64_t get_address() { return reinterpret_cast<Elf32_Shdr*>(&(*_header.begin()))->sh_addr; }
			uint64_t get_offset() { return reinterpret_cast<Elf32_Shdr*>(&(*_header.begin()))->sh_offset; }
			uint64_t get_size() { return reinterpret_cast<Elf32_Shdr*>(&(*_header.begin()))->sh_size; }
			uint64_t get_link() { return reinterpret_cast<Elf32_Shdr*>(&(*_header.begin()))->sh_link; }
			uint64_t get_info() { return reinterpret_cast<Elf32_Shdr*>(&(*_header.begin()))->sh_info; }
			uint64_t get_address_alignment() { return reinterpret_cast<Elf32_Shdr*>(&(*_header.begin()))->sh_addralign; }
			uint64_t get_entry_size() { return reinterpret_cast<Elf32_Shdr*>(&(*_header.begin()))->sh_entsize; }
		};
	}
}