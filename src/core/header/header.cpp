#include "src/include/core/header/header.h"

#include "src/include/core/strategy_generator.h"
namespace N_Core
{
	namespace N_Header
	{

		//@brief dump header to stream
		//
		void dump(std::ostream& stream, HeaderA const& header)
		{
			auto ptr32 = dynamic_cast<Header<Elf32_Ehdr> const*>(&header);
			if (ptr32)
			{
				return dump(stream, ptr32->_header_content);
			}
			auto ptr64 = dynamic_cast<Header<Elf64_Ehdr> const*>(&header);
			if (ptr64)
			{
				return dump(stream, ptr64->_header_content);
			}
		}

		std::unique_ptr<N_Header::HeaderA> create_header(BinaryBlob blob)
		{
			std::unique_ptr<N_Header::HeaderA> header;

			if (N_Core::N_Header::Header<N_Core::N_Header::Elf32_Ehdr>(blob).is_64bit_header())
			{
				header = std::make_unique<N_Header::Header<N_Header::Elf64_Ehdr>>(blob);
			}
			else
			{
				header = std::make_unique<N_Header::Header<N_Header::Elf32_Ehdr>>(blob);
			}

			return header;
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
	}
}