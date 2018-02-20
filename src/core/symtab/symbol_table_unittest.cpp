#include <boost/test/unit_test.hpp> 
#include "src/include/core/elf.h"
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
	BOOST_CHECK_EQUAL(std::distance(elf.begin_symbol(), iterator), 9);

	iterator = std::find_if(elf.begin_symbol()
		, elf.end_symbol()
		, [](auto const& i) {return i.get_name_as_string() == "Log"; });
	BOOST_CHECK_EQUAL(std::distance(elf.begin_symbol(), iterator), 10);

	iterator = std::find_if(elf.begin_symbol()
		, elf.end_symbol()
		, [](auto const& i) {return i.get_name_as_string() == "Log2"; });
	BOOST_CHECK_EQUAL(std::distance(elf.begin_symbol(), iterator), 11);
}
BOOST_AUTO_TEST_SUITE_END()