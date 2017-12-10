#pragma once
#include <cstdint>
#include <ostream>
namespace N_Core
{
	namespace N_Header
	{
		enum class Class : uint8_t
		{
			ELFCLASSNONE = 0,
			ELFCLASS32 = 1,
			ELFCLASS64 = 2
		};

		inline std::ostream& operator<<(std::ostream& os, Class const& class_to_print)
		{
			os << class_to_print;
			return os;
		}
		enum Type : uint16_t
		{
			ET_NONE = 0,
			ET_REL = 1,
			ET_EXEC = 2,
			ET_DYN = 3,
			ET_CORE = 4
		};

		enum Machine : uint16_t
		{
			EM_NONE = 0,
			EM_SPARC = 2,
			EM_386 = 3,
			EM_68K = 4,
			EM_88K = 5,
			EM_IAMCU = 6,
			EM_860 = 7,
			EM_SPARC32PLUS = 18,
			EM_SPARCV9 = 43,
			EM_IA_64 = 50,
			EM_X86_64 = 62
		};

		enum Version :uint32_t
		{
			EV_NONE = 0,
			EV_CURRENT = 1
		};

		enum OS_ABI : uint8_t
		{
			ELFOSABI_NONE = 0,
			ELFOSABI_HPUX = 1,
			ELFOSABI_NETBSD = 2,
			ELFOSABI_LINUX = 3,
			ELFOSABI_SOLARIS = 6,
			ELFOSABI_AIX = 7,
			ELFOSABI_IRIX = 8,
			ELFOSABI_FREEBSD = 9,
			ELFOSABI_TRU64 = 10,
			ELFOSABI_MODESTO = 11,
			ELFOSABI_OPENBSD = 12,
			ELFOSABI_OPENVMS = 13,
			ELFOSABI_NSK = 14,
			ELFOSABI_AROS = 15,
			ELFOSABI_FENIXOS = 16,
			ELFOSABI_CLOUDABI = 17,
			ELFOSABI_OPENVOS = 18
		};
		struct Elf32_Ehdr
		{
			uint8_t			e_magic_byte_0;
			uint8_t			e_magic_byte_1;
			uint8_t			e_magic_byte_2;
			uint8_t			e_magic_byte_3;
			Class			e_class;
			uint8_t			e_data;
			uint8_t			e_file_version;
			OS_ABI			e_OS_ABI;
			uint8_t			e_ABI_version;
			uint8_t			e_padding_byte_0;
			uint8_t			e_padding_byte_1;
			uint8_t			e_padding_byte_2;
			uint8_t			e_padding_byte_3;
			uint8_t			e_padding_byte_4;
			uint8_t			e_padding_byte_5;
			uint8_t			e_padding_byte_6;
			Type            e_type;
			Machine         e_machine;
			uint32_t        e_version;
			uint32_t        e_entry;
			uint32_t        e_phoff;
			uint32_t        e_shoff;
			uint32_t        e_flags;
			uint16_t        e_ehsize;
			uint16_t        e_phentsize;
			uint16_t        e_phnum;
			uint16_t        e_shentsize;
			uint16_t        e_shnum;
			uint16_t        e_shstrndx;
		};


		struct Elf64_Ehdr
		{
			uint8_t			e_magic_byte_0;
			uint8_t			e_magic_byte_1;
			uint8_t			e_magic_byte_2;
			uint8_t			e_magic_byte_3;
			Class			e_class;
			uint8_t			e_data;
			uint8_t			e_file_version;
			OS_ABI			e_OS_ABI;
			uint8_t			e_ABI_version;
			uint8_t			e_padding_byte_0;
			uint8_t			e_padding_byte_1;
			uint8_t			e_padding_byte_2;
			uint8_t			e_padding_byte_3;
			uint8_t			e_padding_byte_4;
			uint8_t			e_padding_byte_5;
			uint8_t			e_padding_byte_6;
			Type		    e_type;
			Machine         e_machine;
			uint32_t        e_version;
			uint64_t        e_entry;
			uint64_t        e_phoff;
			uint64_t        e_shoff;
			uint32_t        e_flags;
			uint16_t        e_ehsize;
			uint16_t        e_phentsize;
			uint16_t        e_phnum;
			uint16_t        e_shentsize;
			uint16_t        e_shnum;
			uint16_t        e_shstrndx;
		};
	}
}
