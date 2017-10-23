#pragma once
#include <memory>
#include "src/include/core/general.h"
namespace N_Core
{
	template<class T>
	class ReadWriteBlob
	{
	private:
		T* _ptr; ///< Memory access ptr
		std::unique_ptr<T> _allocated_ptr; ///< Allocated memory if writes are required.
		T* allocate_if_required();
	public:
		ReadWriteBlob(N_Core::BinaryBlob& header) :
			_ptr(reinterpret_cast<T*>(&(*header.begin()))) {}

		template <typename MemberType>
		void set(MemberType T::* _member_ptr, uint64_t val)
		{
			_ptr->*_member_ptr = val;
		}

		template <class MemberType>
		MemberType get(MemberType T::* _member_ptr)
		{
		
			return _ptr->*_member_ptr;
		}
		
	};
}
