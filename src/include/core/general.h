#pragma once
#include <boost/range.hpp>
#include <vector>
#include <cstdint>
namespace N_Core
{
	using BinaryBlob = boost::iterator_range<uint8_t*>; ///< A blob of bytes
};
