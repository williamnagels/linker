#include <boost/test/unit_test.hpp> 
#include "src/include/core/header/header.h"
#include "src/include/core/elf.h"

#include <fstream>
#include <string>
#define HAS_MESSAGE(message_to_verify) [](auto& exception) { return !std::string(exception.what()).compare(message_to_verify); }

BOOST_AUTO_TEST_SUITE(elf_header)

// Not being able to parse elf header (magic byte = wrong)
BOOST_AUTO_TEST_CASE(not_an_elf)
{
	BOOST_CHECK_EXCEPTION(
	N_Core::create_elf("testfiles/wrong_magic_bytes");, 
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
	const void * address = static_cast<const void*>(this); // cast to void* to avoid operator<<(T const* const) overload kicking in
	std::stringstream ss;
	ss << "testfiles/"<< address;

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

	N_Core::dump_to_file(ss.str(), elf2);

	auto elf3 = N_Core::create_elf(ss.str());
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_0(), 'w');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_1(), 'i');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_2(), 'l');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_3(), 'l');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_4(), 'i');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_5(), 'a');
	BOOST_CHECK_EQUAL(elf3._header->get_padding_byte_6(), 'm');
}


BOOST_AUTO_TEST_SUITE_END()