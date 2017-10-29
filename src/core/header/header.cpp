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
	}
}