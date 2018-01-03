#include "src/include/core/header/header.h"

#include "src/include/core/pimpl_deducer.h"
namespace N_Core
{
	namespace N_Header
	{

		void dump(std::ostream& stream, HeaderA const& header)
		{
			auto ptr32 = dynamic_cast<Header<Elf32_Ehdr> const*>(&header);
			if (ptr32)
			{
				stream << ptr32->_header_content;
			}
			auto ptr64 = dynamic_cast<Header<Elf64_Ehdr> const*>(&header);
			if (ptr64)
			{
				stream << ptr64->_header_content;
			}
		}

		std::unique_ptr<N_Header::HeaderA> create_header(BinaryBlob memory_mapped_elf)
		{
			if (N_Core::N_Header::Header<N_Core::N_Header::Elf32_Ehdr>(memory_mapped_elf).get_class() == N_Core::N_Header::Class::ELFCLASS64)
			{
				return std::make_unique<N_Header::Header<N_Header::Elf64_Ehdr>>(memory_mapped_elf);
			}
			else
			{
				return std::make_unique<N_Header::Header<N_Header::Elf32_Ehdr>>(memory_mapped_elf);
			}
		}

		std::unique_ptr<N_Header::HeaderA> create_header(Class class_to_create_header)
		{
			if (class_to_create_header == N_Core::N_Header::Class::ELFCLASS64)
			{
				return std::make_unique<N_Header::Header<N_Header::Elf64_Ehdr>>();
			}
			else
			{
				return std::make_unique<N_Header::Header<N_Header::Elf32_Ehdr>>();
			}
		}
	}
}