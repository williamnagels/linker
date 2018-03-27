#include <cstdint>
#include <map>
#include "src/include/core/general.h"
namespace N_Core
{
	namespace N_Linker
	{

		// This class represents a section that is in progress of
		// being linked.
		//
		// The base virtual address is the first address of the segment
		// to which this section belongs. For example: all executable sections
		// are packed into one executable(code) segment. All sections in the
		// code segment will have the same base virtual address.
		// A section does not know to which segment it belongs.
		//
		// The base offset is the offset of the segment in the file.
		// Once again the same logic as base virtual address applies.
		// All sections in the segment will have the same base offset.
		//
		// The virtual address (address of first byte of this section when
		// loaded into memory) will always be:
		//	[offset of section] - [offset of segment] + [base virtual address].
		//
		// The offset of segment and base virtual address are not know 
		// when segments are being populated with sections. This is why those
		// variables are const references and will be set when all sections have
		// had a segment assigned.
		// 
		// For example:
		//  To know the offset of the code segment (containing only executable sections)
		//  the size of the data segment must be know (this is, if the code segment is placed
		//  after the data segment in the ELF file). The position of the 
		//  code segment will equal [position of data segment] + [size of data segment] +
		//	[padding for alignment]
		//
		//
		template <typename SectionImplementationTy>
		class Section
		{
			uint64_t const& _base_virtual_address;
			uint64_t const& _base_offset;
			SectionImplementationTy const& _section;
			Index _index; 
		public:

			Section(Index index, SectionImplementationTy const& section, uint64_t const& base_virtual_address, uint64_t const& base_offset) :
				_index(index)
				, _base_virtual_address(base_virtual_address)
				, _base_offset(base_offset)
				, _section(section){}

			uint64_t get_virtual_address() const
			{
				//apply padding here
				uint64_t offset_of_section_in_segment = (_section.get_offset() - _base_offset);
				return _base_virtual_address + offset_of_section_in_segment;
			};

			bool operator<(Section const& section) const
			{
				return _index < section._index;
			}
		};

		struct ConvertSymbolToLinkSection
		{
			
			uint64_t const& _code;
			uint64_t const& _code_offset;
			uint64_t const& _data;
			uint64_t const& _data_offset;
			ConvertSymbolToLinkSection(uint64_t const& code, uint64_t const& code_offset, uint64_t const& data, uint64_t const& data_offset):
			_code(code),_code_offset(code_offset),_data(data),_data_offset(data_offset){}

			template<typename T>
			auto operator()(T const& symbol)
			{
				Index index = Index(symbol.get_section_index());
				auto const& section = symbol.get_parent().get_parent().get_parent().get_section_at(index);			

				if (section.is_code_section())
				{
					return Section(index, section,_code, _code_offset);
				}
				else
				{
					return Section(index, section,_data, _data_offset);
				}

				
			}
		};	
	}
}