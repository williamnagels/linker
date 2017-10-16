#include "src/include/core/read_write_blob.h"

namespace N_Core
{
	template <class T>
	T* ReadWriteBlob<T>::allocate_if_required()
	{
		if (_allocated_ptr)
		{
			return _allocated_ptr;
		}

		_allocated_ptr = std::make_unique<T>();
		*_allocated_ptr = *_ptr;
		_ptr = _allocated_ptr;

		return _ptr;
	}
}