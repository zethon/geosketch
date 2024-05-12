#pragma once
#include <string>
#include <vector>

namespace gs
{

class Region
{

public:
    Region(const std::string& name)
        : _name{name}
    {
        // nothing to do
    }

    std::string name() const { return _name; }

private:
    std::string _name;
};

class RegionDB
{
    
public:
    RegionDB(const std::vector<std::string>& data);

    void addRegion(const std::string& name);

    Region* current() 
    {
        if (_index >= _countries.size()) return nullptr;
        return &(_countries[_index]);
    }

    Region* next() 
    {
        _index++;
        if (_index >= _countries.size()) return nullptr;
        return &(_countries[_index]);
    }

private:
    std::vector<Region> _countries;
    std::size_t _index { 0 };
};

using RegionDBPtr = std::unique_ptr<RegionDB>;

} // namespace gs
