#include "src/include/core/section/section_table.h"
#include "src/include/core/section/section.h"
#include "src/include/core/elf.h"

namespace N_Core
{
	namespace N_Section
	{
		Table::~Table() = default;

		Table::Table(Table&& other_table)
		{
			_sections = std::move(other_table)._sections;
		}

		Table::Table(Table const& other_table)
		{

			for (auto const& section : other_table._sections)
			{
				_sections.emplace_back(section->deep_copy());
			}
		}

		void Table::add_section_to_back(std::unique_ptr<ASection>&& section)
		{
			return _sections.push_back(std::move(section));
		}
		void Table::add_section(std::unique_ptr<ASection>&& section, Index index)
		{
			if (is_wildcard(index))
			{
				add_section_to_back(std::move(section));
			}
			else
			{
				add_section_at_index(std::move(section), index);
			}
		}

		void Table::swap_section(Index index1, Index index2)
		{
			if (!are_valid_indices<DoesNotSupportWildCard>(index1, index2))
			{
				throw std::invalid_argument(invalid_section_index);
			}

			decltype(_sections)::iterator iterator1 = std::begin(_sections);
			decltype(_sections)::iterator iterator2 = std::begin(_sections);
			std::advance(iterator1, index1);
			std::advance(iterator2, index2);

			std::iter_swap(iterator1, iterator2);
		}

		void Table::add_section_at_index(std::unique_ptr<ASection>&& section, Index index)
		{
			if (_sections.size() >= index)
			{
				decltype(_sections)::iterator iterator = std::begin(_sections);
				std::advance(iterator, index);

				_sections.insert(iterator, std::move(section));
			}
		}
		void Table::remove_section(Index index, SectionRemovalPolicy policy)
		{
			if (!is_valid_index<DoesNotSupportWildCard>(index))
			{
				throw std::range_error(invalid_section_index);
			}

			if (policy == SectionRemovalPolicy::COMPACT)
			{
				auto offset_to_subtract = _sections.at(index)->get_size_in_file();

				std::for_each(
					_sections.begin() + (index + 1)
					, _sections.end()
					, [=](auto const& section) { section->set_offset(section->get_offset() - offset_to_subtract); }
				);
			}

			auto element_to_delete = _sections.begin() + index;
			_sections.erase(_sections.begin() + index);
		}

		ASection& Table::get_section_at_index(Index index)
		{
			return *_sections.at(index);
		}

		Table create_section_table(N_Core::Elf const& elf)
		{
			Table table;

			if (elf._header->get_section_header_number_of_entries() == 0)
			{
				return table;
			}
			auto number_of_entries = elf._header->get_section_header_number_of_entries();

			auto start_of_table = elf._header->get_section_header_offset();
			auto size_of_entry = elf._header->get_section_header_entry_size();

			if (elf.is_memory_mapped())
			{
				for (auto i = 0; i < number_of_entries; i++)
				{
					auto header_of_section_entry = start_of_table + size_of_entry * i;
					auto begin_header = elf.get_memory_mapped_region().begin() + header_of_section_entry;
					auto end_header = begin_header + size_of_entry;

					auto header_range = boost::make_iterator_range(begin_header, end_header);
					auto section = create_section(elf.get_memory_mapped_region(), header_range);
					table.add_section(std::move(section));
				}
			}
			else
			{
				for (auto i = 0; i < number_of_entries; i++)
				{
					table.add_section(create_section(elf._header->get_class() == N_Core::N_Header::Class::ELFCLASS64));
				}
			}


			return table;
		}
	}
}