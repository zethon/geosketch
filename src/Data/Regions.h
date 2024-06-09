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

enum class RegionType : std::uint8_t
{
    NONE = 0,
    CONTINENT,
    COUNTRY,
    STATE,
    COUNTY,
};

class Region
    : public std::enable_shared_from_this<Region>
{
    friend void to_json(nl::json& j, const Region& info);

public:
    using Ptr = std::shared_ptr<Region>;
    using WeakPtr = std::weak_ptr<Region>;
    using List = std::vector<Ptr>;
    using Set = std::set<Ptr>;
    using Stats = std::map<std::string, std::any>;

    std::string name() const { return _name; }
    RegionType type() const { return _type; }

    Region::List children() const { return _children; }
    void addChild(Region::Ptr child)
    {
        _children.push_back(child);
    }

    template<class RegionT, typename... T>
    Region::Ptr addChild(T... args)
    {
        Region::Ptr child = std::make_shared<RegionT>(args...);
        child->setParent(shared_from_this());
        _children.push_back(child);
        return child;
    }

    Region::Ptr parent() const { return _parent.lock(); }
    void setParent(Region::Ptr parent)
    {
        _parent = parent;
    }

    bool operator==(const Region& other) const;
    bool operator!=(const Region& other) const;

protected:
    RegionType _type = RegionType::NONE;

    Region() = default;

    explicit Region(const std::string& name)
        : _name{ name }
    {
        // nothing to do
    }

    Region::WeakPtr _parent;
    Region::List _children;
    std::string _name;

};

class Continent : public Region
{
    friend void from_json(const nl::json& j, Continent& info);

public:
    using Ptr = std::shared_ptr<Continent>;

    Continent() = default;

    explicit Continent(const std::string& name) 
        : Region(name) { _type = RegionType::CONTINENT; }
};

class Country : public Region
{
    friend void from_json(const nl::json& j, Country& info);

public:
    using Ptr = std::shared_ptr<Country>;

    Country() = default;

    explicit Country(const std::string& name) 
        : Region(name) { _type = RegionType::COUNTRY; }
};

class State : public Region
{
    friend void from_json(const nl::json& j, State& info);

public:
    using Ptr = std::shared_ptr<State>;

    State() = default;

    explicit State(const std::string& name) 
        : Region(name) { _type = RegionType::STATE; }
};

class County : public Region
{
    friend void from_json(const nl::json& j, County& info);

public:
    using Ptr = std::shared_ptr<County>;

    County() = default;

    explicit County(const std::string& name)
        : Region(name) {
        _type = RegionType::COUNTY;
    }
};

void from_json(const nl::json& j, RegionType& type);

void from_json(const nl::json& j, Continent& continent);
void from_json(const nl::json& j, Country& country);
void from_json(const nl::json& j, State& state);
void from_json(const nl::json& j, County& county);

} // namespace gs

namespace std
{
    std::ostream& operator<<(std::ostream& os, const gs::Region& region);
    std::ostream& operator<<(std::ostream& os, const gs::RegionType& type);

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