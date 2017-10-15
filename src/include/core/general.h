#pragma once
#include <boost/range.hpp>
#include <vector>
#include <cstdint>
namespace N_Core
{
	using BinaryBlob = boost::iterator_range<std::vector<uint8_t>::iterator>; ///< A blob of bytes
};
