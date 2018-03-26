#include "src/include/core/elf.h"
#include "src/include/core/symtab/filters.h"
#include "src/include/core/section/helpers.h"
#include <set>
#include <range/v3/view.hpp>
namespace N_Core
{
    namespace N_Linker
    {
        //@brief Extract all sections that export a global symbol.
        template <typename T>
        auto build_principal_sections(T const& elf, uint64_t const& code, uint64_t const& code_offset, uint64_t const& data, uint64_t const& data_offset)
        {
            std::set<N_Core::N_Linker::Section<typename T::SectionTy>> sections = elf._section_table
                | ranges::view::filter(N_Core::N_Section::N_Filters::SymbolTable{})
                | ranges::view::transform(N_Core::N_Section::ConvertSectionToSymbolRange{})
                | ranges::view::join
                | ranges::view::filter(N_Core::N_Symbol::N_Filters::Global{})
                | ranges::view::filter(N_Core::N_Symbol::N_Filters::Defined{})
                | ranges::view::transform(N_Core::N_Linker::ConvertSymbolToLinkSection{code, code_offset, data, data_offset});
            return sections;
        }

        //@brief Extract all sections that export a global symbol.
        // 1. Create container of all symbols that expose a global symbol
        // 2. Convert the global symbol to a section using the symbol info.
        // 3. Store the section in the linker with the linker symbol.
        template <typename T, typename L>
        void prepare_elf(T const& elf, L& linker)
        {
            elf._section_table
                | ranges::view::filter(N_Core::N_Section::N_Filters::SymbolTable{})
                | ranges::view::transform(N_Core::N_Section::ConvertSectionToSymbolRange{})
                | ranges::view::join
                | ranges::view::filter(N_Core::N_Symbol::N_Filters::Global{})
                | ranges::view::filter(N_Core::N_Symbol::N_Filters::Defined{})
                | ranges::view::for_each(
                    [L& linker](auto const& symbol) 
                    {  
                        auto const& section = add_global_symbol(linker, elf, symbol_to_section);
                        linker.emplace(symbol, section);
                    });
        }


        //@brief Add a section identfied by a global symbol to the container.
        // 1. Check if symbol is known to the linker; if so, return existing ref
        // 2. If the global symbol is unknown convert the section and store;
        // 3. Obtain any additional sections that may have to be stored. Like local sections that are required.
        // 4. Return ref of the newly stored section.
        template <typename T, typename C>
        void add_global_symbol(T const& elf, C& container)
        {
           C sections = elf._section_table
                | ranges::view::filter(N_Core::N_Section::N_Filters::RelocationTable{container})
                | ranges::view::transform(N_Core::N_Section::ConvertSectionToRelocationRange{})
                | ranges::view::join
                | ranges::view::transform(N_Core::N_Relocation::ConvertRelocationToSymbol{})
                | ranges::view::transform(N_Core::N_Linker::ConvertSymbolToLinkSection{code, code_offset, data, data_offset});

            C combined_sections = ranges::v3::set_union(container, sections);
            
            if (ranges::v3::count(combined_sections) == ranges::v3::count(sections))
            {
                return container;
            }
            return iterate_over_collected_sections(elf, combined_sections);
        }
    }
}