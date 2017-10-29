#include <memory>
#include "src/include/core/general.h"


namespace N_Core
{
	namespace N_Header
	{
		class HeaderParseStrategy;
		class Header
		{
		public:
			Header(N_Core::BinaryBlob& header);
			~Header();
			std::unique_ptr<N_Core::N_Header::HeaderParseStrategy> _header_parse_strategy; ///< Will be 64-bit or 32-bit variant.


		};

	}
}