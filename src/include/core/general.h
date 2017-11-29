#pragma once
#include <boost/range.hpp>
#include <vector>
#include <cstdint>
namespace N_Core
{
	struct Bit32 {};
	struct Bit64 {};
	using BinaryBlob = boost::iterator_range<uint8_t*>; ///< A blob of bytes

	/* @brief Mathematical branch
	*	Avoid jumps and killing instruction pipelining. 
	*   Same as: auto value = (condition)?true_branch:false_branch;
	*   Basically returns value without the jump statement in there.
	*/
	template <typename RV1, typename RV2>
	inline auto math_branch(bool condition, RV1 true_branch, RV2  false_branch)
	{
		return true_branch & (condition  * std::numeric_limits<RV1>::max())
			| false_branch & (not condition * std::numeric_limits<RV2>::max());
	};
};
