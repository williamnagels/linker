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

		T const& get() { return *_ptr; }
		T& set() { return *allocate_if_required(); }
	};
}
