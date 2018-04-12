#pragma once
#include <cstdint>
#include <string>
#include <range/v3/view/all.hpp>
namespace N_Core
{
	namespace N_Section
	{
		static const std::string invalid_section_index = "No section could be found with the index";

		// This will convert a section of any type to an symbol range.
		// This function will throw std::bad_variant_access if
		// section does not contain a symbol table.
		struct ConvertSectionToSymbolRange
		{
			ConvertSectionToSymbolRange(){}

			template <typename SectionTy>
			auto operator()(SectionTy const& section) const -> typename SectionTy::SymbolTableTy::InternalStorageTy const&
			{
				return std::get<1>(section.get_interpreted_content())._symbols;
			}
		};

		// This will convert a section of any type to an relocation range.
		// This function will throw std::bad_variant_access if
		// section does not contain a relocation table.
		struct ConvertSectionToRelocationRange
		{
			ConvertSectionToRelocationRange(){}

			template <typename SectionTy>
			auto operator()(SectionTy const& section) const -> typename SectionTy::RelocationTableTy::InternalStorageTy const&
			{
				return std::get<2>(section.get_interpreted_content())._relocations;
			}
		};
	}
}
