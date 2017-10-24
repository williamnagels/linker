#include <boost/test/unit_test.hpp> 

BOOST_AUTO_TEST_SUITE(symbol_table)

void single_test(int i)
{
	BOOST_TEST( 1 == 0);
}

void combined_test()
{
	int params[] = { 1, 2, 3, 4, 5 };
	std::for_each(params, params + 5, &single_test);
}


BOOST_AUTO_TEST_CASE(basic_symbol)
{
	combined_test();
}
BOOST_AUTO_TEST_SUITE_END()