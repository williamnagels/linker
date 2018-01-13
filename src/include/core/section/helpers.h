#pragma once
#include <cstdint>
#include <string>
namespace N_Core
{
	namespace N_Section
	{
		static const std::string invalid_section_index = "No section could be found with the index";

		// @brief Section identifier.
		// 
		// Identifies a unique section in the section table. Used in various API functions.
		//
		// Note: First section has index 0.
		//
		struct Index
		{
			using T = uint16_t;
			T _value;
			static const T Wildcard = std::numeric_limits<T>::max();
			operator decltype(_value)(){ return _value; }
			Index(T value) : _value(value) {}
		};
		// @brief Wether or not the index is the wildcard index.
		// 
		// Used in the API to check if the index of some section is the wildcard section.
		//
		bool is_wildcard(Index index);
	}
}
