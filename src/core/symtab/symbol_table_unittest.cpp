#include <boost/test/unit_test.hpp> 
#include "src/include/core/elf.h"
#include <variant>
#include <utility>
BOOST_AUTO_TEST_SUITE(symbol_table)

 
BOOST_AUTO_TEST_CASE(single_symbol_table_parse)
{
	auto elf = N_Core::create_elf<N_Core::Bit64>("testfiles/data_empty_bss_global_and_local_symbol");

	std::for_each(
		elf._section_table.begin_symbol_table(),
		elf._section_table.end_symbol_table(),
		[](auto const& symbol_table_section) 
		{
			auto const& interpreted_content = symbol_table_section.get_interpreted_content();
			
			auto const& symbol_table = std::get <1> (interpreted_content);
			
			std::for_each(
				std::begin(symbol_table), 
				std::end(symbol_table),
			[](auto const& symbol)
			{
				auto name = *symbol.get_name_as_string();
			});
		});
}
BOOST_AUTO_TEST_SUITE_END()