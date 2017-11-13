#include "src/include/core/header/header.h"

#include "src/include/core/strategy_generator.h"
namespace N_Core
{
	namespace N_Header
	{

		template<typename T>
		bool Header<T>::are_magic_bytes_correct()
		{
			return _header_content.get(&MemoryMap::e_magic_byte_0) == 0x7F &&
				_header_content.get(&MemoryMap::e_magic_byte_1) == 'E' &&
				_header_content.get(&MemoryMap::e_magic_byte_2) == 'L' &&
				_header_content.get(&MemoryMap::e_magic_byte_3) == 'F';
		}

		template<typename T>
		Header<T>::Header(N_Core::BinaryBlob const& header_memory_blob) :
			_header_content(header_memory_blob)
		{

			if (!are_magic_bytes_correct())
			{
				throw std::invalid_argument(wrong_magic_bytes_message);
			}

		}

		template<typename T>
		Header<T>::Header(Header const& header) :
			_header_content(header._header_content)
		{

		}

		/*
		Header create_header_from_memory_blob(N_Core::BinaryBlob& memory_range_with_header_at_top)
		{
			uint8_t* base_address = memory_range_with_header_at_top.begin();

			Class class_of_elf = static_cast<Class>(*(base_address + 4)); //Fourth byte (zero byte - actual 5th byte) is class for both 32 and 64 bit elfs.

			uint64_t size_of_header = 0;
			switch (class_of_elf)
			{

			case ELFCLASS32:
				size_of_header =  sizeof(Elf32_Ehdr);
				break;
			case ELFCLASS64:
				size_of_header =  sizeof(Elf64_Ehdr);
				break;
			default:
				throw std::range_error("class is not in valid range. Could not deduce size of header.");
			}

			return Header(BinaryBlob(base_address, base_address + size_of_header));

		}*/

		template class Header<Bit32>;
		template class Header<Bit64>;
	}
}