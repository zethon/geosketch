#include "RegionDatabase.h"

namespace gs
{

void from_json(const nl::json& j, Region& region)
{
    j.at("name").get_to(region._name);
}

void from_json(const nl::json& j, RegionType& type)
{
    std::string s;
    j.get_to(s);
    if (s == "COUNTRY") type = RegionType::COUNTRY;
    else if (s == "STATE") type = RegionType::STATE;
    else if (s == "PROVINCE") type = RegionType::PROVINCE;
    else if (s == "TERRITORY") type = RegionType::TERRITORY;
    else if (s == "DEPENDENCY") type = RegionType::DEPENDENCY;
    else if (s == "REGION") type = RegionType::REGION;
    else if (s == "CONTINENT") type = RegionType::CONTINENT;
}


// dynamic source of records
// load-ahead cache
RegionDatabase::RegionDatabase(const std::string& folder)
    : _folder{ folder }
{
}


} // namespace gs
