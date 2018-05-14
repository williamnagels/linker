#include <boost/test/unit_test.hpp> 
#include "src/include/core/script/parser.h"
BOOST_AUTO_TEST_SUITE(parser)


BOOST_AUTO_TEST_CASE(parse_some_script)
{
    N_Core::N_Parser::Parser Parser;
    Parser.parse();
}

BOOST_AUTO_TEST_SUITE_END()