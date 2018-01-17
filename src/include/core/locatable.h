
#include <variant>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/any_cast.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/free.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/any_cast.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/free.hpp>
#include <boost/signals2/signal.hpp>

#include <boost/mpl/vector.hpp>

#include <cstdint>

// While assembling sections into segment and elfs the absolute position in the file 
// is not required to be know. What is required to be know is the relative position
// to some marker.
// afterall symbols represent a virtual address. E.g. main is @ 0x0000
// then everywhere where that symbols is used in relocations the value 0x0000 is filled in.
//
// then pages come in to play. The segment is typically page aligned. If we want main at 0x0000
// we probably want it to start at offset 0 in some segment and map that segment into virtual address 0X0000
// that makes sense.
//
// Let's say main starts at 0xa. Now main is copied at offset 0xa in the segment yet the segment is still mapped
// at virtual address 0x0000. That makes sense.
//
// Let's say the page size is 0x10. main must be mapped to virtual address 0x1a. what is the offset of that symbol in the elf segment?
// probably a again. and map it virtual address 0x10. Offset in segment(file bytes) = virtual address(memory bytes) % page size(file bytes).
//
// so how does it work? Push section into segment then the offset must be updated. given the section alignment.
// if the section needs to grow; change its size.
//
// when all sections are completed (code, data, ...) assign the virtual address.
// 
// Let's say main must be at 0xabcd and is at offset 0x4543 in some segment. Page size = 0x1000
//
// virtual address of the first byte of the segment = 0xabcd - 0x4543 = some_value




boost::signals2::signal<uint64_t(uint64_t, uint64_t)> _updated_size_signal; ///< emit to all slots that some element got its size changed. Returns new offset
boost::signals2::signal<uint64_t(uint64_t, uint64_t)> _updated_alignment_signal; ///< emit to all slots that some element changed aligment. Returns new offset


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