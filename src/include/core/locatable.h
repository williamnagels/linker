
#include <variant>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/any_cast.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/free.hpp>
#include <boost/mpl/vector.hpp>#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/any_cast.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/free.hpp>
#include <boost/mpl/vector.hpp>

using StorageProvider = boost::type_erasure::any<
	boost::mpl::vector<
		boost::type_erasure::copy_constructible<>,
		boost::type_erasure::typeid_<>,
		boost::type_erasure::relaxed
	>,
	boost::type_erasure::_self&
>;

//template <typename T>
//using Provider = std::variant<N_Core::Elf<T>>;