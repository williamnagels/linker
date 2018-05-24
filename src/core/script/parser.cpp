#include "src/include/core/script/parser.h"

N_Core::N_Parser::Parser::Parser(std::string const& script_name)
{
    parse(script_name);
}

N_Core::N_Parser::ContainerTy::const_iterator N_Core::N_Parser::Parser::begin() const{return std::begin(_segments);}
N_Core::N_Parser::ContainerTy::const_iterator N_Core::N_Parser::Parser::end() const{return std::end(_segments);}
N_Core::N_Parser::ContainerTy::iterator N_Core::N_Parser::Parser::begin(){return std::begin(_segments);}
N_Core::N_Parser::ContainerTy::iterator N_Core::N_Parser::Parser::end(){return std::end(_segments);}

N_Core::N_Parser::ContainerTy::value_type & N_Core::N_Parser::Parser::operator[](std::size_t idx)
{
    return _segments[idx]; 
}
N_Core::N_Parser::ContainerTy::value_type const& N_Core::N_Parser::Parser::operator[](std::size_t idx) const 
{ 
    return _segments[idx]; 
}

N_Core::N_Parser::SegmentContainerTy::const_iterator N_Core::N_Parser::Segment::begin() const
{
    return std::begin(_filters);
}
N_Core::N_Parser::SegmentContainerTy::const_iterator N_Core::N_Parser::Segment::end() const
{
    return std::end(_filters); 
}
N_Core::N_Parser::SegmentContainerTy::iterator N_Core::N_Parser::Segment::begin()
{
    return std::begin(_filters);
}
N_Core::N_Parser::SegmentContainerTy::iterator N_Core::N_Parser::Segment::end()
{
    return std::end(_filters); 
}
N_Core::N_Parser::SegmentContainerTy::value_type & N_Core::N_Parser::Segment::operator[](std::size_t idx)
{
    return _filters[idx];
}
N_Core::N_Parser::SegmentContainerTy::value_type const& N_Core::N_Parser::Segment::operator[](std::size_t idx) const
{
    return _filters[idx];
}

void N_Core::N_Parser::Parser::set_segment_name(std::string const& name)
{
    _segment_under_construction._name = name;

    if (_base_address.has_value())
    {
        _segment_under_construction._address = *_base_address;
        _base_address.reset();
    }
    _segment_under_construction._name = name;
    _segments.push_back(_segment_under_construction);
    _segment_under_construction._filters.clear();
}

void N_Core::N_Parser::Parser::add_filter(std::string const& filter)
{
    _segment_under_construction._filters.push_back(filter);
}

void N_Core::N_Parser::Parser::set_base_address(std::string const& name)
{
    _base_address = std::stoul(name, nullptr, 16);
}