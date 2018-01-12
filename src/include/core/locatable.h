#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/any_cast.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/free.hpp>
#include <boost/mpl/vector.hpp>

BOOST_TYPE_ERASURE_MEMBER((has_set_region), set_region, 1)
BOOST_TYPE_ERASURE_MEMBER((has_get_region), get_region, 0)
BOOST_TYPE_ERASURE_MEMBER((has_get_size), get_size, 0)

using Locatable = boost::type_erasure::any<
	boost::mpl::vector<
	has_set_region<void(int)>,
	has_get_region<int(void)>,
	has_get_size<int(void)>,
	boost::type_erasure::_self&>
>;

BOOST_TYPE_ERASURE_MEMBER((assign_region), assign_region, 1)
using Provider = boost::type_erasure::any<
	boost::mpl::vector<
	assign_region<void(Locatable&)>,
	boost::type_erasure::_self&>
>;