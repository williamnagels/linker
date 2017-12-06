#include <boost/test/unit_test.hpp> 
#include "src/include/core/elf.h"
BOOST_AUTO_TEST_SUITE(elf_section_table)




// Not being able to parse elf header (magic byte = wrong)
BOOST_AUTO_TEST_CASE(correct_amount_of_sections)
{
	auto elf = N_Core::create_elf("testfiles/sleep");


//	BOOST_CHECK_EQUAL(elf._section_table._sections.size(), 0);
	

}

BOOST_AUTO_TEST_SUITE_END()