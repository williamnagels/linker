#include <src/include/core/general.h>
#include <src/include/core/section/header_parse_strategy.h>
namespace N_Core
{
	namespace N_Section
	{
		template <class T>
		T* THeaderParseStrategy<T>::allocate_if_required()
		{
			if (_allocated_ptr)
			{
				return _allocated_ptr;
			}

			T* backup_ptr = _ptr;

			_allocated_ptr = std::make_unique<T>();
			*_allocated_ptr = *backup_ptr;
			_ptr = _allocated_ptr;

			return _ptr;
		}
		/*
		HeaderParseStrategy64Bit::HeaderParseStrategy64Bit(N_Core::BinaryBlob& header):
			_header(header)
		{
			
		}

		HeaderParseStrategy32Bit::HeaderParseStrategy32Bit(N_Core::BinaryBlob& header) :
			 _header(header)
			,_ptr(reinterpret_cast<Elf32_Shdr*>(&(*_header.begin())))
		{
		}

		THeaderParseStrategy::THeaderParseStrategy(N_Core::BinaryBlob& header)
		{

		}*/
	}
}