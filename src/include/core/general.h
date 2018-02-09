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


	// @brief Section identifier.
	// 
	// Identifies a unique section in the section table. Used in various API functions.
	//
	// Note: First section has index 0.
	//
	struct Index
	{
		using T = uint16_t;
		T _value;
		static const T Wildcard = std::numeric_limits<T>::max();
		operator decltype(_value)(){ return _value; }

			Index(T value) : _value(value) {}
	};

	Index operator ++(Index& idx, int);
	Index& operator++(Index& idx);

	using IndexList = std::vector<Index>;

	bool operator==(Index const& a, Index const& b);
	std::ostream& operator<<(std::ostream& os, Index);
	std::ostream& operator<<(std::ostream& os, IndexList);
	
	// @brief Wether or not the index is the wildcard index.
	// 
	// Used in the API to check if the index of some section is the wildcard section.
	//
	bool is_wildcard(Index index);

	
	struct SupportsWildcard {};
	struct DoesNotSupportWildCard {};
	template <typename V, typename ContainerTy>
	std::enable_if_t<std::is_same_v<V, DoesNotSupportWildCard>, bool> is_valid_index(ContainerTy const& container, Index index)
	{
		return index < container.size();
	}

	template <typename V, typename ContainerTy>
	std::enable_if_t<std::is_same_v<V, SupportsWildcard>, bool> is_valid_index(ContainerTy const& container, Index index)
	{
		return is_valid_index<DoesNotSupportWildCard>(container, index) || (index == Index::Wildcard);
	}

	template <typename V, typename ContainerTy, typename ...G>
	bool are_valid_indices(ContainerTy const& container, G... indices) 
	{
		bool rc = true;
		for (auto&& x : { indices... })
		{
			rc |= is_valid_index<V>(container, x);
		}
		return rc;
	}
	
	// @brief Multipurpose iterator implementation
	// 
	// Used to access elements in tables (section, symbol, ...)
	// using iterators. The idea is that the iterator
	// points to an element in the table (_container) at position
	// _index. Incrementing the iterator will move to next
	// element in the table. May run out-of-bounds; there
	// are no checks on operator++ or --.
	// This iterator is bidirectional. The container
	// may or may not be random access. To retrieve
	// the element std::advance is used, which is overloaded
	// for any iterator.
	//
	// @note API of container:
	// From the container an element must be constructable 
	// using the api: GetElementAt(index). This is a
	// compile-time enforced constraint.
	//
	template <typename ContainerTy, typename T, bool IsConst>
	class Iterator
	{
		uint16_t _current_index;
		ContainerTy& _container;

	public:
		Iterator(ContainerTy& container, uint16_t index) :
			_container(container),
			_current_index(index) {}
		Iterator(Iterator const& o) :
			_current_index(o._current_index),
			_container(o._container) {};

		bool operator ==(Iterator const& other)
		{
			return _current_index == other._current_index;
		}
		bool operator!=(Iterator const& other)
		{
			return !(*this == other);
		}

		Iterator& operator++()
		{
			_current_index++;
			return *this;
		}

		Iterator& operator--()
		{
			_current_index--;
			return *this;
		}
		Iterator operator++(int)
		{
			auto current_iterator = *this;
			++*this;
			return current_iterator;
		}
		Iterator operator--(int)
		{
			auto current_iterator = *this;
			--*this;
			return current_iterator;
		}
		using ValueType =
			std::conditional_t<
			IsConst
			, std::add_const_t<T>
			, T>;
		ValueType operator*()
		{
			auto begin_it = _table.get_content().begin();
			std::advance(begin_it, index);
			auto end_it = begin_it;
			std::advance(end_it, sizeof(T));
			return ValueType(begin_it, end_it);
		}
	};

};
