#pragma once
#include "src/include/core/section/section.h"
#include <list>
#include <functional>
template <typename T1, typename T2>
struct Segment
{
    std::list<std::reference_wrapper<N_Core::N_Section::Section<T1, T2>>> _sections;
};