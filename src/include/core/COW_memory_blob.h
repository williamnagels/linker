#pragma once
#include <memory>
#include <ostream>
#include <algorithm>

#include "src/include/core/general.h"

namespace N_Core
{



	struct DefaultAllocator
	{		
	};

	struct ArrayAllocator
	{

		uint64_t _desired_size_in_elements; // how many  elements should be stored in the allocated array
		uint64_t _current_size_in_elements; // how many  elements are stored in the allocated array

		ArrayAllocator(uint64_t desired_size_in_elements=0, uint64_t current_size_in_elements=0):
			_desired_size_in_elements(desired_size_in_elements)
			, _current_size_in_elements(current_size_in_elements){}
	};

	template<class T, class Allocator>
	class COW_MemoryBlob;

	template <typename T>
	void allocate(COW_MemoryBlob<T, ArrayAllocator>& cow)
	{
		if (cow._allocated_ptr && cow._allocator._desired_size_in_elements == cow._allocator._current_size_in_elements)
		{
			return;
		}

		std::unique_ptr<COW_MemoryBlob<T, ArrayAllocator>::ValueType[]> temporary_storage = 
			std::make_unique<COW_MemoryBlob<T, ArrayAllocator>::ValueType[]>(cow._allocator._desired_size_in_elements);

		/*std::memcpy(temporary_storage.get(), cow._ptr, cow._allocator._current_size_in_elements);

		cow._allocated_ptr = std::move(temporary_storage);
		cow._ptr = cow._allocated_ptr.get();

		cow._allocator._current_size_in_elements = cow._allocator._desired_size_in_elements;*/
	}
	template <typename T>
	void allocate(COW_MemoryBlob<T, DefaultAllocator>& cow)
	{
		if (cow._allocated_ptr)
		{
			return;
		}

		cow._allocated_ptr = std::make_unique<T>();

		if (cow._ptr)
		{
			*cow._allocated_ptr = *cow._ptr;
		}
		else
		{
			std::memset(cow._allocated_ptr.get(), 0, sizeof(T));
		}

		cow._ptr = cow._allocated_ptr.get();
	}

	// @brief Copy-on-write memory blob
	//
	//
	//
	// The memory region passed in the ctor is reinterpreted as
	// an object of type T. Very useful when working with memory
	// mapped elfs.
	//
	// 
	//
	// @returns true_branch if condition is true else returns false.
	//
	template<class T, class Allocator=DefaultAllocator>
	class COW_MemoryBlob
	{
	private:



	public:
		using ValueType = std::decay_t<T>;
		using PointerType = std::add_pointer_t<ValueType>;
		using UniquePtrType = std::conditional_t<std::is_same_v<Allocator, DefaultAllocator>, ValueType, ValueType[]>;


		PointerType _ptr; ///< Memory access ptr
		Allocator _allocator;
		std::unique_ptr<UniquePtrType> _allocated_ptr; ///< Allocated memory if writes are required.

		template <class = std::enable_if_t<std::is_same_v<Allocator, DefaultAllocator>>>
		COW_MemoryBlob(N_Core::BinaryBlob const& header, std::add_pointer_t<std::enable_if_t<std::is_pod_v<ValueType>>> = 0 ) :
			_ptr(reinterpret_cast<PointerType>(header.begin())) {}


		template <class = int, class = std::enable_if_t<std::is_same_v<Allocator, ArrayAllocator>>>
		COW_MemoryBlob(N_Core::BinaryBlob const& header, std::add_pointer_t<std::enable_if_t<std::is_pod_v<ValueType>>> = 0) :
			_ptr(reinterpret_cast<PointerType>(header.begin()))
			, _allocator(header.size(), header.size()) {}

		COW_MemoryBlob():
			_ptr(nullptr)
			,_allocated_ptr(nullptr)
			, _allocator()
		{
			allocate(*this);
		}


		COW_MemoryBlob(COW_MemoryBlob const& blob) :
			_ptr(blob._ptr)
			,_allocator(blob._allocator)
		{
			if (blob._allocated_ptr)
			{
				//_allocated_ptr = std::make_unique<typename UniquePtrType>(*blob._allocated_ptr); //copy ctor of T
				_ptr = _allocated_ptr.get();
			}
		}

		COW_MemoryBlob(COW_MemoryBlob&& blob) :
			_ptr(blob._ptr)
			, _allocator(blob._allocator)
		{
			if (blob._allocated_ptr)
			{
				_allocated_ptr = std::move(blob)._allocated_ptr;
				_ptr = _allocated_ptr.get();
			}
		}

 		~COW_MemoryBlob(){ }
	};

	template <typename T, typename MemberType>
	MemberType get(COW_MemoryBlob<T, DefaultAllocator> const& cow, MemberType T::* _member_ptr)
	{
		return cow._ptr->*_member_ptr;
	}

	template <typename T, typename MemberType, typename ParamType>
	void set(COW_MemoryBlob<T, DefaultAllocator>& cow, MemberType T::* _member_ptr, ParamType val)
	{
		allocate(cow);
		cow._ptr->*_member_ptr = val;
	}

	template <typename T>
	uint64_t get_size(COW_MemoryBlob<T, DefaultAllocator> const& cow)
	{
		return sizeof(COW_MemoryBlob<T, DefaultAllocator>::ValueType);
	}

	template <typename T>
	uint64_t get_size(COW_MemoryBlob<T, ArrayAllocator> const& cow)
	{

		return cow._allocator._current_size_in_elements * sizeof(COW_MemoryBlob<T, ArrayAllocator>::ValueType);
	}

	template <typename T, typename Allocator>
	void dump(std::ostream& stream, COW_MemoryBlob<T, Allocator>const& blob)
	{	
		stream.write(reinterpret_cast<char const*>(blob._ptr), get_size(blob));
	}

}
