#pragma once
#include <string>
#include <vector>
#include <optional>
extern "C" { int yyparse(void* parser);}
namespace N_Core
{
    namespace N_Parser
    {
        using SegmentContainerTy = std::vector<std::string>;
        struct Segment
        {
            std::string _name;
            SegmentContainerTy _filters;
            SegmentContainerTy::const_iterator begin() const;
            SegmentContainerTy::const_iterator end() const;
            SegmentContainerTy::iterator begin();
            SegmentContainerTy::iterator end();
            SegmentContainerTy::value_type & operator[](std::size_t idx);
            SegmentContainerTy::value_type const& operator[](std::size_t idx) const;
            std::optional<uint64_t> _address;
        };
        
        using ContainerTy = std::vector<Segment>;
        class Parser
        {
            public:
                Parser(std::string const& script_name);
                ContainerTy::const_iterator begin() const;
                ContainerTy::const_iterator end() const;
                ContainerTy::iterator begin();
                ContainerTy::iterator end();
                ContainerTy::value_type & operator[](std::size_t idx);
                ContainerTy::value_type const& operator[](std::size_t idx) const;
            private:
                Segment _segment_under_construction;
                ContainerTy _segments;
                std::optional<uint64_t> _base_address;
                
                friend int ::yyparse(void* parser);
                // called by script parser; reason why this method
                // is a friend;
                int parse(std::string const& path);
                void set_segment_name(std::string const& name);
                void add_filter(std::string const& filter_name);
                void set_base_address(std::string const& base_address);
        };

    }
}