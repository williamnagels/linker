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

BOOST_AUTO_TEST_CASE(simple_main_global_symbol)
{
	N_Core::N_Linker::Linker<N_Core::Bit64> linker("", {"testfiles/weak_advanced_main","testfiles/weak_advanced_ext"}); 

	linker.do_link("testfiles/simple_main_weak_advanced_symbol_linked");
}
BOOST_AUTO_TEST_SUITE_END()