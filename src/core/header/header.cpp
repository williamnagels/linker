#include "src/include/core/header/header.h"
#include "src/include/core/header/header_parse_strategy.h"
#include "src/include/core/strategy_generator.h"
namespace N_Core
{
	namespace N_Header
	{
		Header::Header(N_Core::BinaryBlob& header)
		{
			_header_parse_strategy = create_strategy_from_size(PossibleGeneratorTypes<HeaderParseStrategy, THeaderParseStrategy<Elf32_Ehdr>, THeaderParseStrategy<Elf64_Ehdr>>(), header);
		}

		Header::~Header()
		{
		}

		Header::Header(Header&& header)
		{
			(*this)._header_parse_strategy = std::move(header._header_parse_strategy);
		}


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

		}
	}
}