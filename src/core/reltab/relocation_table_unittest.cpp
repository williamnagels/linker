#include <boost/test/unit_test.hpp> 
#include "src/include/core/elf.h"
#include <variant>
#include <utility>
BOOST_AUTO_TEST_SUITE(relocation_table)

// data_empty_bss_global_and_local_symbol only contains 1 symbol table
// 
// 5th symbol: 'local_symbol'
// 7th symbol: 'global_symbol'
//10th symbol: 'Log'
//11th symbol: 'Log2'
//
BOOST_AUTO_TEST_CASE(single_relocation_parse)
{
}
BOOST_AUTO_TEST_SUITE_END()