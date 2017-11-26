#pragma once
#include "src/include/core/general.h"
#include "src/include/core/header/header_member_types.h"
#include "src/include/core/read_write_blob.h"

#include <memory>
#include <type_traits>
#include <ostream>
#include <boost/mpl/vector.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/iterator.hpp>
#include <boost/type_erasure/operators.hpp>
#include <boost/type_erasure/tuple.hpp>
#include <boost/type_erasure/same_type.hpp>
#include <boost/type_erasure/member.hpp>
BOOST_TYPE_ERASURE_MEMBER((has_member_xoxo), xoxo, 0)

using HeaderConcept =
boost::type_erasure::any<
	boost::mpl::vector<
	boost::type_erasure::copy_constructible<>
	, boost::type_erasure::relaxed
	>
>;


namespace N_Core
{
	namespace N_Header
	{



		const std::string wrong_magic_bytes_message = "Wrong magic bytes in elf header. Did not find /7FELF. Is the file an elf?";

		template <typename T>
		class Header
		{
		private:
			struct UnknownHeaderMapping {};
			using MemoryMap = std::conditional_t <
				std::is_same_v<Bit32, T>, 
				Elf32_Ehdr, 
				std::conditional_t <
					std::is_same_v<Bit64, T>,
					Elf64_Ehdr,
					UnknownHeaderMapping
				>
			>;

			ReadWriteBlob<MemoryMap> _header_content; ///< Memory blob with some map applied to it.
			bool are_magic_bytes_correct()
			{
				return _header_content.get(&MemoryMap::e_magic_byte_0) == 0x7F &&
					_header_content.get(&MemoryMap::e_magic_byte_1) == 'E' &&
					_header_content.get(&MemoryMap::e_magic_byte_2) == 'L' &&
					_header_content.get(&MemoryMap::e_magic_byte_3) == 'F';
			}



		public:		


			template <typename = typename std::enable_if_t<std::is_same_v<T, Bit32> || std::is_same_v<T, Bit64>> >
			Header(N_Core::BinaryBlob const& header_memory_blob) :
				_header_content(header_memory_blob)
			{
				if (!are_magic_bytes_correct())
				{
					throw std::invalid_argument(wrong_magic_bytes_message);
				}
			}

			Header(Header const& header) :
				_header_content(header._header_content)
			{

			}
			bool is_64bit_header()
			{
				return _header_content.get(&MemoryMap::e_class) == Class::ELFCLASS64;
			}
			BOOST_CONCEPT_USAGE(Header)
			{
				MemoryMap*  map = reinterpret_cast<MemoryMap*>(this);
				auto a = map->e_magic_byte_0;
			};

			friend void dump(std::ostream& stream, N_Header::Header<T> header);
		};

		//@brief dump header to stream
		//
		//
		template <typename T>
		void dump(std::ostream& stream, N_Header::Header<T> header)
		{
			dump(stream, header._header_content);
		}
	}
}