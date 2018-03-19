
namespace N_Core
{
	namespace N_Symbol
	{
		// This will convert a section of any type to an symbol range.
		// This function will throw std::bad_variant_access if
		// section does not contain a symbol table.
		struct convert_symbol_to_related_section
		{
			ConvertSectionToSymbolRange(){}

			template <typename SectionTy>
			auto operator()(SectionTy const& section) const
			{
				return ranges::view::all(std::get<1>(section.get_interpreted_content())._symbols);
			}
		};
    
    }
}