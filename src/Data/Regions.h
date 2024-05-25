#pragma once
#include <vector>
#include <set>
#include <memory>

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

} // namespace gs

namespace std
{
    std::ostream& operator<<(std::ostream& os, const gs::Region& region);
}