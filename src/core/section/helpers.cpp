#include "src/include/core/section/helpers.h"

namespace N_Core
{
	namespace N_Section
	{
		bool is_wildcard(Index index)
		{
			return index == Index::Wildcard;
		}

		Index operator ++(Index& idx, int)
		{
			Index old = idx;
			idx._value++;
			return old;
		}
		Index& operator++(Index& idx)
		{
			idx._value++;
			return idx;
		}
	}
}

