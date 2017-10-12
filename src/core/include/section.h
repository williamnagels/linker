#include "src/core/include/addressable.h"
#include <variant>
#include <functional>
#include <boost\range.hpp>
namespace Core
{
	using BinaryBlob = boost::iterator_range<std::vector<uint8_t>::iterator>; ///< A blob of bytes

	/*@brief ELF Section representation.
	*
	* Contains section header and reference to the content of the section.
	*
	*/
	class Section
	{
	public:
		Section();

	private:

		struct Header64
		{

		};

		struct Header32
		{

		};
		std::variant<Header64, Header32> _header; ///< 32 or 64-bit header depending on the elf it is contained in.
		std::function<BinaryBlob(void)> _content_retriever; ///< Content of the section stored somewhere. Expected to be accesible as long as the section object exists.
		VirtualAddressChangedSignal _virtual_address_changed_signal; ///< Connect slot to this signal to receive updates about VMA changes.
	};
};
