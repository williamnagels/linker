#pragma once
#include "src/include/core/general.h"
namespace N_Core
{
	namespace N_Symbol
	{
		template <typename T>
		class Symbol
		{
		public:
			Symbol(BinaryBlob Range) {}
		};

		template <typename T>
		std::ostream& operator<<(std::ostream& stream, Symbol<T> const& symbol)
		{
			return stream;
		}
	}
}

