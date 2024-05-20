#pragma once
#include <memory>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace nl = nlohmann;

namespace gs
{

class Region;
using RegionPtr = std::shared_ptr<Region>;
void from_json(const nl::json& j, Region& info);

enum class RegionType
{
    COUNTRY,
    STATE,
    PROVINCE,
    TERRITORY,
    DEPENDENCY,
    REGION,
    CONTINENT
};

void from_json(const nl::json& j, RegionType& type);

class Region
{

public:
    Region(const std::string& name)
        : _name{name}
    {
        // nothing to do
    }

    std::string name() const { return _name; }

    friend void from_json(const nl::json& j, Region& info);

private:
    std::string _name;
    std::string _capital;

};

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
