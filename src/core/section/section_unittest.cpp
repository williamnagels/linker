#include <boost/test/unit_test.hpp> 
#include <boost/filesystem.hpp>
#include "src/include/core/elf.h"
#include "src/include/core/section/section.h"
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

			std::size_t size_section_blob_elf_1 = section_elf_1->get_content().get_size();
			std::size_t size_section_blob_elf_2 = section_elf_2->get_content().get_size();


			BOOST_CHECK_EQUAL(section_elf_1->get_size(), section_elf_2->get_size());
			BOOST_CHECK_EQUAL(size_section_blob_elf_1, size_section_blob_elf_2);
			BOOST_CHECK_EQUAL(section_elf_1->get_size(), size_section_blob_elf_2);

			int result = std::memcmp(
				&(*std::begin(section_elf_1->get_content())),
				&(*std::begin(section_elf_2->get_content())),
				size_section_blob_elf_1);

			BOOST_CHECK_EQUAL(result, 0);
			return N_Core::VoidIterator<>::value_type();
		}
	);
}

BOOST_AUTO_TEST_CASE(size_in_header_and_in_memory_sanity_check)
{
	auto size_of_section_21 = 0x1d0;
	auto elf = N_Core::create_elf("testfiles/sleep");
	
	BOOST_CHECK_EQUAL(elf._section_table.get_section_at_index(21).get_size(), size_of_section_21);
	//BOOST_CHECK_EQUAL(elf._section_table._sections[21]->get_content().size(), size_of_section_21);
}



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

	elf_to_remove_section_from.remove_section(index_of_section_to_remove, N_Core::N_Section::SectionRemovalPolicy::COMPACT);

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
}

BOOST_AUTO_TEST_CASE(remove_all_sections_from_elf_beginning_at_start)
{
	auto elf_under_test = N_Core::create_elf("testfiles/sleep");
	auto size_with_section = boost::filesystem::file_size("testfiles/sleep");
	auto number_of_sections_in_original_elf = elf_under_test._header->get_section_header_number_of_entries();

	auto tot_size_of_program_headers = elf_under_test._header->get_program_header_number_of_entries() * elf_under_test._header->get_program_header_entry_size();
	
	std::string path_for_this_iteration = "testfiles/sleep_rebuild";
	N_Core::dump_to_file(path_for_this_iteration, N_Core::create_elf("testfiles/sleep"));

	for (auto i = 0; i < number_of_sections_in_original_elf; i++)
	{
		auto elf_to_remove_section_from = N_Core::create_elf(path_for_this_iteration.c_str());
		auto size_of_removed_section = elf_to_remove_section_from._section_table._sections[0]->get_size_in_file();

		elf_to_remove_section_from.remove_section(0, N_Core::N_Section::SectionRemovalPolicy::COMPACT);

		path_for_this_iteration = std::string("testfiles/remove_all_sections_from_elf_iteration_") + std::to_string(i);

		N_Core::dump_to_file(path_for_this_iteration, elf_to_remove_section_from);

		auto size_without_section = boost::filesystem::file_size(path_for_this_iteration);

		// This check only works for all except the last section removed.
		// For one, the program header is still present and padding(sections are not always glued together there may be spacing (in file size) between them)
		// between the sections will have been accumulated ensuring that when all but one sections has been removed there is a gap
		// between the elf header and the first sections.
		// to ensure that when the elf is memory mapped sections can be loaded into memory directly and do not need copying to the correct address (see page alignment).
		// For example
		//
		// [HEADER][SEGMENT TABLE][S1][PADDING1][S2][PADDING2][SECTION TABLE]
		//
		// Remove S1
		//
		// [HEADER][SEGMENT TABLE][PADDING1][S2][PADDING2][SECTION TABLE]
		//
		// Note S1 padding is still present. This padding is actually present in the offset of S2.
		// offset of S2 = offset S1 + size S1 + padding between S1 and S2. 
		// Remove 2
		//
		// [HEADER][SEGMENT TABLE][PADDING1][PADDING2][SECTION TABLE]
		//
		// Note S1 and S2 padding. thsi padding is present in the offset of the section header table.
		//
		// However; if the section table contains no entries it will not be dumped to file and thus the offset is
		// ignored and all padding spacing and segment table spacing is ignored. leading to an elf with a header only.
		if (i != number_of_sections_in_original_elf - 1)
		{
			BOOST_CHECK_EQUAL(
				size_with_section - size_of_removed_section - sizeof(N_Core::N_Section::Elf64_Shdr),
				size_without_section
			);
		} 

		size_with_section = size_without_section;
	}

	//finally only elf header should be left.
	auto size_without_any_sections = size_with_section;
	BOOST_CHECK_EQUAL(
		sizeof(N_Core::N_Header::Elf64_Ehdr),
		size_without_any_sections
	);

}

