#include <boost/test/unit_test.hpp> 
#include "src/include/core/elf.h"
#include "src/include/core/symtab/filters.h"
#include <variant>
#include <utility>
BOOST_AUTO_TEST_SUITE(symbol_table)

// data_empty_bss_global_and_local_symbol only contains 1 symbol table
// 
// 5th symbol: 'local_symbol'
// 7th symbol: 'global_symbol'
//10th symbol: 'Log'
//11th symbol: 'Log2'
//
BOOST_AUTO_TEST_CASE(single_symbol_table_parse_renewed)
{
	N_Core::Elf<N_Core::Bit64> elf("testfiles/data_empty_bss_global_and_local_symbol");

	auto iterator = std::find_if(elf.begin_symbol()
		, elf.end_symbol()
		, [](auto const& i) {return i.get_name_as_string()=="local_symbol"; });
	BOOST_CHECK_EQUAL(std::distance(elf.begin_symbol(), iterator), 5);

	iterator = std::find_if(elf.begin_symbol()
		, elf.end_symbol()
		, [](auto const& i) {return i.get_name_as_string() == "global_symbol"; });
	BOOST_CHECK_EQUAL(std::distance(elf.begin_symbol(), iterator), 8);

	iterator = std::find_if(elf.begin_symbol()
		, elf.end_symbol()
		, [](auto const& i) {return i.get_name_as_string() == "Log"; });
	BOOST_CHECK_EQUAL(std::distance(elf.begin_symbol(), iterator),9);

	iterator = std::find_if(elf.begin_symbol()
		, elf.end_symbol()
		, [](auto const& i) {return i.get_name_as_string() == "Log2"; });
	BOOST_CHECK_EQUAL(std::distance(elf.begin_symbol(), iterator), 10);
	
	BOOST_CHECK_NE(std::distance(iterator, elf.end_symbol()), 0);
}

// iterate over global symbols.
// 
// 1st global symbol: 'global_symbol'
// 2nd global symbol: 'Log'
// 3th global symbol: 'Log2'
//
BOOST_AUTO_TEST_CASE(global_symbols)
{
	N_Core::Elf<N_Core::Bit64> elf("testfiles/data_empty_bss_global_and_local_symbol");


	auto iterator = std::find_if(elf.begin_symbol()
		, elf.end_symbol()
		, [](auto const& symbol) {return symbol.is_global(); });

	BOOST_CHECK_EQUAL(iterator->get_name_as_string() == "global_symbol", true);
	iterator = std::find_if(
		std::next(iterator)
		, elf.end_symbol()
		, [](auto const& symbol) {return symbol.is_global(); });

	BOOST_CHECK_EQUAL(iterator->get_name_as_string() == "Log", true);

	iterator = std::find_if(
		std::next(iterator)
		, elf.end_symbol()
		, [](auto const& symbol) {return symbol.is_global(); });
	BOOST_CHECK_EQUAL(iterator->get_name_as_string() == "Log2", true);

	iterator = std::find_if(
		std::next(iterator)
		, elf.end_symbol()
		, [](auto const& symbol) {return symbol.is_global(); });

	BOOST_CHECK_EQUAL(std::distance(iterator, elf.end_symbol()), 0);
}


BOOST_AUTO_TEST_CASE(determine_sections_to_link_for_simple_translation_unit)
{
	N_Core::Elf<N_Core::Bit64> elf("testfiles/data_empty_bss_global_and_local_symbol");

	auto range = elf.range(N_Core::N_Filters::N_Symbol::Global<decltype(elf)>{});

	int number_of_global_symbols = std::distance(range.first, range.second);

	BOOST_CHECK_EQUAL(number_of_global_symbols, 3);

}
BOOST_AUTO_TEST_SUITE_END()