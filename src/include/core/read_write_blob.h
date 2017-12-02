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
		T* _ptr; ///< Memory access ptr
		std::unique_ptr<T> _allocated_ptr; ///< Allocated memory if writes are required.

		void allocate_if_required()
		{
			if (_allocated_ptr)
			{
				return;
			}

			_allocated_ptr = std::make_unique<T>();
			*_allocated_ptr = *_ptr;
			_ptr = _allocated_ptr.get();
		}
	public:
		
		ReadWriteBlob(
			N_Core::BinaryBlob const& header, 
			std::add_pointer_t<std::enable_if_t<std::is_pod_v<T>>> = 0 ) :
			_ptr(reinterpret_cast<T*>(header.begin())) {}

		ReadWriteBlob(ReadWriteBlob const& blob)
		{

		}

 		~ReadWriteBlob(){ }

		T const& get_content() { return *ptr; }


		template <typename MemberType, typename ParamType> //, std::enable_if_t<sizeof(T) <= sizeof(ParamType), int> = 0 
		void set(MemberType T::* _member_ptr, ParamType val)
		{
			static_assert(sizeof(T) >= sizeof(ParamType), "Value to set may be bigger than target variable can hold.");

			allocate_if_required();

			_ptr->*_member_ptr = val;
		}

		template <class MemberType>
		MemberType get(MemberType T::* _member_ptr)
		{
			return _ptr->*_member_ptr;
		}



		friend void dump(std::ostream& stream, ReadWriteBlob<T> const& header);
	};
	template <class T>
	void dump(std::ostream& stream, ReadWriteBlob<T>const& header)
	{
		stream.write(reinterpret_cast<uint8_t*>(header._ptr), sizeof(T));
	}
}
