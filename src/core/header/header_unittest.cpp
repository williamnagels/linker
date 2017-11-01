#include <boost/test/unit_test.hpp> 
#include "src/include/core/header/header.h"
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


BOOST_AUTO_TEST_SUITE_END()