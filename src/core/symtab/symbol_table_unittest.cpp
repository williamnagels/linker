#include <boost/test/unit_test.hpp> 
#include "src/include/core/elf.h"

BOOST_AUTO_TEST_SUITE(symbol_table)

 
BOOST_AUTO_TEST_CASE(single_symbol_table_parse)
{
	auto elf = N_Core::create_elf<N_Core::Bit64>("testfiles/data_empty_bss_global_and_local_symbol");

	auto i = 0;
}
BOOST_AUTO_TEST_SUITE_END()