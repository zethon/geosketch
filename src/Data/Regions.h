#pragma once
#include <vector>
#include <set>
#include <memory>
#include <any>

#include <nlohmann/json.hpp>
#include <fmt/format.h>

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
void to_json(nl::json& j, const Region& info);

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
    friend void to_json(nl::json& j, const Region& info);

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
    using Stats = std::map<std::string, std::any>;

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

    bool operator==(const Region& other) const;
    bool operator!=(const Region& other) const;

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
        : Region(name) {
        _type = RegionType::COUNTY;
    }
};

using ContinentPtr = std::shared_ptr<Continent>;
using CountryPtr = std::shared_ptr<Country>;
using StatePtr = std::shared_ptr<State>;
using CountyPtr = std::shared_ptr<County>;

} // namespace gs

namespace std
{
    std::ostream& operator<<(std::ostream& os, const gs::Region& region);

    template<>
    struct hash<gs::Region>
    {
        std::size_t operator()(const gs::Region& region) const
        {
            auto hash_string = fmt::format("{}_{}", region.type(), region.name());
            auto parent = region.parent();
            while (parent)
            {
                hash_string += fmt::format("_{}_{}", parent->type(), parent->name());
                parent = parent->parent();
            }

            return std::hash<std::string>{}(hash_string);
        }
    };
}