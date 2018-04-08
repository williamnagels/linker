#pragma once

#include "src/include/core/general.h"
#include "src/include/core/segment/segment.h"
#include "src/include/core/section/section.h"
#include "src/include/core/elf.h"
#include "src/include/core/section/helpers.h"
#include "src/include/core/symtab/filters.h"
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <range/v3/all.hpp>

namespace N_Core
{
	namespace N_Linker
	{

		struct Rule
		{
			std::string _name;
			Rule(std::string name):_name(name){}
			template<typename T, typename C>
			bool operator()(N_Core::N_Section::Section<T, C> const& section)
			{
				if (section.get_name_as_string())
				{
					return _name == section.get_name_as_string();
				}

				return false;
			}
		};

		template <typename SegmentTy>
		struct SegmentFunctor
		{
			SegmentTy& _segment;
			SegmentFunctor(SegmentTy& segment):
				_segment(segment)
			{

			}

			void operator()(auto& section)
			{
				_segment._segment._sections.emplace_back(section);
				section._is_in_segment = true; //should this be ptr to segment that contains this section? Not a ref cause not all sections in segment
				section.set_address(_segment._base_virtual_address);
				section.set_offset(_segment.calculate_offset(section.get_address())); //this should probably include alignment
				_segment._running_virtual_address += section.get_size();
			}
		};

		template <typename V, typename C>
		struct SegmentBuilder
		{
			SegmentBuilder(N_Core::N_Segment::Segment<V, C>& segment, std::string rule, uint64_t virtual_address, uint64_t offset):
				_segment(segment)
				,_rule(rule)
				,_running_virtual_address(virtual_address)
				,_base_virtual_address(virtual_address)
				,_offset(offset)
			{

			}
			N_Core::N_Segment::Segment<V, C>& _segment; ///< Some segment in the output elf
			Rule _rule;
			uint64_t _running_virtual_address; ///< Can this be the VA of the segment?
			const uint64_t _base_virtual_address; 
			uint64_t _offset;
			uint64_t calculate_offset(uint64_t address)
			{
				return (address - _base_virtual_address) + _segment.get_offset();
			}
		};

		template<typename T>
		struct Linker
		{
			N_Core::Elf<T> _output_elf;

			std::string _entry_symbol;
			uint64_t _entry_value;
			std::vector<SegmentBuilder<T, decltype(_output_elf)> > _segment_builders;

			std::vector<N_Core::Elf<T> > _input_elfs;

			Linker(std::string _linker_script_path, std::vector<std::string> _input_files)
			{
				for(std::string const& path: _input_files)
				{
					_input_elfs.emplace_back(path);
				}
			}

			void build_segment_rules()
			{
				_entry_symbol = "main";
				_segment_builders.emplace_back(_output_elf.create_new_segment(0, 0), ".text", 100000, 0);
				_segment_builders.emplace_back(_output_elf.create_new_segment(0, 0), ".data", 100000, 100);
			}


			void collect_sections(auto& segment_builder)
			{
				for (auto& elf : _input_elfs)
				{
					auto range = elf._section_table 
						| ranges::view::filter(segment_builder._rule);

					ranges::v3::for_each(range,SegmentFunctor{segment_builder});
				}
			} 

			void update_segment_builder(auto& segment_builder, uint64_t offset, uint64_t size)
			{
				
			}
			std::optional<uint64_t> get_value_of_entry_symbol(auto& elf)
			{
				auto symbol_range = elf._section_table
					| ranges::view::filter(N_Core::N_Section::N_Filters::SymbolTable{})
					| ranges::view::transform(N_Core::N_Section::ConvertSectionToSymbolRange{})
					| ranges::view::join
					| ranges::view::filter(N_Core::N_Symbol::N_Filters::HasName{_entry_symbol})
					| ranges::view::filter(N_Core::N_Symbol::N_Filters::Defined{})
					| ranges::view::transform(N_Core::ConvertSymbolToSection{})
					| ranges::view::transform([](auto const& section){return section.get_address();});

				if (ranges::distance(symbol_range) == 1)
				{
					return *symbol_range.begin();
				}
			}
			std::optional<uint64_t> get_value_of_entry_symbol()
			{
				for (auto& elf : _input_elfs)
				{
					auto value = get_value_of_entry_symbol(elf);

					if(value)
					{
						return value;
					}
				}	
			}


			void collect_sections()
			{
				uint64_t prev_offset = 0;
				for (auto& segment_builder: _segment_builders)
				{
					collect_sections(segment_builder);
					prev_offset+= segment_builder._segment.get_offset();
					segment_builder._segment.set_offset(prev_offset);
					prev_offset+= segment_builder._segment.get_file_size();
				}
			}

			void do_link()
			{
				build_segment_rules();
				collect_sections();
				//do relocations here
				_entry_value = *get_value_of_entry_symbol(); //check is in segment bool???
				//create header in output elf
			}

		};




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