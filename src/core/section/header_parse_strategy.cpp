#include <src/include/core/general.h>
#include <src/include/core/section/header_parse_strategy.h>
namespace N_Core
{
	namespace N_Section
	{
		HeaderParseStrategy64Bit::HeaderParseStrategy64Bit(N_Core::BinaryBlob& header):
			_header(header)
		{
			
		}

		HeaderParseStrategy32Bit::HeaderParseStrategy32Bit(N_Core::BinaryBlob& header) :
			_header(header)
		{

		}
	}
}