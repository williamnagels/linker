#include <boost/test/unit_test.hpp> 
#include <boost/filesystem.hpp>
#include "src/include/core/elf.h"
#include <algorithm>
BOOST_AUTO_TEST_SUITE(elf_section_table)

/*
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
		N_Core::VoidIterator<>(),
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

			return N_Core::VoidIterator<>::value_type();
		}
	);

}

BOOST_AUTO_TEST_CASE(correct_section_content_after_dump)
{
	auto elf = N_Core::create_elf("testfiles/sleep");
	N_Core::dump_to_file("testfiles/correct_content_when_dumping_table", N_Core::create_elf(elf));
	auto elf2 = N_Core::create_elf("testfiles/correct_content_when_dumping_table");
	BOOST_CHECK_EQUAL(elf._header->get_section_header_number_of_entries(), elf2._header->get_section_header_number_of_entries());

	std::transform(
		elf._section_table._sections.begin(),
		elf._section_table._sections.end(),
		elf2._section_table._sections.begin(),
		N_Core::VoidIterator<>(),
		[](auto const& section_elf_1, auto const& section_elf_2)
		{
			N_Core::BinaryBlob binary_blob_elf_1 = section_elf_1->get_content();
			N_Core::BinaryBlob binary_blob_elf_2 = section_elf_2->get_content();

			BOOST_CHECK_EQUAL(section_elf_1->get_size(), section_elf_2->get_size());
			BOOST_CHECK_EQUAL(binary_blob_elf_1.size(), binary_blob_elf_2.size());
			BOOST_CHECK_EQUAL(section_elf_1->get_size(), binary_blob_elf_2.size());
			BOOST_CHECK_EQUAL(std::memcmp(binary_blob_elf_1.begin(), binary_blob_elf_2.begin(), binary_blob_elf_1.size()), 0);
			return N_Core::VoidIterator<>::value_type();
		}
	);
}

BOOST_AUTO_TEST_CASE(size_in_header_and_in_memory_sanity_check)
{
	auto size_of_section_21 = 0x1d0;
	auto elf = N_Core::create_elf("testfiles/sleep");
	
	BOOST_CHECK_EQUAL(elf._section_table._sections[21]->get_size(), size_of_section_21);
	BOOST_CHECK_EQUAL(elf._section_table._sections[21]->get_content().size(), size_of_section_21);
}


/*
BOOST_AUTO_TEST_CASE(remove_section)
{
	auto elf_to_remove_section_from = N_Core::create_elf("testfiles/sleep");
	auto copy_of_original = N_Core::create_elf(elf_to_remove_section_from);

	BOOST_CHECK_EQUAL(
		elf_to_remove_section_from._header->get_section_header_number_of_entries(),
		copy_of_original._header->get_section_header_number_of_entries()
	);

	N_Core::dump_to_file("testfiles/remove_section", N_Core::create_elf(elf_to_remove_section_from));

	auto index_of_section_to_remove = 6;
	auto size_of_removed_section_check = 0x289;
	auto size_of_removed_section = elf_to_remove_section_from._section_table._sections[index_of_section_to_remove]->get_size();
	BOOST_CHECK_EQUAL(size_of_removed_section_check, size_of_removed_section);

	elf_to_remove_section_from.remove_section(index_of_section_to_remove);

	BOOST_CHECK_EQUAL(
		elf_to_remove_section_from._header->get_section_header_number_of_entries(),
		copy_of_original._header->get_section_header_number_of_entries() - 1
	);

	N_Core::dump_to_file("testfiles/remove_section", N_Core::create_elf(elf_to_remove_section_from));

	auto verification_elf = N_Core::create_elf("testfiles/remove_section");

	BOOST_CHECK_EQUAL(
		verification_elf._header->get_section_header_number_of_entries(),
		copy_of_original._header->get_section_header_number_of_entries() - 1
	);

	BOOST_CHECK_EQUAL(
		boost::filesystem::file_size("testfiles/remove_section"), 
		boost::filesystem::file_size("testfiles/sleep") - sizeof(N_Core::N_Section::Elf64_Shdr) - size_of_removed_section
	);	
}*/
/*
BOOST_AUTO_TEST_CASE(remove_all_sections_from_elf)
{
	auto elf_under_test = N_Core::create_elf("testfiles/sleep");
	auto size_with_section = boost::filesystem::file_size("testfiles/sleep");
	auto number_of_sections_in_original_elf = elf_under_test._header->get_section_header_number_of_entries();

	std::string path_for_this_iteration = "testfiles/sleep_rebuild";
	N_Core::dump_to_file(path_for_this_iteration, N_Core::create_elf("testfiles/sleep"));

	for (auto i = 0; i < number_of_sections_in_original_elf; i++)
	{
		auto elf_to_remove_section_from = N_Core::create_elf(path_for_this_iteration.c_str());
		auto size_of_removed_section = elf_to_remove_section_from._section_table._sections[0]->get_size();
		elf_to_remove_section_from.remove_section(0);

		path_for_this_iteration = std::string("testfiles/remove_all_sections_from_elf_iteration_") + std::to_string(i);

		N_Core::dump_to_file(path_for_this_iteration, elf_to_remove_section_from);

		auto size_without_section = boost::filesystem::file_size(path_for_this_iteration);

		BOOST_CHECK_EQUAL(
			size_with_section - size_of_removed_section - sizeof(N_Core::N_Section::Elf64_Shdr),
			size_without_section
		);

		size_with_section = size_without_section;
	}

	//finally only elf header should be left.
	auto size_without_any_sections = size_with_section;
	BOOST_CHECK_EQUAL(
		sizeof(N_Core::N_Header::Elf64_Ehdr),
		size_without_any_sections
	);¨

}*/



BOOST_AUTO_TEST_CASE(remove_first_section)
{
	std::string path_for_this_iteration = "testfiles/remove_first_section";

	auto elf_to_remove_section_from = N_Core::create_elf("testfiles/sleep");
	elf_to_remove_section_from.remove_section(0);
	N_Core::dump_to_file(path_for_this_iteration, elf_to_remove_section_from);
	auto elf_to_remove_section_from_2 = N_Core::create_elf(path_for_this_iteration.c_str());


	auto a = 0;
}

BOOST_AUTO_TEST_SUITE_END()