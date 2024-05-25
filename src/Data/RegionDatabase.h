#pragma once
#include <memory>
#include <string>

#include "Regions.h"

namespace gs
{

class RegionDatabase
{
public:
    explicit RegionDatabase(const std::string& folder);

private:
    std::string _folder;
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

