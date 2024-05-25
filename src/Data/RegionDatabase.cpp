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



RegionDatabase::RegionDatabase(const std::string& folder)
    : _folder{ folder }
{
}


} // namespace gs
