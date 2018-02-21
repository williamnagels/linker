#pragma once
#include "src/include/core/section/section.h"

namespace N_Core
{
	namespace N_Section
	{
		namespace N_Filters
		{
			//tags to select a filter.
			struct Code {};
			struct Data {};
			struct SymbolTable {};

			namespace __Detail__
			{
				template <typename ElfTy, typename TagTy>
				struct Filter 
				{
				};

				template <typename ElfTy>
				struct Filter<ElfTy, Code>
				{
					bool operator()(typename ElfTy::SectionTy const& section) { return section.get_type() == N_Section::Type::SHT_PROGBITS && section.get_flags().SHF_ALLOC && section.get_flags().SHF_EXECINSTR; };
				};

				template <typename ElfTy>
				struct Filter<ElfTy, SymbolTable>
				{
					bool operator()(typename ElfTy::SectionTy const& section) { return section.get_type() == N_Section::Type::SHT_SYMTAB; };
				};
			}

/*
			template <typename SectionTy>
			struct Data
			{
				bool operator()(SectionTy const& section) { return section.get_type() == N_Section::Type::SHT_PROGBITS && section.get_flags().SHF_ALLOC && !section.get_flags().SHF_EXECINSTR; };
			};

			template <typename SectionTy>
			struct SymbolTable
			{
				bool operator()(SectionTy const& section) { return section.get_type() == N_Section::Type::SHT_SYMTAB; };
			};*/
		}
	}
}