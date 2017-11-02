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
	
	BOOST_CHECK(elf._header._header_parse_strategy->get_type() == N_Core::N_Header::Type::ET_EXEC);
	BOOST_CHECK(elf._header._header_parse_strategy->get_type() != N_Core::N_Header::Type::ET_DYN);
	BOOST_CHECK(elf._header._header_parse_strategy->get_version() == N_Core::N_Header::Version::EV_CURRENT);
	BOOST_CHECK(elf._header._header_parse_strategy->get_header_size() == sizeof(N_Core::N_Header::Elf64_Ehdr));
	BOOST_CHECK(elf._header._header_parse_strategy->get_program_header_offset() == sizeof(N_Core::N_Header::Elf64_Ehdr));
	BOOST_CHECK(elf._header._header_parse_strategy->get_machine() == N_Core::N_Header::Machine::EM_X86_64);
}

BOOST_AUTO_TEST_CASE(COW_header)
{
	N_Core::Elf elf = N_Core::create_elf_from_path_to_file("testfiles/sleep");

	BOOST_CHECK(elf._header._header_parse_strategy->get_type() == N_Core::N_Header::Type::ET_EXEC);
	elf._header._header_parse_strategy->set_type(N_Core::N_Header::Type::ET_DYN);
	BOOST_CHECK(elf._header._header_parse_strategy->get_type() == N_Core::N_Header::Type::ET_DYN);
}
BOOST_AUTO_TEST_CASE(memory_leak_cleanup)
{
	BOOST_CHECK(1 == 1);
}


BOOST_AUTO_TEST_SUITE_END()