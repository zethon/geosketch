#include <filesystem>

#include "../GeoSketchLogger.h"

#include "RegionDatabase.h"

namespace gs
{

constexpr auto REGION_DATABASE_FILE = "region.db";

// you don't always know how many regions you will be iterating in a game
// sometimes you might a region-specific set of sub-regions
//    * sometimes you might want multiple region specific sub-regions (all of just North and South America)
//    * sometimes you might want to iterate all regions
//
// continents
//      countries
//          states/provinces/territories
//              counties

// support for iterating regions by:
//  * specific type (i.e. only countries)
//  * specific region with sunregions (i.e. only North America)
//  * all regions
//
// "all countries of North America" - SELECT * FROM country WHERE (parent = 'North America' AND parent.type = 'continent')
// "all continents and countries of South America" - SELECT * FROM continent WHERE name = 'South America' UNION SELECT * FROM country WHERE parent = 'South America'
// "all states of the United States" - SELECT * FROM state WHERE parent = 'United States'
// "all states of countries from North America" - SELECT * FROM state WHERE parent IN (SELECT name FROM country WHERE parent = 'North America')

RegionDatabase::RegionDatabase(const std::string& folder)
    : _folder{ folder }
{
}


RegionDatabaseCompiler::RegionDatabaseCompiler(const std::string& folder)
    : _folder{ folder },
      _logger{ log::initializeLogger("RegionDatabaseCompiler") }
{
}

auto RegionDatabaseCompiler::compile() -> Result
{
    // move existing db to backup (i.e. region.db.001, region.db.002, etc.)
    if (!backup())
    {
        _logger->error("cannot backup existing database");
        return Result::BACKUP_ERROR;
    }

    // create new db file
    if (!createNewDB())
    {
        _logger->error("cannot create new database");
        return Result::CREATE_ERROR;
    }

    // create tables
    // insert data from json
    // close db

    return Result();
}

bool RegionDatabaseCompiler::backup()
{
    return true;
}

bool RegionDatabaseCompiler::createNewDB()
{
    std::filesystem::path dbfile = _folder;
    dbfile /= REGION_DATABASE_FILE;

    int rc = sqlite3_open_v2(dbfile.string().c_str(), &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK)
    {
        _logger->error("cannot open database: {}", sqlite3_errmsg(_db));
        sqlite3_close(_db);
        return false;
    }

    return true;
}

} // namespace gs
