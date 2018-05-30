#pragma once

#include "src/include/core/general.h"
#include "src/include/core/segment/segment.h"
#include "src/include/core/section/section.h"
#include "src/include/core/elf.h"
#include "src/include/core/section/helpers.h"
#include "src/include/core/script/parser.h"
#include "src/include/core/symtab/filters.h"
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <range/v3/all.hpp>
#include <iostream>
namespace N_Core
{
	namespace N_Linker
	{
		namespace
		{
			enum struct SymbolPrio
			{
				LOCAL,
				WEAK,
				GLOBAL
			};

			struct SymbolDef
			{
				uint64_t _value;
				N_Core::N_Symbol::Binding _binding;
			};
			std::map<std::string, SymbolDef> _symbols;

			uint64_t calculate_offset(uint64_t size_prev_segment, uint64_t alignment_this_segment)
			{
				return size_prev_segment - size_prev_segment % alignment_this_segment;
			}

			uint64_t round_to_nearest_multiple(uint64_t va, uint64_t alignment)
			{
				uint64_t remainder = va % alignment;
				if (remainder == 0)
					return va;


				return va + alignment - remainder;
			}
		}
		struct Rule
		{
			N_Core::N_Parser::Parser::const_iterator _it;
			Rule(N_Core::N_Parser::Parser::const_iterator it):_it(it){}
			template<typename T, typename C>
			bool operator()(N_Core::N_Section::Section<T, C> const& section)
			{
				if (section.get_name_as_string())
				{
					return is_valid(*section.get_name_as_string(),*_it);
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
		
				auto alignment = section.get_address_alignment();

				auto wrong = _segment._running_virtual_address % alignment;
				
				if (wrong)
				{
					auto to_offset = alignment-wrong;
					_segment._running_virtual_address+= to_offset;
				}
				section.set_address(_segment._running_virtual_address);
				section.set_offset(_segment.calculate_offset(section.get_address())); //this should probably include alignment
				_segment._running_virtual_address += section.get_size();

				auto defined_global_symbol = section.get_parent()._section_table
					| ranges::view::filter(N_Core::N_Section::N_Filters::SymbolTable{})
					| ranges::view::transform(N_Core::N_Section::ConvertSectionToSymbolRange{})
					| ranges::view::join
					| ranges::view::filter(N_Core::N_Symbol::N_Filters::Global{})
					| ranges::view::filter(N_Core::N_Symbol::N_Filters::Defined{})
					| ranges::view::filter(N_Core::N_Symbol::N_Filters::HasIndex{section.get_index()});

				for (auto const& symbol:defined_global_symbol)
				{
					auto new_symbol_value = section.get_address() + symbol.get_value();
					auto it = _symbols.find(*symbol.get_name_as_string());

					if(it != _symbols.end())
					{
						//local has prio on global
						//global has prio on weak
						//weak symbols cannot be preempted by other weak symbols
						if (it->second._binding > symbol.get_binding())
						{
							std::cout << "*REPLACED* SYM;name="<<*symbol.get_name_as_string()<<" 0x"<<it->second._value<<" with 0x"<<new_symbol_value<<std::endl;
							
							it->second._value = new_symbol_value;
							it->second._binding = symbol.get_binding();
							//_symbols[*symbol.get_name_as_string()] = {new_symbol_value, symbol.get_binding()};
						} 
						else if( (it->second._binding == symbol.get_binding()) && symbol.get_binding() == N_Core::N_Symbol::Binding::STB_GLOBAL)
						{
							throw std::runtime_error("Redefined global symbol: " + *symbol.get_name_as_string());
						}
					}
					else
					{
						std::cout << "*NEW* SYM;name="<<*symbol.get_name_as_string()<<"0x"<<new_symbol_value<<std::endl;
						_symbols[*symbol.get_name_as_string()] = {new_symbol_value, symbol.get_binding()};
					}
		
					
				}
				
			}
		};

		template <typename V, typename C>
		struct SegmentBuilder
		{
			SegmentBuilder(N_Core::N_Segment::Segment<V, C>& segment, N_Core::N_Parser::Parser::const_iterator it):
				_segment(segment)
				,_rule(it)
				,_running_virtual_address( (it->_address)?*it->_address:0)
				,_it(it)
			{

			}

			bool should_include_section(std::string section_name)
			{
				return is_valid(section_name, *_it);
			}
			void update_internal_offset(uint64_t new_value)
			{
				_segment._internal_offset += new_value;
				_running_virtual_address = _segment.get_virtual_address() + _segment._internal_offset;
			}
			uint64_t calculate_offset(uint64_t address)
			{
				return (address - _segment.get_virtual_address()) + _segment.get_offset();
			}
			N_Core::N_Segment::Segment<V, C>& _segment; ///< Some segment in the output elf
			Rule _rule;
			uint64_t _running_virtual_address;
			N_Core::N_Parser::Parser::const_iterator _it;

		};

