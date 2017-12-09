#include "src/include/core/header/header.h"

#include "src/include/core/strategy_generator.h"
namespace N_Core
{
	namespace N_Header
	{

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
	}
}