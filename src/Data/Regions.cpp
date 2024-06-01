#include "Regions.h"

namespace std
{
    std::ostream& operator<<(std::ostream& os, const gs::Region& region)
    {
        os << nl::json{ region };
        return os;
    }
}

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

void from_json(const nl::json& j, Region& region)
{
    j.at("name").get_to(region._name);
}

void to_json(nl::json& j, const Region& info)
{
    j = nl::json{ {"name", info._name} };
}

void from_json(const nl::json& j, RegionType& type)
{
    std::string s;
    j.get_to(s);
    if (s == "CONTINENT") type = RegionType::CONTINENT;
    else if (s == "COUNTRY") type = RegionType::COUNTRY;
    else if (s == "STATE") type = RegionType::STATE;
    else if (s == "COUNTY") type = RegionType::COUNTY;

    throw std::runtime_error("Invalid region type");
}

}