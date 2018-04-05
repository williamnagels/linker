#include "src/include/core/linker/link_struct.h"
#include "src/include/core/linker/linker.h"
#include "src/include/core/elf.h"

#include "src/include/core/section/helpers.h"
#include <boost/test/unit_test.hpp> 
#include <range/v3/view.hpp>
#include <range/v3/all.hpp>
#include <variant>
#include <utility>
#include <set>
#include <functional>
#include <vector>
BOOST_AUTO_TEST_SUITE(link_struct_unittest)

// Each section id that is defined should be exposed
//
BOOST_AUTO_TEST_CASE(convert_to_link_structs_simple)
{
	N_Core::N_Linker::Linker<N_Core::Bit64> linker("", {"testfiles/data_empty_bss_global_and_local_symbol"});

	linker.do_link();

	BOOST_CHECK_EQUAL(linker._input_elfs.size(), 1);
	BOOST_CHECK_EQUAL(linker._segment_builders[0]._segment._sections.size(), 1);
	BOOST_CHECK_EQUAL(linker._segment_builders[1]._segment._sections.size(), 1);
	BOOST_CHECK_EQUAL(linker._segment_builders[0]._segment._sections.begin()->get().get_name_as_string() == ".text", true);
	BOOST_CHECK_EQUAL(linker._segment_builders[1]._segment._sections.begin()->get().get_name_as_string() == ".data", true);
}


// Each section id that is defined should be exposed
//
BOOST_AUTO_TEST_CASE(convert_to_link_structs_simpler)
{
	N_Core::N_Linker::Linker<N_Core::Bit64> linker("", {"testfiles/simple_main_no_data"});

	linker.do_link();

	BOOST_CHECK_EQUAL(linker._input_elfs.size(), 1);
	BOOST_CHECK_EQUAL(linker._segment_builders[0]._segment._sections.size(), 1);
	BOOST_CHECK_EQUAL(linker._segment_builders[1]._segment._sections.size(), 1); // this data is actually empty
	BOOST_CHECK_EQUAL(linker._segment_builders[0]._segment._sections.begin()->get().get_name_as_string() == ".text", true);
	BOOST_CHECK_EQUAL(linker._segment_builders[0]._segment._sections.begin()->get().get_address(), 100000);

	
	BOOST_CHECK_EQUAL(linker._entry_value, 100000);
}

BOOST_AUTO_TEST_SUITE_END()