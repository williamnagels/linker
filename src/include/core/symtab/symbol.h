#pragma once
#include "src/include/core/read_write_blob.h"
#include <cstdint>
namespace N_Core
{
	struct Elf32_Sym {
		uint32_t  st_name;
		uint32_t  st_value;
		uint32_t  st_size;
		uint8_t   st_info;
		uint8_t   st_other;
		uint16_t  st_shndx;
	};

	struct Elf64_Sym {
		uint32_t  st_name;
		uint8_t   st_info;
		uint8_t   st_other;
		uint16_t  st_shndx;
		uint64_t  st_value;
		uint64_t  st_size;
	} ;

	class Symbol
	{
	public:
		virtual uint64_t get_name() = 0;
		virtual uint64_t get_info() = 0;
		virtual uint64_t get_other() = 0;
		virtual uint64_t get_value() = 0;
		virtual uint64_t get_size() = 0;
		virtual uint64_t get_section_index() = 0;
	};

	template<class T>
	class TSymbol : public Symbol
	{
	private:
		ReadWriteBlob<T> _read_write_blob;
	public:
		TSymbol(N_Core::BinaryBlob& header) :
			_read_write_blob(header) {}

		uint64_t get_name() { return _read_write_blob.get().st_name; }
		uint64_t get_info() { return _read_write_blob.get().st_info; }
		uint64_t get_other() { return _read_write_blob.get().st_other; }
		uint64_t get_value() { return _read_write_blob.get().st_value; }
		uint64_t get_size() { return _read_write_blob.get().st_size; }
		uint64_t get_section_index() { return _read_write_blob.get().st_shndx; }

		void set_name(uint64_t name) { _read_write_blob.set().st_name = name; }
		void set_value(uint64_t value) { _read_write_blob.set().st_value = value; }
		void set_size(uint64_t size) { _read_write_blob.set().st_size = size; }
		void set_info(uint64_t info) { _read_write_blob.set().st_info = info; }
		void set_other(uint64_t other) { _read_write_blob.set().st_other = other; }
		void set_section_index(uint64_t section_index) { _read_write_blob.set().st_shndx = section_index; }
	};
}
