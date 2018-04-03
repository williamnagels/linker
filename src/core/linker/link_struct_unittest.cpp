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
	auto b = 0;
	//N_Core::Elf<N_Core::Bit64, N_Core::N_Linker::MetaData> elf("testfiles/data_empty_bss_global_and_local_symbol");	

	// uint64_t a,b,c,d;
	// auto principal_sections = N_Core::N_Linker::build_principal_sections(elf, a,b,c,d);
	// BOOST_CHECK_EQUAL(principal_sections.size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()