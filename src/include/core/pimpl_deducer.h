#pragma once
#include "src/include/core/general.h"

#include <memory>
#include <cstdint>

namespace N_Core
{
	/*@brief Wrapper to hold the generator interface and possible types.
	*
	* @param StrategyInterface Some interface all strategies implement
	* @param StrategyType Strategies to select from.
	*
	*/
	template <typename StrategyInterface, typename... StrategyType>
	struct PossibleGeneratorTypes {};


	/*@brief Select a strategy from a set of possible strategies.
	*
	* The selected strategy is the strategy from which the memory map type (::InnerType) has the same size as the memory_blob.
	*
	*/
	template<typename StrategyInterface, typename StrategyType, typename ... StrategyTypes>
	std::unique_ptr<StrategyInterface> pimpl_deducer(PossibleGeneratorTypes<StrategyInterface, StrategyType, StrategyTypes...> e, BinaryBlob memory_blob)
	{
		if (sizeof(StrategyType::InnerType) == memory_blob.size())
		{
			return std::make_unique<StrategyType>(memory_blob);
		}

		return pimpl_deducer(PossibleGeneratorTypes<StrategyInterface, StrategyTypes...>(), memory_blob);
	};

	template <typename StrategyInterface>
	std::unique_ptr<StrategyInterface> pimpl_deducer(PossibleGeneratorTypes<StrategyInterface>, BinaryBlob memory_blob)
	{ 
		throw std::invalid_argument("Cannot deduce generator to use. Size of header blob is of unexpected size."); 
		return nullptr;
	}
}