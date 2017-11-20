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
		};
	}
}