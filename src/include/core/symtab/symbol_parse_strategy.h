#pragma once
#include "src/include/core/read_write_blob.h"
#include <cstdint>
namespace N_Core
{
	namespace N_SymTab
	{
		struct Elf32_Sym {
			uint32_t  st_name;
			uint32_t  st_value;
			uint32_t  st_size;
			uint8_t   st_info;
			uint8_t   st_other;
			uint16_t  st_shndx;

			static const uint64_t SIZE_IN_BYTES = 4 * 4; //4 * 32bits = 4 * 4 bytes
		};

		struct Elf64_Sym {
			uint32_t  st_name;
			uint8_t   st_info;
			uint8_t   st_other;
			uint16_t  st_shndx;
			uint64_t  st_value;
			uint64_t  st_size;

			static const uint64_t SIZE_IN_BYTES = 3 * 8; //3 * 64 bits = 3 * 8 bytes
		};

		class SymbolParseStrategy
		{
		public:
			virtual ~SymbolParseStrategy() {}
			virtual uint64_t get_name() = 0;
			virtual uint64_t get_info() = 0;
			virtual uint64_t get_other() = 0;
			virtual uint64_t get_value() = 0;
			virtual uint64_t get_size() = 0;
			virtual uint64_t get_section_index() = 0;
			virtual void set_name(uint64_t) = 0;
			virtual void set_info(uint64_t) = 0;
			virtual void set_other(uint64_t) = 0;
			virtual void set_value(uint64_t) = 0;
			virtual void set_size(uint64_t) = 0;
			virtual void set_section_index(uint64_t) =0;
		};

		template<class T>
		class TSymbolParseStrategy : public SymbolParseStrategy
		{
		private:
			ReadWriteBlob<T> _read_write_blob;
		public:
			using InnerType = T;
			TSymbolParseStrategy(N_Core::BinaryBlob& header) :
				_read_write_blob(header) {}

			uint64_t get_name() { return _read_write_blob.get(&T::st_name); }
			uint64_t get_info() { return _read_write_blob.get(&T::st_info); }
			uint64_t get_other() { return _read_write_blob.get(&T::st_other); }
			uint64_t get_value() { return _read_write_blob.get(&T::st_value); }
			uint64_t get_size() { return _read_write_blob.get(&T::st_size); }
			uint64_t get_section_index() { return _read_write_blob.get(&T::st_shndx); }
			uint64_t get_total_size_in_bytes() { return T::SIZE_IN_BYTES; }


			void set_name(uint64_t name) { _read_write_blob.set(&T::st_name, name); }
			void set_value(uint64_t value) { _read_write_blob.set(&T::st_value, value); }
			void set_size(uint64_t size) { _read_write_blob.set(&T::st_size, size); }
			void set_info(uint64_t info) { _read_write_blob.set(&T::st_info, info); }
			void set_other(uint64_t other) { _read_write_blob.set(&T::st_other, other); }
			void set_section_index(uint64_t section_index) { _read_write_blob.set(&T::st_shndx, section_index); }
		};
	}
}