		template<typename T>
		struct Linker
		{
			N_Core::Elf<T> _output_elf;

			std::string _entry_symbol;
			std::vector<SegmentBuilder<T, decltype(_output_elf)> > _segment_builders;

			std::vector<N_Core::Elf<T> > _input_elfs;
    		N_Core::N_Parser::Parser _parser;
			Linker(std::string _linker_script_path, std::vector<std::string> _input_files):
				_parser(_linker_script_path)
			{
				_input_elfs.reserve(_input_files.size());
				for(std::string const& path: _input_files)
				{
					_input_elfs.emplace_back(path);
				}
			}

			void build_segment_rules()
			{
				uint64_t virtual_address = 0x400000;
				_entry_symbol = "main";


				auto text_segment_it = std::find_if(std::begin(_parser), std::end(_parser), [](auto const& segment){return segment._name == ".text";});
				auto data_segment_it = std::find_if(std::begin(_parser), std::end(_parser), [](auto const& segment){return segment._name == ".data";});


				auto& text_segment = _output_elf.create_new_segment(64+56+56);
				text_segment.set_type(N_Core::N_Segment::PT_LOAD);
				text_segment.set_flags(N_Core::N_Segment::Flags{1,0,1});
				text_segment.set_alignment(0x200000);


				auto& data_segment = _output_elf.create_new_segment(0);
				data_segment.set_type(N_Core::N_Segment::PT_LOAD);
				data_segment.set_flags(N_Core::N_Segment::Flags{0,1,1});
				data_segment.set_alignment(0x200000);
				
				_segment_builders.emplace_back(text_segment, text_segment_it);
				_segment_builders.emplace_back(data_segment, data_segment_it);
			}


