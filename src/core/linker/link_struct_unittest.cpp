#include "src/include/core/linker/link_struct.h"
#include "src/include/core/elf.h"
#include "src/include/core/symtab/filters.h"
#include "src/include/core/section/helpers.h"
#include <boost/test/unit_test.hpp> 
#include <range/v3/view.hpp>
#include <range/v3/all.hpp>
#include <variant>
#include <utility>
#include <set>
#include <functional>
#include <vector>
BOOST_AUTO_TEST_SUITE(link_struct_unittest)

// Each section id that is defined should be exposed
//
BOOST_AUTO_TEST_CASE(convert_to_link_structs_simple)
{
	N_Core::Elf<N_Core::Bit64> elf("testfiles/data_empty_bss_global_and_local_symbol");	

	uint64_t code, code_offset, data, data_offset;
	std::set<N_Core::N_Linker::Section<decltype(elf)::SectionTy>> sections = elf._section_table
		  | ranges::view::filter(N_Core::N_Section::N_Filters::SymbolTable{})
		  | ranges::view::transform(N_Core::N_Section::ConvertSectionToSymbolRange{})
		  | ranges::view::join
		  | ranges::view::filter(N_Core::N_Symbol::N_Filters::Global{})
		  | ranges::view::filter(N_Core::N_Symbol::N_Filters::Defined{})
		  | ranges::view::transform(N_Core::N_Linker::ConvertSymbolToLinkSection{code, code_offset, data, data_offset});

	BOOST_CHECK_EQUAL(sections.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()