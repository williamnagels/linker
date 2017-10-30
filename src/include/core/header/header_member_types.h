#pragma once
#include <cstdint>
namespace N_Core
{
	namespace N_Header
	{

		enum Identification:uint8_t
		{
			EI_MAG0=0,
			EI_MAG1=1,
			EI_MAG2=2,
			EI_MAG3=3,
			EI_CLASS=4,
			EI_DATA=5,
			EI_VERSION=6,
			EI_OSABI=7,
			EI_ABIVERSION=8,
			EI_NIDENT=16
		};

		enum Type : uint8_t
		{
			ET_NONE = 0,
			ET_REL = 1,
			ET_EXEC = 2,
			ET_DYN = 3,
			ET_CORE = 4
		};

		enum Machine : uint8_t
		{
			EM_NONE=0,
			EM_SPARC=2,
			EM_386=3,
			EM_SPARC32PLUS=18,
			EM_SPARCV9=43
		};

		enum Version :uint8_t
		{
			EV_NONE=0,
			EV_CURRENT=1
		};

		enum Class :uint8_t
		{
			ELFCLASSNONE=0,
			ELFCLASS32=1,
			ELFCLASS64=2
		};
	}
}
