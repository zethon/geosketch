#include "Regions.h"

namespace gs
{

// you don't always know how many regions you will be iterating in a game
// sometimes you might a region-specific set of sub-regions
//    * sometimes you might want multiple region specific sub-regions (all of just North and South America)
//    * sometimes you might want to iterate all regions
//
// continents
//      countries
//          states/provinces/territories
//              counties


bool Region::operator==(const Region& other) const
{
    if ((_type != other._type)
        || (_name != other._name)
        || (_children.size() != other._children.size()))
    {
        return false;
    }
    
    
    auto this_parent = parent();
    auto other_parent = other.parent();

    if ((!this_parent && other_parent)
        || (this_parent && !other_parent)) return false;

    assert((this_parent && other_parent) || (!this_parent && !other_parent));
    if (!this_parent) return true; // both parents are null
    if (*this_parent != *other_parent) return false;

    return true;
}

bool Region::operator!=(const Region& other) const
{
    return !(*this == other);
}

void from_json(const nl::json& j, RegionType& type)
{
    std::string s;
    j.get_to(s);

    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::toupper(c); });

    if (s == "CONTINENT")
    {
        type = RegionType::CONTINENT;
    }
    else if (s == "COUNTRY")
    {
        type = RegionType::COUNTRY;
    }
    else if (s == "STATE")
    {
        type = RegionType::STATE;
    }
    else if (s == "COUNTY")
    {
        type = RegionType::COUNTY;
    }
    else
    {
        throw std::runtime_error("Invalid region type");
    }
}

void to_json(nl::json& j, const Region& info)
{
    j = nl::json
    {
        {"name", info._name}
    };
}

void from_json(const nl::json& j, Continent& continent)
{
    j.at("name").get_to(continent._name);
    continent._type = j["type"].get<RegionType>();
    assert(continent._type == RegionType::CONTINENT);
}

void from_json(const nl::json& j, Country& country)
{
    j.at("name").get_to(country._name);
    country._type = j["type"].get<RegionType>();
    assert(country._type == RegionType::COUNTRY);
}

void from_json(const nl::json& j, State& state)
{
}

void from_json(const nl::json& j, County& county)
{
}

} // namespace gs

namespace std
{
    std::ostream& operator<<(std::ostream& os, const gs::Region& region)
    {
        os << nl::json{ region };
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const gs::RegionType& type)
    {
        switch (type)
        {
        case gs::RegionType::CONTINENT:
            os << "Continent";
            break;
        case gs::RegionType::COUNTRY:
            os << "Country";
            break;
        case gs::RegionType::STATE:
            os << "State";
            break;
        case gs::RegionType::COUNTY:
            os << "County";
            break;
        default:
            os << "Unknown";
            break;
        }

        return os;
    }
}
