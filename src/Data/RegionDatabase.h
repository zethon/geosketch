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

enum class RegionType : std::uint8_t
{
    NONE = 0,
    CONTINENT,
    COUNTRY,
    STATE,
    COUNTY,
};

void from_json(const nl::json& j, RegionType& type);

class Region
    : public std::enable_shared_from_this<Region>
{
    friend void from_json(const nl::json& j, Region& info);

    RegionWeakPtr _parent;
    RegionList _children;
    std::string _name;

protected:
    RegionType _type = RegionType::NONE;

   explicit Region(const std::string& name)
        : _name{name}
    {
        // nothing to do
    }

public:
    std::string name() const { return _name; }
    RegionType type() const { return _type; }

    RegionList children() const { return _children; }
    void addChild(RegionPtr child)
    {
        _children.push_back(child);
    }

    template<class RegionT, typename... T>
    RegionPtr addChild(T... args)
    {
        RegionPtr child = std::make_shared<RegionT>(args...);
        child->setParent(shared_from_this());
        _children.push_back(child);
        return child;
    }

    RegionPtr parent() const { return _parent.lock(); }
    void setParent(RegionPtr parent)
    {
        _parent = parent;
    }
};

class Continent : public Region
{
public:
    explicit Continent(const std::string& name) 
        : Region(name) { _type = RegionType::CONTINENT; }
};

class Country : public Region
{
public:
    explicit Country(const std::string& name) 
        : Region(name) { _type = RegionType::COUNTRY; }
};

class State : public Region
{
public:
    explicit State(const std::string& name) 
        : Region(name) { _type = RegionType::STATE; }
};

class County : public Region
{
public:
    explicit County(const std::string& name) 
        : Region(name) { _type = RegionType::COUNTY; }
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
