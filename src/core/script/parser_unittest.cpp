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
    BOOST_CHECK_EQUAL(std::distance(std::begin(parser[0]._filters), std::end(parser[0]._filters)), 1);
    BOOST_CHECK_EQUAL(std::distance(std::begin(parser[1]._filters), std::end(parser[1]._filters)), 1);
    BOOST_CHECK_EQUAL(std::distance(std::begin(parser[2]._filters), std::end(parser[2]._filters)), 1);

    BOOST_CHECK_EQUAL(parser[0]._filters[0], "*(.text)");
    BOOST_CHECK_EQUAL(parser[1]._filters[0], "*(.data)");
    BOOST_CHECK_EQUAL(parser[2]._filters[0], "*(.bss)");
}

BOOST_AUTO_TEST_SUITE_END()