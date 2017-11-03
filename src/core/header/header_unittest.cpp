#include <boost/test/unit_test.hpp> 
#include "src/include/core/header/header.h"
#include "src/include/core/header/header_parse_strategy.h"
#include "src/include/core/elf.h"
#define HAS_MESSAGE(message_to_verify) [](auto& exception) { return !std::string(exception.what()).compare(message_to_verify); }

BOOST_AUTO_TEST_SUITE(elf_header)

// Not being able to parse elf header (magic byte = wrong)
BOOST_AUTO_TEST_CASE(not_an_elf)
{
	BOOST_CHECK_EXCEPTION(
		N_Core::create_elf_from_path_to_file("testfiles/wrong_magic_bytes"), 
		std::invalid_argument, 
		HAS_MESSAGE(N_Core::N_Header::wrong_magic_bytes_message));
}

BOOST_AUTO_TEST_CASE(parse_64bit_sleep)
{
	N_Core::Elf elf = N_Core::create_elf_from_path_to_file("testfiles/sleep");
	
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_type(), N_Core::N_Header::Type::ET_EXEC);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_version(), N_Core::N_Header::Version::EV_CURRENT);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_header_size(), sizeof(N_Core::N_Header::Elf64_Ehdr));
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_program_header_offset(), sizeof(N_Core::N_Header::Elf64_Ehdr));
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_machine(), N_Core::N_Header::Machine::EM_X86_64);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_number_of_entries_in_program_header_table(), 9);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_number_of_entries_in_section_header_table(), 27);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_index_of_section_that_contains_section_names(), 26);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_size_of_program_header_table(), 56);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_size_of_section_header_table(), 64);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_flags(), 0);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_section_header_offset(), 29480);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_entry(), 0x4017b9);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_OS_ABI(), N_Core::N_Header::OS_ABI::ELFOSABI_NONE);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_ABI_version(), 0);
}


BOOST_AUTO_TEST_CASE(COW_header)
{
	N_Core::Elf elf = N_Core::create_elf_from_path_to_file("testfiles/sleep");

	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_type(), N_Core::N_Header::Type::ET_EXEC);
	elf._header._header_parse_strategy->set_type(N_Core::N_Header::Type::ET_DYN);
	BOOST_CHECK_EQUAL(elf._header._header_parse_strategy->get_type(), N_Core::N_Header::Type::ET_DYN);
}



BOOST_AUTO_TEST_SUITE_END()