#pragma once
#include "cow.h"

#include <boost/range.hpp>
#include <utility>
#include <vector>
#include <set>
#include <cstdint>
#include <optional>
#include <functional>
#include <variant>
namespace N_Core
{
	struct EmptyMeta{};

	struct Bit64 {};
	struct Bit32 {};

	using AddressRange = std::pair<uint8_t*, uint8_t*>;
	// Region of memory. Start and end-address range.
	using BinaryBlob = boost::iterator_range<uint8_t*>;

	using OptionalNonOwningMemory = std::optional < std::reference_wrapper<const MMap::Container<uint8_t>>>;
	// @brief Helper function to dump a region of memory to an output stream
	// 
	// @param stream	Output stream to write memory region to.
	// @param blob		The memory region to write to the stream.
	// 
	void dump(std::ostream& stream, BinaryBlob blob);

	template <typename T>
	std::ostream& operator<<(std::ostream& stream, MMap::Container<T> const& container)
	{
		stream.write(reinterpret_cast<char const*>(container._ptr), container.get_size());
		return stream;
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

		explicit Index(T value) : _value(value) {}
	};

	Index operator ++(Index& idx, int);
	Index& operator++(Index& idx);
	Index operator --(Index& idx, int);
	Index& operator--(Index& idx);
	using IndexList = std::set<Index>;

	bool operator==(Index const& a, Index const& b);
	bool operator<(Index const& a, Index const& b);
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

	template <typename ContainerIteratorTy, typename ElementIteratorTy, typename Value= typename std::iterator_traits<ElementIteratorTy>::value_type>
	class Iterator
		: public boost::iterator_facade<
		  Iterator<ContainerIteratorTy, ElementIteratorTy, Value>
		, Value
		, boost::forward_traversal_tag>
	{
	public:

		//@brief create iterator at last container; last element.
		//
		//@param it_end past end iterator of last container.
		//
		explicit Iterator(ContainerIteratorTy it_end) :
			_it_container_current(std::prev(it_end))
			, _it_container_end(it_end)
		{
			init_element_it_from_table_it();
			_it_element_current = _it_element_end;
			_it_container_current = _it_container_end;
		};


		//@brief create iterator to loop over range of contains.
		//
		//@param it_begin iterator to first valid container.
		//@param it_end past end iterator of last container.
		//
		explicit Iterator(ContainerIteratorTy it_begin, ContainerIteratorTy it_end) :
			_it_container_current(it_begin)
			,_it_container_end(it_end)
		{
			init_element_it_from_table_it();

			//look for first table which contains elements and is not empty
			while (_it_element_current == _it_element_end && _it_container_current != _it_container_end)
			{
				increment();
			}
		}

	private:
		friend class boost::iterator_core_access;

		bool equal(Iterator const& other) const
		{
			return _it_element_current == other._it_element_current && _it_container_current == other._it_container_current;
		}

		void increment()
		{
			//move to next element in the table.
			_it_element_current = std::next(_it_element_current);
			if (_it_element_current != _it_element_end)
			{
				return;
			}

			//table was empty; go to next table
			_it_container_current = std::next(_it_container_current);

			//out of tables & elements.
			if (_it_container_current == _it_container_end)
			{
				return;
			}

			//grab a hold of first & last elements in the new table.
			init_element_it_from_table_it();
		}

		Value& dereference() const
		{
			return (*_it_element_current);
		}

		void init_element_it_from_table_it()
		{
			auto& interpreted_content = _it_container_current->get_interpreted_content();
			auto& symbol_table = std::get<1>(interpreted_content);

			_it_element_current = std::begin(symbol_table);
			_it_element_end = std::end(symbol_table);
		}

		ContainerIteratorTy _it_container_current;
		ContainerIteratorTy _it_container_end;
		ElementIteratorTy _it_element_current;
		ElementIteratorTy _it_element_end;
	};
};
//
//using Iterator = node_iterator;
//using ConstIterator = 
//typedef impl::Iterator<node_base> node_iterator;
//typedef impl::Iterator<node_base const> node_const_iterator;
