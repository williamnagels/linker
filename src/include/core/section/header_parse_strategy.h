#pragma once
#include "src/include/core/section/types.h"
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
			T* _ptr; ///< Memory access ptr
			std::unique_ptr<T> _allocated_ptr; ///< Allocated memory if writes are required.
			T* allocate_if_required();

		public:
			THeaderParseStrategy(N_Core::BinaryBlob& header) :
				_ptr(reinterpret_cast<T*>(&(*header.begin()))){}

			uint64_t get_name() { return _ptr->sh_name; }
			Type get_type() { return static_cast<Type>(_ptr->sh_type); }
			Flags get_flags() { return static_cast<Flags>(_ptr->sh_flags); }
			uint64_t get_address() { return _ptr->sh_addr; }
			uint64_t get_offset() { return _ptr->sh_offset; }
			uint64_t get_size() { return _ptr->sh_size; }
			uint64_t get_link() { return _ptr->sh_link; }
			uint64_t get_info() { return _ptr->sh_info; }
			uint64_t get_address_alignment() { return _ptr->sh_addralign; }
			uint64_t get_entry_size() { return _ptr->sh_entsize; }


			void set_name(uint64_t name) { allocate_if_required()->sh_name = name; }
			void set_type(Type type) { allocate_if_required()->sh_type = type; }
			void set_flags(Flags flags) { allocate_if_required()->sh_flags = flags; }
			void set_address(uint64_t address) { allocate_if_required()->sh_addr = address; }
			void set_offset(uint64_t offset) { allocate_if_required()->sh_offset = offset; }
			void set_size(uint64_t size) { allocate_if_required()->sh_size = size; }
			void set_link(uint64_t link) { allocate_if_required()->sh_link = link; }
			void set_info(uint64_t info) { allocate_if_required()->sh_info = info; }
			void set_address_alignment(uint64_t alignment) { allocate_if_required()->sh_addralign = alignment; }
			void set_entry_size(uint64_t size) { allocate_if_required()->sh_entsize = size; }
		};
	}
}