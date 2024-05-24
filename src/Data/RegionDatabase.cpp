#include "RegionDatabase.h"

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

void from_json(const nl::json& j, Region& region)
{
    j.at("name").get_to(region._name);
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


RegionDatabase::RegionDatabase(const std::string& folder)
    : _folder{ folder }
{
}


} // namespace gs
