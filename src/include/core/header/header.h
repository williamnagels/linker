#pragma once
#include <memory>
#include <type_traits>
#include "src/include/core/general.h"
#include "src/include/core/header/header_member_types.h"
#include "src/include/core/read_write_blob.h"
namespace N_Core
{
	namespace N_Header
	{
		const std::string wrong_magic_bytes_message = "Wrong magic bytes in elf header. Did not find /7FELF. Is the file an elf?";

		template <typename T>
		class Header
		{
			using MemoryMap = std::conditional_t <std::is_same_v<Bit32, T>, Elf32_Ehdr, Elf64_Ehdr>;
			ReadWriteBlob<MemoryMap> _header_content; ///< Memory blob with some map applied to it.
			bool are_magic_bytes_correct();
		public:		
			Header(N_Core::BinaryBlob const& header_memory_blob);
			
			Header(Header const& header);
			Header() = delete;
			

		};
	}
}