#pragma once
#include <string>
#include <vector>
#include <optional>

namespace N_Core
{
    namespace N_Parser
    {
        class Parser
        {
            struct Segment
            {
                std::string _name;
                std::vector<std::string> _filters;
                uint64_t _address;
                bool is_valid(){return !_name.empty();}
            };

            std::vector<Segment> _segments;
            std::optional<uint64_t> _base_address;
            public:
                int parse();
                void set_segment_name(std::string const& name);
                void add_filter(std::string const& filter_name);
                void set_base_address(std::string const& base_address);
        };

    }
}