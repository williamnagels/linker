#include "src/include/core/section/section.h"

#include <cstdint>
#include <map>
namespace N_Core
{
	namespace N_Linker
	{
		// This class represents a section that is in progress of
		// being linked.
		//
		// The base virtual address is the first address of the segment
		// to which this section belongs. For example: all executable sections
		// are packed into one executable segment. All sections in the
		// code segment will have the same base virtual address.
		// A section does not know to which segment it belongs.
		//
		// The base offset is the offset of the segment in the file.
		// Once again the same logic as base virtual address applies.
		// All sections in the segment will have the same base offset.
		//
		// The virtuql address (address of first byte of this section when
		// loaded into memory) will always be:
		//	[offset of placeable] - [offset of segment] + [base virtual address].
		//
		// The offset of segment and base virtual address are not know 
		// when segments are being populated with sections. This is why those
		// variables are references and will be filled in when all sections have
		// had a segment assigned.
		// 
		// For example:
		//  To know the offset of the code segment (containing only executable sections)
		//  the size of the data segment must be know (this is, if the code segment is placed
		//  after the data segment in the ELF file). The position of the 
		//  code segment will more or less equal [position of data segment] + [size of data segment] +
		//	[padding for alignment]
		//
		// _links is a structure that translates local section indices used in
		// relocations/symbol tables to a LinkPlacement. It is a look-up table
		// to find the correct virtual address to be used in 
		// relocations after all sections have been put into segments.
		//
		/*template <typename DataWidthTy, typename ContainerTy>
		class Placement
		{
			uint64_t const& _base_virtual_address;
			uint64_t const& _base_offset;
			N_Section::Section<DataWidthTy>& _placeable;
			std::map<uint16_t, N_Section::Section<DataWidthTy>&> _links; //other sections used for stuff; int is a local index
												  //use _links[0]->get_va() when applying relocations.
		public:
			Placement(uint64_t const& base_virtual_address, uint64_t const& base_offset, N_Section::Section<DataWidthTy> const& section) :
				_base_virtual_address(base_virtual_address)
				, _base_offset(base_offset)
				, _section(section)
				, _links()
			{

			}

			uint64_t get_virtual_address() const
			{
				return _base_virtual_address + _section.get_offset() - _base_offset;
			};
		};

		template <typename T>
		using GlobalSymbolTable = std::map<std::string, Placement<T>>;*/
	}
}