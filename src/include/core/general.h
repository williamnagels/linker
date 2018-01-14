#pragma once
#include "cow.h"

#include <boost/range.hpp>
#include <vector>
#include <cstdint>
namespace N_Core
{
	struct Bit64 {};
	struct Bit32 {};


	// Region of memory. Start and end-address range.
	using BinaryBlob = boost::iterator_range<uint8_t*>;

	// @brief Helper function to dump a region of memory to an output stream
	// 
	// @param stream	Output stream to write memory region to.
	// @param blob		The memory region to write to the stream.
	// 
	void dump(std::ostream& stream, BinaryBlob blob);

	template <typename T>
	void dump(std::ostream& stream, MMap::Container<T> const& container)
	{
		stream.write(reinterpret_cast<char const*>(container._ptr), container.get_size());
	}
	
	struct Math {}; ///< Tag to dispatch math branch overload.
	struct Regular {}; ///< Tag to dispatch regular branch overload.

	// @brief Regular branch implementation.
	// 
	// @param condition		statement expected to be true or false
	// @param true_branch	value to return if condition is true
	// @param false_branch	value to return if condition is false
	//
	// @returns true_branch if condition is true else returns false.
	//
	template <typename RV1, typename RV2>
	inline auto branch(bool condition, RV1 true_branch, RV2  false_branch, Regular)
	{
		return condition ? true_branch:false_branch;
	}

	// @brief Mathematical branch
	//
	// Avoid jumps if not reqired and thus avoids bubbles in the CPU pipeline.
	//
	// @param condition		statement expected to be true or false
	// @param true_branch	value to return if condition is true
	// @param false_branch	value to return if condition is false
	//
	// @returns true_branch if condition is true else returns false.
	//
	template <typename RV1, typename RV2>
	inline auto branch(bool condition, RV1 true_branch, RV2  false_branch, Math)
	{
		auto true_branch_val = true_branch & (condition  * std::numeric_limits<RV1>::max());
		auto false_branch_val = false_branch & (not condition * std::numeric_limits<RV2>::max());
		return true_branch_val | false_branch_val;
	};

	namespace
	{
		// @brief Used as storage type used by the void iterator.
		//
		struct VoidConsumer {};
	}

	// @brief Void iterator
	//
	// Used in std::transform to create a zip iterator.
	//
	template <typename T= VoidConsumer>
	struct VoidIterator
	{
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = std::add_pointer_t<T>;
		using reference = std::add_lvalue_reference<T>;

		VoidIterator() :
			t(nullptr) {}

		VoidIterator<T> operator+(const ptrdiff_t& movement) { return *this; }
		VoidIterator<T> operator++() { return *this; }
		T& operator*() { return *t; }

	private:
		T * t;
	};

};
