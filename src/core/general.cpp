#include "src/include/core/general.h"
void N_Core::dump(std::ostream& stream, N_Core::BinaryBlob blob)
{
	stream.write(reinterpret_cast<char const*>(blob.begin()), blob.size());
}