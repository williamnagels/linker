#include <boost/test/unit_test.hpp> 
#include "src/include/core/header/header.h"
#include "src/include/core/elf.h"
#define HAS_MESSAGE(message_to_verify) [](auto& exception) { return !std::string(exception.what()).compare(message_to_verify); }

BOOST_AUTO_TEST_SUITE(elf_header)

// Not being able to parse elf header (magic byte = wrong)
BOOST_AUTO_TEST_CASE(not_an_elf)
{
	/*
	BOOST_CHECK_EXCEPTION(
	N_Core::create_elf("testfiles/wrong_magic_bytes");, 
	std::invalid_argument, 
	HAS_MESSAGE(N_Core::N_Header::wrong_magic_bytes_message));*/
}

BOOST_AUTO_TEST_CASE(parse_64bit_sleep)
{
	/*
	boost::interprocess::file_mapping m_file("testfiles/sleep", boost::interprocess::read_only);
	auto&& memory_region = std::make_shared<boost::interprocess::mapped_region>(m_file, boost::interprocess::read_only);
	N_Core::Elf<N_Core::Bit64> elf(std::move(memory_region));

	auto header = elf._header.get();
	
	BOOST_CHECK_EQUAL(header.e_type, N_Core::N_Header::Type::ET_EXEC);
	BOOST_CHECK_EQUAL(header.e_version, N_Core::N_Header::Version::EV_CURRENT);
	BOOST_CHECK_EQUAL(header.e_ehsize, sizeof(N_Core::N_Header::Elf64_Ehdr));
	BOOST_CHECK_EQUAL(header.e_phoff, sizeof(N_Core::N_Header::Elf64_Ehdr));
	BOOST_CHECK_EQUAL(header.e_machine, N_Core::N_Header::Machine::EM_X86_64);
	BOOST_CHECK_EQUAL(header.e_phnum, 9);
	BOOST_CHECK_EQUAL(header.e_shnum, 27);
	BOOST_CHECK_EQUAL(header.e_shstrndx, 26);
	BOOST_CHECK_EQUAL(header.e_phentsize, 56);
	BOOST_CHECK_EQUAL(header.e_shentsize, 64);
	BOOST_CHECK_EQUAL(header.e_flags, 0);
	BOOST_CHECK_EQUAL(header.e_shoff, 29480);
	BOOST_CHECK_EQUAL(header.e_entry, 0x4017b9);
	BOOST_CHECK_EQUAL(header.e_OS_ABI, N_Core::N_Header::OS_ABI::ELFOSABI_NONE);
	BOOST_CHECK_EQUAL(header.e_ABI_version, 0);*/
}


BOOST_AUTO_TEST_CASE(COW_header)
{
	/*
	boost::interprocess::file_mapping m_file("testfiles/sleep", boost::interprocess::read_only);
	auto&& memory_region = std::make_shared<boost::interprocess::mapped_region>(m_file, boost::interprocess::read_only);
	N_Core::Elf<N_Core::Bit64> elf(std::move(memory_region));


	auto header = elf._header.get();
	BOOST_CHECK_EQUAL(header.e_type, N_Core::N_Header::Type::ET_EXEC);
	header.e_type = N_Core::N_Header::Type::ET_DYN;
	elf._header.set(header);
	BOOST_CHECK_EQUAL(elf._header.get().e_type, N_Core::N_Header::Type::ET_DYN);
	*/
}



BOOST_AUTO_TEST_SUITE_END()