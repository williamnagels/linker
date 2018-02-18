#include <algorithm>

#include "src/include/core/general.h"

namespace N_Core
{
	void dump(std::ostream& stream, N_Core::BinaryBlob blob)
	{
		stream.write(reinterpret_cast<char const*>(blob.begin()), blob.size());
	}


	bool operator==(Index const& a, Index const& b)
	{
		return a._value == b._value;
	}
	std::ostream& operator<<(std::ostream& os, Index index)
	{
		os << index._value;
		return os;
	}
	std::ostream& operator<<(std::ostream& os, IndexList indices)
	{
		os << "(";
		for (Index const& index : indices)
		{
			os << index << ",";
		}
		os << ")";

		return os;
	}

	bool is_wildcard(Index index)
	{
		return index._value == Index::Wildcard;
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
	Index operator--(Index& idx, int)
	{
		Index old = idx;
		idx._value--;
		return old;
	}
	Index& operator--(Index& idx)
	{
		idx._value--;
		return idx;
	}
}