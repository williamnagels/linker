#include <boost/test/unit_test.hpp> 
#include "src/include/core/header/header.h"
#include "src/include/core/elf.h"

#include <fstream>
#include <string>
#define HAS_MESSAGE(message_to_verify) [](auto& exception) { return !std::string(exception.what()).compare(message_to_verify); }

BOOST_AUTO_TEST_SUITE(elf_header)
/*

// Not being able to parse elf header (magic byte = wrong)
BOOST_AUTO_TEST_CASE(not_an_elf)
{
	BOOST_CHECK_EXCEPTION(
	N_Core::create_elf("testfiles/wrong_magic_bytes"), 
	std::invalid_argument, 
	HAS_MESSAGE(N_Core::N_Header::wrong_magic_bytes_message));
}

BOOST_AUTO_TEST_CASE(parse_64bit_sleep)
{
	auto elf = N_Core::create_elf("testfiles/sleep");

	BOOST_CHECK_EQUAL(elf._header->get_type(), N_Core::N_Header::Type::ET_EXEC);
	BOOST_CHECK_EQUAL(elf._header->get_version(), N_Core::N_Header::Version::EV_CURRENT);
	BOOST_CHECK_EQUAL(elf._header->get_elf_header_size(), sizeof(N_Core::N_Header::Elf64_Ehdr));
	BOOST_CHECK_EQUAL(elf._header->get_program_header_offset(), sizeof(N_Core::N_Header::Elf64_Ehdr));
	BOOST_CHECK_EQUAL(elf._header->get_machine(), N_Core::N_Header::Machine::EM_X86_64);
	BOOST_CHECK_EQUAL(elf._header->get_program_header_number_of_entries(), 9);
	BOOST_CHECK_EQUAL(elf._header->get_section_header_number_of_entries(), 27);
	BOOST_CHECK_EQUAL(elf._header->get_section_index_that_contains_strings(), 26);
	BOOST_CHECK_EQUAL(elf._header->get_program_header_entry_size(), 56);
	BOOST_CHECK_EQUAL(elf._header->get_section_header_entry_size(), 64);
	BOOST_CHECK_EQUAL(elf._header->get_flags(), 0);
	BOOST_CHECK_EQUAL(elf._header->get_section_header_offset(), 29480);
	BOOST_CHECK_EQUAL(elf._header->get_entry(), 0x4017b9);
	BOOST_CHECK_EQUAL(elf._header->get_OS_ABI(), N_Core::N_Header::OS_ABI::ELFOSABI_NONE);
	BOOST_CHECK_EQUAL(elf._header->get_ABI_version(), 0);
}


BOOST_AUTO_TEST_CASE(COW_header)
{
	auto elf = N_Core::create_elf("testfiles/sleep");
	BOOST_CHECK_EQUAL(elf._header->get_type(), N_Core::N_Header::Type::ET_EXEC);
	elf._header->set_type(N_Core::N_Header::Type::ET_DYN);
	BOOST_CHECK_EQUAL(elf._header->get_type(), N_Core::N_Header::Type::ET_DYN);
}

BOOST_AUTO_TEST_CASE(deep_copy)
{
	auto elf = N_Core::create_elf("testfiles/sleep");
	BOOST_CHECK_EQUAL(elf._header->get_type(), N_Core::N_Header::Type::ET_EXEC);

	auto elf2 = N_Core::create_elf(elf);
	BOOST_CHECK_EQUAL(elf2._header->get_type(), N_Core::N_Header::Type::ET_EXEC);
	elf2._header->set_type(N_Core::N_Header::Type::ET_DYN);
	BOOST_CHECK_EQUAL(elf._header->get_type(), N_Core::N_Header::Type::ET_EXEC);
	BOOST_CHECK_EQUAL(elf2._header->get_type(), N_Core::N_Header::Type::ET_DYN);
}

BOOST_AUTO_TEST_CASE(dump)
{
	auto elf = N_Core::create_elf("testfiles/sleep");
	BOOST_CHECK_EQUAL(elf._header->get_type(), N_Core::N_Header::Type::ET_EXEC);

	N_Core::Elf elf2 = N_Core::create_elf(elf);
	elf2._header->set_padding_byte_0('w');
	elf2._header->set_padding_byte_1('i');
	elf2._header->set_padding_byte_2('l');
	elf2._header->set_padding_byte_3('l');
	elf2._header->set_padding_byte_4('i');
	elf2._header->set_padding_byte_5('a');
	elf2._header->set_padding_byte_6('m');

	N_Core::dump_to_file("testfiles/dump", elf2);
	
	auto elf3 = N_Core::create_elf("testfiles/dump");

	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_0(), 'w');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_1(), 'i');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_2(), 'l');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_3(), 'l');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_4(), 'i');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_5(), 'a');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_6(), 'm');
}

BOOST_AUTO_TEST_CASE(non_existing_file)
{
	BOOST_CHECK_EXCEPTION(
		N_Core::create_elf("testfiles/non_existing_file"),
		boost::interprocess::interprocess_exception,
		HAS_MESSAGE("The system cannot find the file specified."));
}

BOOST_AUTO_TEST_CASE(new_elf)
{
	// There have been issues in the past where there was a bug in 
	// dump_to_file leading bytes to be interepreted as characters i.s.o. actually binary data
	// this would only be an issue on certain numbers. That's why checking a certain range of numbers.
	for (auto i = 0; i < 1000; i++)
	{
		auto elf = N_Core::create_elf(N_Core::N_Header::Class::ELFCLASS64);
		elf._header->set_entry(i);
		N_Core::dump_to_file("testfiles/new_elf", elf);
		auto elf2 = N_Core::create_elf("testfiles/new_elf");

		BOOST_CHECK_EQUAL(elf2._header->get_class(), N_Core::N_Header::Class::ELFCLASS64);
		BOOST_CHECK_EQUAL(elf2._header->get_entry(), i);
	}

}*/

BOOST_AUTO_TEST_SUITE_END()