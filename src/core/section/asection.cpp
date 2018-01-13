#include "src/include/core/section/asection.h"
#include "src/include/core/section/section.h"

namespace N_Core
{
	namespace N_Section
	{
		std::unique_ptr<ASection> create_section(N_Core::BinaryBlob elf_blob, N_Core::BinaryBlob header_blob)
		{
			switch (header_blob.size())
			{
			case sizeof(Elf32_Shdr) :
				return std::make_unique<Section<Elf32_Shdr>>(header_blob, elf_blob);
			case sizeof(Elf64_Shdr) :
				return std::make_unique<Section<Elf64_Shdr>>(header_blob, elf_blob);
			default:
				throw std::invalid_argument("Blob is of unexpected size");
			}

		}



		std::unique_ptr<ASection> create_section(bool is_64_bit)
		{
			if (is_64_bit)
			{
				return  std::make_unique<Section<Elf64_Shdr>>();
			}
			else
			{
				return std::make_unique<Section<Elf32_Shdr>>();
			}
		}
	}
}

