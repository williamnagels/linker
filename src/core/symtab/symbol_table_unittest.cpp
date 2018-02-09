#include <boost/test/unit_test.hpp> 
#include "src/include/core/elf.h"

BOOST_AUTO_TEST_SUITE(symbol_table)

void single_test(int i)
{
	BOOST_TEST( 1 == 1);
}

void combined_test()
{
	int params[] = { 1, 2, 3, 4, 5 };
	std::for_each(params, params + 5, &single_test);
}

 
BOOST_AUTO_TEST_CASE(basic_symbol)
{
	auto elf = N_Core::create_elf<N_Core::Bit64>("testfiles/global_and_local_symbol");
}
BOOST_AUTO_TEST_SUITE_END()