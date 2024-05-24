#pragma once
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <nlohmann/json.hpp>

namespace nl = nlohmann;

namespace gs
{

// continents
//      countries
//          states/provinces/territories
//              counties

class Region;
using RegionPtr = std::shared_ptr<Region>;
using RegionWeakPtr = std::weak_ptr<Region>;
using RegionList = std::vector<RegionPtr>;
using RegionSet = std::set<RegionPtr>;

void from_json(const nl::json& j, Region& info);

enum class RegionType
{
    CONTINENT,
    COUNTRY,
    STATE,
    COUNTY
};

void from_json(const nl::json& j, RegionType& type);

class Region
{

public:
    explicit Region(const std::string& name)
        : _name{name}
    {
        // nothing to do
    }

    std::string name() const { return _name; }

    friend void from_json(const nl::json& j, Region& info);

private:
    RegionWeakPtr _parent;
    RegionList _children;

    std::string _name;

};

class Continent : public Region
{
public:
    using Region::Region;
};

class Country : public Region
{
public:
    using Region::Region;
};

class State : public Region
{
public:
    using Region::Region;
};

class County : public Region
{
public:
    using Region::Region;
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
