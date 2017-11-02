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

 		~ReadWriteBlob(){ }

		T const& get_content() { return *ptr; }
		template <typename MemberType, typename ParamType>
		void set(MemberType T::* _member_ptr, ParamType val)
		{
			static_assert(sizeof(T) >= sizeof(ParamType), "Value to set may be bigger than target variable can hold.");

			if (!_allocated_ptr)
			{
				_allocated_ptr = std::make_unique<T>();
				*_allocated_ptr = *_ptr;
				_ptr = _allocated_ptr.get();
			}

			_ptr->*_member_ptr = val;
		}

		template <class MemberType>
		MemberType get(MemberType T::* _member_ptr)
		{
		
			return _ptr->*_member_ptr;
		}
		
	};
}
