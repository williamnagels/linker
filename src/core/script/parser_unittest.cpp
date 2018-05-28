#include <boost/test/unit_test.hpp> 
#include "src/include/core/script/parser.h"
BOOST_AUTO_TEST_SUITE(parser)


BOOST_AUTO_TEST_CASE(parse_some_script)
{
    N_Core::N_Parser::Parser parser("testfiles/simple_script");

    BOOST_CHECK_EQUAL(std::distance(std::begin(parser), std::end(parser)), 3);

    BOOST_CHECK_EQUAL(parser[0]._name, ".text");
    BOOST_CHECK_EQUAL(parser[1]._name, ".data");
    BOOST_CHECK_EQUAL(parser[2]._name, ".bss");
    BOOST_CHECK(N_Core::N_Parser::is_valid("front.text",parser[0]));
    BOOST_CHECK(not N_Core::N_Parser::is_valid(".text.duh",parser[0]));
    BOOST_CHECK(not N_Core::N_Parser::is_valid("front.text.duh",parser[0]));
    BOOST_CHECK(N_Core::N_Parser::is_valid(".text",parser[0]));

    BOOST_CHECK(N_Core::N_Parser::is_valid(".data.back",parser[1]));
    BOOST_CHECK(not N_Core::N_Parser::is_valid(".back.data",parser[1]));
    BOOST_CHECK(not N_Core::N_Parser::is_valid(".dat",parser[1]));
    BOOST_CHECK(N_Core::N_Parser::is_valid(".data",parser[1]));

    BOOST_CHECK(N_Core::N_Parser::is_valid("front.bssback",parser[2]));
    BOOST_CHECK(N_Core::N_Parser::is_valid(".bss",parser[2]));
}

BOOST_AUTO_TEST_SUITE_END()