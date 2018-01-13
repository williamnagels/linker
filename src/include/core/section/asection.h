#pragma once
#include "src/include/core/general.h"
#include "src/include/core/section/section_member_types.h"

#include "cow.h"

#include <cstdint>
#include <memory>

namespace N_Core
{
	namespace N_Section
	{
		// @brief Public API of a section.
		// 
		// This class exposes many setters & getters that can be used to change
		// the value of basically all members of a section as described by the ELF standard.
		//
		class ASection
		{
		public:
			virtual ~ASection() {}
			virtual MMap::Container<uint8_t>const& get_content() const = 0;
			virtual uint64_t get_name() const = 0;
			virtual Type get_type() const = 0;
			virtual Flags get_flags() const = 0;
			virtual uint64_t get_address()const = 0;
			virtual uint64_t get_offset()const = 0;
			virtual void set_offset(uint64_t offset) = 0;
			virtual uint64_t get_size()const = 0;
			virtual uint64_t get_link() const = 0;
			virtual uint64_t get_info()const = 0;
			virtual uint64_t get_address_alignment()const = 0;
			virtual uint64_t get_entry_size()const = 0;
			virtual uint64_t get_size_in_file() const = 0;
			virtual std::unique_ptr<ASection> deep_copy() const& = 0;
			virtual std::unique_ptr<ASection> deep_copy() && = 0;
		};

		// @brief Create a section from already allocated memory.
		// 
		// @param header	Address range where the header entry of this section is loaded into memory.
		// @param elf_blob	Full address range of the elf (needed to look up the content).
		//
		// @returns the section
		//
		// @note In no way does this function take ownership of the data at the memory regions
		//   of the content and section header. As long as the section exists the memory must contain
		//   the section content.
		//
		std::unique_ptr<ASection> create_section(N_Core::BinaryBlob elf_blob, N_Core::BinaryBlob header_blob);

		// @brief Create an empty section.
		// 
		// @param is_64_bit		Create 32-bit or 64-bit header for section.
		//
		// @returns the section
		//
		std::unique_ptr<ASection> create_section(bool is_64_bit);

	}
}