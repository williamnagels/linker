#include <boost/test/unit_test.hpp> 
#include "src/include/core/elf.h"
#include <algorithm>
BOOST_AUTO_TEST_SUITE(elf_section_table)


BOOST_AUTO_TEST_CASE(correct_amount_of_sections)
{
	auto elf = N_Core::create_elf("testfiles/sleep");
	BOOST_CHECK_EQUAL(elf._section_table._sections.size(), 27);

	auto elf2(elf);
	BOOST_CHECK_EQUAL(elf2._section_table._sections.size(), 27);

	N_Core::dump_to_file("testfiles/correct_amount_of_sections", elf2);
	
	auto elf3 = N_Core::create_elf("testfiles/correct_amount_of_sections");
	BOOST_CHECK_EQUAL(elf3._section_table._sections.size(), 27);

}
BOOST_AUTO_TEST_CASE(correct_section_header_after_dump)
{
	auto elf = N_Core::create_elf("testfiles/sleep");
	N_Core::dump_to_file("testfiles/correct_header_after_dump", elf);
	auto elf2 = N_Core::create_elf("testfiles/correct_header_after_dump");

	std::transform(
		elf._section_table._sections.begin(),
		elf._section_table._sections.end(),
		elf2._section_table._sections.begin(),
		N_Core::VoidStorage<>(),
		[](auto const& section_elf_1, auto const& section_elf_2)
		{
			BOOST_CHECK_EQUAL(section_elf_1->get_name(), section_elf_2->get_name());
			BOOST_CHECK_EQUAL(section_elf_1->get_type(), section_elf_2->get_type());
			BOOST_CHECK_EQUAL(section_elf_1->get_flags(), section_elf_2->get_flags());
			BOOST_CHECK_EQUAL(section_elf_1->get_address(), section_elf_2->get_address());
			BOOST_CHECK_EQUAL(section_elf_1->get_offset(), section_elf_2->get_offset());
			BOOST_CHECK_EQUAL(section_elf_1->get_size(), section_elf_2->get_size());
			BOOST_CHECK_EQUAL(section_elf_1->get_link(), section_elf_2->get_link());
			BOOST_CHECK_EQUAL(section_elf_1->get_info(), section_elf_2->get_info());
			BOOST_CHECK_EQUAL(section_elf_1->get_address_alignment(), section_elf_2->get_address_alignment());
			BOOST_CHECK_EQUAL(section_elf_1->get_entry_size(), section_elf_2->get_entry_size());

			return N_Core::VoidStorage<>::Type();
		}
	);

}

BOOST_AUTO_TEST_CASE(correct_section_content_after_dump)
{
	auto elf = N_Core::create_elf("testfiles/sleep");
	N_Core::dump_to_file("testfiles/correct_content_when_dumping_table", N_Core::create_elf(elf));
	auto elf2 = N_Core::create_elf("testfiles/correct_content_when_dumping_table");

	std::transform(
		elf._section_table._sections.begin(),
		elf._section_table._sections.end(), 
		elf2._section_table._sections.begin(), 
		N_Core::VoidStorage<>(),
		[](auto const& section_elf_1, auto const& section_elf_2)
		{
			N_Core::BinaryBlob binary_blob_elf_1 = section_elf_1->get_content();
			N_Core::BinaryBlob binary_blob_elf_2 = section_elf_2->get_content();
			std::ptrdiff_t diff = binary_blob_elf_1.end() - binary_blob_elf_1.begin();
			std::ptrdiff_t diff2 = binary_blob_elf_2.end() - binary_blob_elf_2.begin();
			BOOST_CHECK_EQUAL(diff, diff2);
			BOOST_CHECK_EQUAL(std::memcmp(binary_blob_elf_1.begin(), binary_blob_elf_2.begin(), diff), 0);
			return N_Core::VoidStorage<>::Type();
		}
	);
}

BOOST_AUTO_TEST_SUITE_END()