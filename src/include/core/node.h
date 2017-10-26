#pragma once
#include <cstdint>
namespace N_Core
{
	/*@brief Represents an entity accessible in the tree that will be build
	* for each elf component. Essentially it is an interface for entitites of
	* the binary that require dumping to an output file (e.g. sections, symtabs, ...).
	* Operations that should be performed on each component of an elf should be added to this interface.
	*
	*/
	class Node
	{
	public:
		virtual void dump_content_to_memory(uint8_t* base_address) {}
	};
}
