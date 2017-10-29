#include "src/include/core/header/header.h"
#include "src/include/core/header/header_parse_strategy.h"
namespace N_Core
{
	namespace N_Header
	{
		Header::Header(N_Core::BinaryBlob& header)
		{
			/*
			switch (header.size())
			{
			case 0x80: //size T structs
				_header_parse_strategy = std::make_unique<THeaderParseStrategy<Elf64_Shdr>>(header);
				break;
			case 0x70:
				_header_parse_strategy = std::make_unique<THeaderParseStrategy<Elf32_Shdr>>(header);
				break;
			default:
				throw std::invalid_argument("Size of header blob is of unexpected size.");
			}*/
		}

		Header::~Header()
		{
		}
	}
}