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
BOOST_AUTO_TEST_SUITE(symbol_table)

BOOST_AUTO_TEST_CASE(ranges_simple)
{
	N_Core::Elf<N_Core::Bit64> elf("testfiles/data_empty_bss_global_and_local_symbol");	

	auto symbol_range = elf._section_table 
		| ranges::view::filter(N_Core::N_Section::N_Filters::SymbolTable{})
		| ranges::view::transform(N_Core::N_Section::ConvertSectionToSymbolRange{})
		| ranges::view::join;

	auto number_of_symbols = ranges::distance(symbol_range);

	BOOST_CHECK_EQUAL(number_of_symbols, 11);
}

// There are expected to be 3 global symbols present in data_empty_bss_global_and_local_symbol
//
BOOST_AUTO_TEST_CASE(global_symbols)
{
	N_Core::Elf<N_Core::Bit64> elf("testfiles/data_empty_bss_global_and_local_symbol");	

	auto symbol_range = elf._section_table 
		| ranges::view::filter(N_Core::N_Section::N_Filters::SymbolTable{})
		| ranges::view::transform(N_Core::N_Section::ConvertSectionToSymbolRange{}) 
		| ranges::view::join
		| ranges::view::filter(N_Core::N_Symbol::N_Filters::Global{});

	auto number_of_symbols = ranges::distance(symbol_range);

	BOOST_CHECK_EQUAL(number_of_symbols, 3);
}

// There are expected to be 3 global symbols present in data_empty_bss_global_and_local_symbol
//
BOOST_AUTO_TEST_CASE(global_symbols_exposed)
{
	N_Core::Elf<N_Core::Bit64> elf("testfiles/data_empty_bss_global_and_local_symbol");	

	auto symbol_range = elf._section_table 
		| ranges::view::filter(N_Core::N_Section::N_Filters::SymbolTable{})
		| ranges::view::transform(N_Core::N_Section::ConvertSectionToSymbolRange{}) 
		| ranges::view::join
		| ranges::view::filter(N_Core::N_Symbol::N_Filters::Global{});

	auto number_of_symbols = ranges::distance(symbol_range);

	BOOST_CHECK_EQUAL(number_of_symbols, 3);
}

// Each section id that is defined should be exposed
//
BOOST_AUTO_TEST_CASE(section_names_from_defined_global_symbols)
{
	N_Core::Elf<N_Core::Bit64> elf("testfiles/data_empty_bss_global_and_local_symbol");	

	auto sections = elf._section_table
		| ranges::view::filter(N_Core::N_Section::N_Filters::SymbolTable{})
		| ranges::view::transform(N_Core::N_Section::ConvertSectionToSymbolRange{})
		| ranges::view::join
		| ranges::view::filter(N_Core::N_Symbol::N_Filters::Global{})
		| ranges::view::filter(N_Core::N_Symbol::N_Filters::Defined{})
		| ranges::view::transform(N_Core::ConvertSymbolToSection{})
		| ranges::view::transform([](auto const& section){return *section.get_name_as_string();});

	BOOST_CHECK_EQUAL(ranges::equal(sections, {".data",".text",".text"}), true);
}

BOOST_AUTO_TEST_SUITE_END()