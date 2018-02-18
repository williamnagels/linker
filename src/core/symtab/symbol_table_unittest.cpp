#include <boost/test/unit_test.hpp> 
#include "src/include/core/elf.h"
#include <variant>
#include <utility>
BOOST_AUTO_TEST_SUITE(symbol_table)


BOOST_AUTO_TEST_CASE(single_symbol_table_parse)
{
	N_Core::Elf<N_Core::Bit64> elf("testfiles/data_empty_bss_global_and_local_symbol");

	auto symbol_table_iterator = elf._section_table.begin_symbol_table();

	for (; symbol_table_iterator != elf._section_table.end_symbol_table(); symbol_table_iterator++)
	{
		auto const& interpreted_content = symbol_table_iterator->get_interpreted_content();
		auto const& symbol_table = std::get <1>(interpreted_content);

		auto symbol_iterator = std::begin(symbol_table);
		for (; symbol_iterator != std::end(symbol_table); symbol_iterator++)
		{
			auto name = *symbol_iterator->get_name_as_string();
			auto i = 0;
		}
	}
}


BOOST_AUTO_TEST_CASE(single_symbol_table_parse_renewed)
{
	N_Core::Elf<N_Core::Bit64> elf("testfiles/data_empty_bss_global_and_local_symbol");

	auto symbol_iterator = elf._section_table.begin_symbol();

	for (; symbol_iterator != elf._section_table.end_symbol(); symbol_iterator++)
	{
	}
}
BOOST_AUTO_TEST_SUITE_END()