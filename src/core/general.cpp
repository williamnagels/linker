#include <algorithm>

#include "src/include/core/general.h"

void N_Core::dump(std::ostream& stream, N_Core::BinaryBlob blob)
{


	/*
	char bytes_to_find[] = { 0x6e, 0x66, 0x6f, 0x00 };

	auto it = std::search(
		blob.begin()
		,blob.end()
		,std::begin(bytes_to_find)
		,std::end(bytes_to_find));

	if (it != blob.end())
	{
		throw std::exception("huh");
	}
	*/
	stream.write(reinterpret_cast<char const*>(blob.begin()), blob.size());
}