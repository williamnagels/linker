#include "src/include/core/elf.h"
#include "src/include/core/symtab/filters.h"
#include "src/include/core/section/helpers.h"
#include <boost/test/unit_test.hpp> 
#include <range/v3/all.hpp>

#include <variant>
#include <utility>
#include <set>

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
BOOST_AUTO_TEST_SUITE_END()