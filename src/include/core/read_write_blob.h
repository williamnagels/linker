#pragma once
#include <memory>
#include <ostream>
#include "src/include/core/general.h"
namespace N_Core
{


	template<class T>
	class ReadWriteBlob
	{
	private:

		std::unique_ptr<T> _allocated_ptr; ///< Allocated memory if writes are required.

		void allocate_if_required()
		{
			if (_allocated_ptr)
			{
				return;
			}

			_allocated_ptr = std::make_unique<T>();

			if (_ptr)
			{
				*_allocated_ptr = *_ptr;
			}
			else
			{
				std::memset(_allocated_ptr.get(), 0, sizeof(T));
			}

			_ptr = _allocated_ptr.get();
		}
	public:
		T * _ptr; ///< Memory access ptr
		ReadWriteBlob(N_Core::BinaryBlob const& header, std::add_pointer_t<std::enable_if_t<std::is_pod_v<T>>> = 0 ) :
			_ptr(reinterpret_cast<T*>(header.begin())) {}

		ReadWriteBlob():
			_ptr(nullptr)
			,_allocated_ptr(nullptr)
		{
			allocate_if_required();
		}

		ReadWriteBlob(ReadWriteBlob const& blob) :
			_ptr(blob._ptr)
		{
			if (blob._allocated_ptr)
			{
				_allocated_ptr = std::make_unique<T>(*blob._allocated_ptr);
				_ptr = _allocated_ptr.get();
			}
		}

		ReadWriteBlob(ReadWriteBlob&& blob) :
			_ptr(blob._ptr)
		{
			if (blob._allocated_ptr)
			{
				_allocated_ptr = std::move(blob)._allocated_ptr;
				_ptr = _allocated_ptr.get();
			}
		}

 		~ReadWriteBlob(){ }


		template <typename MemberType, typename ParamType>
		void set(MemberType T::* _member_ptr, ParamType val)
		{
			allocate_if_required();

			_ptr->*_member_ptr = val;
		}

		template <class MemberType>
		MemberType get(MemberType T::* _member_ptr) const
		{
			return _ptr->*_member_ptr; 
		}



		//friend void dump(std::ostream& stream, ReadWriteBlob<T> const& header);
	};
	template <typename T>
	void dump(std::ostream& stream, ReadWriteBlob<T>const& header)
	{
		stream.write(reinterpret_cast<char const*>(header._ptr), sizeof(T));
	}
}