			void collect_sections(auto& segment_builder)
			{
				for (auto& elf : _input_elfs)
				{
					std::cout << "Reading input elf"<<std::endl;

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
				
				auto it = _symbols.find(_entry_symbol);
				auto copy = _symbols;
				if (it == std::end(_symbols))
				{
					throw std::runtime_error("entry symbol not found: "+ _entry_symbol);
				}

				std::cout << "Entry symbol:0x"<<it->second._value<<std::endl;
				return it->second._value;
				// std::find(std::begin(_symbols),std::end(_symbols), _entry)
				// auto symbol_range = elf._section_table
				// 	| ranges::view::filter(N_Core::N_Section::N_Filters::SymbolTable{})
				// 	| ranges::view::transform(N_Core::N_Section::ConvertSectionToSymbolRange{})
				// 	| ranges::view::join
				// 	| ranges::view::filter(N_Core::N_Symbol::N_Filters::HasName{_entry_symbol})
				// 	| ranges::view::filter(N_Core::N_Symbol::N_Filters::Defined{})
				// 	| ranges::view::transform(N_Core::ConvertSymbolToSection{})
				// 	| ranges::view::transform([](auto const& section){return section.get_address();});

				// if (ranges::distance(symbol_range) == 1)
				// {
				// 	return *symbol_range.begin();
				// }
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


			template <typename a, typename b, typename c, typename d>
			void perform_local_relocations(
				N_Core::N_Section::Section<a, b>& section_to_relocate, 
				N_Core::N_Relocation::Relocation<c, d> const& relocation_entry, 
				uint64_t symbol_value)
			{
				uint64_t value_to_apply = 0;
				uint64_t VA_of_reloc = section_to_relocate.get_address() + relocation_entry.get_offset();
				switch(relocation_entry.get_type())
				{
					case N_Core::N_Relocation::Type::R_X86_64_NONE:
					break;
					case N_Core::N_Relocation::Type::R_X86_64_64:
					break;
					case N_Core::N_Relocation::Type::R_X86_64_PC32:
					{
						int32_t first = (int32_t)symbol_value + relocation_entry.get_addend();
						int32_t second = (int32_t)(VA_of_reloc);
						int32_t val = (int32_t)(first - second );

						auto& cont = std::get<0>(section_to_relocate.get_interpreted_content());
						cont.resize(cont.get_size());
						uint8_t* base = &(*cont.begin());
						base += relocation_entry.get_offset();
						std::memcpy(base, &val, 4);

						std::cout << "Going to apply relocation with type:"<<relocation_entry.get_type()<<std::endl;
						std::cout << "value of symbol:0x"<<first<<std::endl;
						std::cout << "VA of reloc:0x"<<second<<std::endl;
						std::cout << "Program counter offset:0x"<<val<<std::endl;
						std::cout << "VA;0x"<<second+val<<std::endl;

					};
					break;
					default:
					break;
				}

			}

			template <typename V, typename C, typename a, typename b>
			void perform_local_relocations(N_Core::N_Section::Section<a, b>& section_to_relocate, N_Core::N_Relocation::Table<V, C> const& relocation_table, auto const& symbol_table)
			{

				for (auto const& relocation_entry:relocation_table)
				{
					auto symbol_index = relocation_entry.get_symbol_index();
					auto it = std::begin(symbol_table);
					std::advance(it, symbol_index);
					auto const& symbol = *it;

					auto section_index = symbol.get_section_index();

					uint64_t value = 0;
				
					if (!section_index || symbol.is_weak())
					{
						auto symbol_to_find = *symbol.get_name_as_string();
						value = _symbols[symbol_to_find]._value;
					}
					else 
					{
						value = section_to_relocate.get_parent().get_section_at(section_index).get_address();
					}

					perform_local_relocations(section_to_relocate, relocation_entry, value);
				}
				
			}

			void perform_local_relocations()
			{
				for (auto& segment_builder: _segment_builders)
				{
					auto& segment = segment_builder._segment;

					for (auto& section_to_relocate:segment._sections)
					{

						auto sections = section_to_relocate.get().get_parent()._section_table
							| ranges::view::filter(N_Core::N_Section::N_Filters::RelocationTable{N_Core::IndexList{section_to_relocate.get().get_index()}});
						
						//std::cout<<"got nr sections: "<< ranges::v3::distance(sections)<<std::endl;
						for (auto const& relocation_table_section:sections)
						{
							auto symbol_table_section_index = relocation_table_section.get_link();	

							auto const& symbol_table_section = relocation_table_section.get_parent().get_section_at(symbol_table_section_index);

							auto const& relocation_table =  std::get<2>(relocation_table_section.get_interpreted_content());
							auto const& symbol_table = std::get<1>(symbol_table_section.get_interpreted_content());

							perform_local_relocations(section_to_relocate.get(), relocation_table, symbol_table);
						}							
					}
				}
		
			}
			void collect_sections()
			{
				uint64_t next_offset = 0;
				uint64_t previous_virtual_address = 0;
				for (auto& segment_builder: _segment_builders)
				{
					auto& segment = segment_builder._segment;

					uint64_t address = segment_builder._running_virtual_address;
					if (!address)
					{
						address= round_to_nearest_multiple(previous_virtual_address, segment.get_alignment());
					}
					segment.set_virtual_address(address);
					segment.set_physical_address(address);
					segment_builder._running_virtual_address = address;

					uint64_t offset = next_offset;
					uint64_t delta_offset = offset % segment.get_alignment();

					if (delta_offset)
					{
						offset -= delta_offset;
					}
				


					segment.set_offset(offset);
					segment_builder.update_internal_offset(delta_offset);

					std::cout << "offset= 0x" <<std::hex<< offset<<std::endl;
					std::cout << "virtual_address= 0x" <<std::hex<<  address<<std::endl;
					std::cout << "internal_offset= 0x"<<std::hex<<segment._internal_offset<<std::endl;

					collect_sections(segment_builder);

					segment_builder._segment.calculate_sizes_based_on_sizes_of_sections();
					next_offset = offset + segment_builder._segment.get_file_size();
					previous_virtual_address = address + segment_builder._segment.get_file_size();

					for (auto const& section:segment._sections)
					{
						std::cout << "section offset= 0x" <<std::hex<< section.get().get_offset()<<std::endl;
						std::cout << "section VA = 0x" <<std::hex<< section.get().get_address()<<std::endl;
					}
				}
			}

			void do_link(std::string path)
			{
				build_segment_rules();
				collect_sections();
				
				//do relocations here
				_output_elf._header.set_entry(*get_value_of_entry_symbol());
				_output_elf._header.set_type(N_Core::N_Header::ET_EXEC);
				_output_elf._header.set_data(1);
				_output_elf._header.set_file_version(1);
				_output_elf._header.set_machine(N_Core::N_Header::EM_X86_64);
				_output_elf._header.set_version(N_Core::N_Header::EV_CURRENT);
				_output_elf._header.set_program_header_number_of_entries(_output_elf._segment_table.size());
				_output_elf._header.set_program_header_entry_size(sizeof(typename decltype(_output_elf)::SegmentTy::T));
				_output_elf._header.set_program_header_offset(_output_elf._header.get_elf_header_size());
				
				perform_local_relocations();
				dump_to_file(path, _output_elf);
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