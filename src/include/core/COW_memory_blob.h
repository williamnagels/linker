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

		std::unique_ptr<T> temporary_storage = std::make_unique<T>(cow._allocator._desired_size_in_elements);

		std::memcpy(temporary_storage.get(), cow._ptr.get(), cow._allocator._current_size_in_elements);

		cow._allocated_ptr = std::move(temporary_storage);
		cow._ptr = cow._allocated_ptr.get();

		cow._allocator._current_size_in_elements = cow._allocator._desired_size_in_elements;
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


		PointerType _ptr; ///< Memory access ptr
		Allocator _allocator;
		std::unique_ptr<ValueType> _allocated_ptr; ///< Allocated memory if writes are required.

		COW_MemoryBlob(N_Core::BinaryBlob const& header, std::add_pointer_t<std::enable_if_t<std::is_pod_v<T>>> = 0 ) :
			_ptr(reinterpret_cast<PointerType>(header.begin())) {}

		COW_MemoryBlob():
			_ptr(nullptr)
			,_allocated_ptr(nullptr)
		{
			allocate(*this);
		}

		COW_MemoryBlob(COW_MemoryBlob const& blob) :
			_ptr(blob._ptr)
		{
			if (blob._allocated_ptr)
			{
				_allocated_ptr = std::make_unique<T>(*blob._allocated_ptr); //copy ctor of T
				_ptr = _allocated_ptr.get();
			}
		}

		COW_MemoryBlob(COW_MemoryBlob&& blob) :
			_ptr(blob._ptr)
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
	void dump(std::ostream& stream, COW_MemoryBlob<T>const& header)
	{	
		stream.write(reinterpret_cast<char const*>(header._ptr), sizeof(T));
	}



}
