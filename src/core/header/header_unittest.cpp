#include <boost/test/unit_test.hpp> 

#include "src/include/core/header/header.h"

#include "src/include/core/elf.h"

BOOST_AUTO_TEST_SUITE(elf_header)


BOOST_AUTO_TEST_CASE(basic_64_bit_header)
{
	BOOST_TEST(1 == 1);
	//N_Core::create_elf_from_path_to_file("testfiles/basic_elf");
}
BOOST_AUTO_TEST_SUITE_END()