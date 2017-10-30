#pragma once
#include <memory>
#include "src/include/core/general.h"
#include "src/include/core/header/header_member_types.h"

namespace N_Core
{
	namespace N_Header
	{
		class HeaderParseStrategy;
		class Header
		{
		public:
			Header(N_Core::BinaryBlob& header);
			Header(Header&&);
			~Header();
			std::unique_ptr<N_Core::N_Header::HeaderParseStrategy> _header_parse_strategy; ///< Will be 64-bit or 32-bit variant.

			/*@brief From some blob of memory create a header object
			*
			* It is assumed the header starts at byte with index 0 (as it would when memory mapping an elf).
			* This function exists to resolve an existance issue. The size of the header is in the header
			* but in order to create the header from a memory blob you must know its size.
			* This function will access the location of the size, read it and create a header object from it.
			*
			*/
			friend Header create_header_from_memory_blob(N_Core::BinaryBlob& memory_range_with_header_at_top);
		};
	}
}