#pragma once
#include "src/include/core/section/section.h"
#include <list>
#include <functional>


namespace N_Core
{
    namespace N_Segment
    {
        template <typename T1, typename T2>
        struct Segment
        {
            Segment(uint64_t virtual_address, uint64_t offset){}
            std::list<std::reference_wrapper<N_Core::N_Section::Section<T1, T2>>> _sections;

            uint64_t get_offset(){return 0;}
        };
    }
}