BOOST_AUTO_TEST_CASE(remove_first_section)
{
	std::string path_for_this_iteration = "testfiles/remove_first_section";

	auto elf_to_remove_section_from = N_Core::create_elf("testfiles/sleep");

	BOOST_CHECK_EQUAL(
		elf_to_remove_section_from._section_table._sections.at(0).get()->get_type()
		, N_Core::N_Section::Type::SHT_NULL
	);

	elf_to_remove_section_from.remove_section(0, N_Core::N_Section::SectionRemovalPolicy::COMPACT);

	for (auto const& section : elf_to_remove_section_from._section_table._sections)
	{
		if (section->get_offset() + section->get_size() > elf_to_remove_section_from._header->get_section_header_offset())
		{
			auto b = 0;
		}
	}

	BOOST_CHECK_EQUAL(
		elf_to_remove_section_from._section_table._sections.at(0).get()->get_type()
		, N_Core::N_Section::Type::SHT_PROGBITS
	);
	
	N_Core::dump_to_file(path_for_this_iteration, elf_to_remove_section_from);
	auto elf_to_remove_section_from_2 = N_Core::create_elf(path_for_this_iteration.c_str());

	BOOST_CHECK_EQUAL(
		elf_to_remove_section_from_2._section_table._sections.at(0).get()->get_type()
		, N_Core::N_Section::Type::SHT_PROGBITS
	);
}

BOOST_AUTO_TEST_CASE(remove_all_sections_from_elf_beginning_at_start_gap)
{
	auto elf_under_test = N_Core::create_elf("testfiles/sleep");
	auto size_with_section = boost::filesystem::file_size("testfiles/sleep");
	auto number_of_sections_in_original_elf = elf_under_test._header->get_section_header_number_of_entries();

	auto tot_size_of_program_headers = elf_under_test._header->get_program_header_number_of_entries() * elf_under_test._header->get_program_header_entry_size();

	std::string path_for_this_iteration = "testfiles/sleep_rebuild";
	N_Core::dump_to_file(path_for_this_iteration, N_Core::create_elf("testfiles/sleep"));

	for (auto i = 0; i < number_of_sections_in_original_elf; i++)
	{
		auto elf_to_remove_section_from = N_Core::create_elf(path_for_this_iteration.c_str());
		auto size_of_removed_section = elf_to_remove_section_from._section_table._sections[0]->get_size_in_file();

		elf_to_remove_section_from.remove_section(0, N_Core::N_Section::SectionRemovalPolicy::GAP);

		path_for_this_iteration = std::string("testfiles/remove_all_sections_from_elf_iteration_gap_") + std::to_string(i);

		N_Core::dump_to_file(path_for_this_iteration, elf_to_remove_section_from);

		auto size_without_section = boost::filesystem::file_size(path_for_this_iteration);

		//When the last section is removed there is no longer any gap in the sections.
		// For example:
		// Say 3 sections exist indicated S1, S2 and S3
		// [HEADER][S1][S2][S3][SECTION_TABLE (3 elements)]
		// remove S1
		// [HEADER][GAP][S2][S3][SECTION_TABLE (2 elements)]
		// remove S3
		// [HEADER][GAP][S2][GAP][SECTION_TABLE (1 element)]
		// remove S2
		// [HEADER][GAP][GAP][GAP][SECTION_TABLE (0 elements)] = [HEADER]
		// GAPS are not written to file nor is a section table without any elements.
		if (i != number_of_sections_in_original_elf - 1)
		{
			BOOST_CHECK_EQUAL(
				size_with_section - sizeof(N_Core::N_Section::Elf64_Shdr)
				, size_without_section
			);
		}
		size_with_section = size_without_section;
	}

	//finally only elf header should be left.
	auto size_without_any_sections = size_with_section;
	BOOST_CHECK_EQUAL(
		size_without_any_sections,
		sizeof(N_Core::N_Header::Elf64_Ehdr)
	);

}

BOOST_AUTO_TEST_SUITE_END()