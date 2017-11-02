#pragma once
#include "src/include/core/section/section_member_types.h"
#include "src/include/core/read_write_blob.h"
#include <memory>
#include <stdint.h>
namespace N_Core
{
	namespace N_Section
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

		class HeaderParseStrategy
		{
		public:
			virtual ~HeaderParseStrategy() {}
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

		template<class T>
		class THeaderParseStrategy: public HeaderParseStrategy
		{
		private:
			ReadWriteBlob<T> _read_write_blob;
		public:
			THeaderParseStrategy(N_Core::BinaryBlob& header) :
				_read_write_blob(header) {}

			uint64_t get_name() { return _read_write_blob.get(&T::sh_name); }
			Type get_type() { return static_cast<Type>(_read_write_blob.get(&T::sh_type)); }
			Flags get_flags() { return static_cast<Flags>(_read_write_blob.get(&T::sh_flags)); }
			uint64_t get_address() { return _read_write_blob.get(&T::sh_addr); }
			uint64_t get_offset() { return _read_write_blob.get(&T::sh_offset); }
			uint64_t get_size() { return _read_write_blob.get(&T::sh_size); }
			uint64_t get_link() { return _read_write_blob.get(&T::sh_link); }
			uint64_t get_info() { return _read_write_blob.get(&T::sh_info); }
			uint64_t get_address_alignment() { return _read_write_blob.get(&T::sh_addralign); }
			uint64_t get_entry_size() { return _read_write_blob.get(&T::sh_entsize); }

			
			void set_name(uint64_t name) { _read_write_blob.set(&T::sh_name, name); }
			void set_type(Type type) { _read_write_blob.set(&T::sh_type, type); }
			void set_flags(Flags flags) { _read_write_blob.set(&T::sh_flags, flags); }
			void set_address(uint64_t address) { _read_write_blob.set(&T::sh_addr, address); }
			void set_offset(uint64_t offset) { _read_write_blob.set(&T::sh_offset, offset); }
			void set_size(uint64_t size) { _read_write_blob.set(&T::sh_size, size); }
			void set_link(uint64_t link) { _read_write_blob.set(&T::sh_link, link); }
			void set_info(uint64_t info) { _read_write_blob.set(&T::sh_info, info); }
			void set_address_alignment(uint64_t alignment) { _read_write_blob.set(&T::sh_addralign, alignment); }
			void set_entry_size(uint64_t size) { _read_write_blob.set(&T::sh_entsize, size); }
		};
	}
}