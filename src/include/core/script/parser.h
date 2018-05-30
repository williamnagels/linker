#pragma once
#include <string>
#include <vector>
#include <optional>
#include <regex>
extern "C" { int yyparse(void* parser);}
namespace N_Core
{
    namespace N_Parser
    {
        using SegmentContainerTy = std::vector<std::regex>;

        
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

        
        bool is_valid(std::string const& name, Segment const& segment);

        using ContainerTy = std::vector<Segment>;
        class Parser
        {
            public:
                using iterator = ContainerTy::iterator;
                using const_iterator = ContainerTy::const_iterator;
                using value_type = ContainerTy::value_type;
                Parser(std::string const& script_name);
                const_iterator begin() const;
                const_iterator end() const;
                iterator begin();
                iterator end();
                value_type & operator[](std::size_t idx);
                value_type const& operator[](std::size_t idx) const;
                
            private:
                Segment _segment_under_construction;
                ContainerTy _segments;
                std::optional<uint64_t> _base_address;
                
                friend int ::yyparse(void* parser);
                // called by script parser; reason why this method
                // is a friend;
                void parse(std::string const& path);
                void set_segment_name(std::string const& name);
                void add_filter(std::string filter_name);
                void set_base_address(std::string const& base_address);
        };

    }
}