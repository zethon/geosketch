#include <filesystem>
#include <fstream>

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


RegionDatabaseCompiler::RegionDatabaseCompiler(const std::string& folder, const std::string& source)
    : _folder{ folder },
      _source{ source },
      _dbfile{ std::filesystem::path{folder} / REGION_DATABASE_FILE },
      _logger{ log::initializeLogger("RegionDatabaseCompiler") }
{
}

auto RegionDatabaseCompiler::compile() -> Result
{
    _logger->info("compiling region database from source: {}", _source);
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
    if (!createTables())
    {
        _logger->error("cannot create tables");
        return Result::CREATE_TABLES_ERROR;
    }

    // insert data from json
    if (!importData())
    {
        _logger->error("cannot import data");
        return Result::DATA_IMPORT_ERROR;
    }

    // close db
    sqlite3_close(_db);

    return Result();
}

bool RegionDatabaseCompiler::backup()
{
    // see https://github.com/zethon/geosketch/issues/4 for how to enhance
    // this function to keep multiple backups
    try
    {
        const auto _dbfile_backup = std::filesystem::path{ _dbfile.string() + ".backup" };
        if (std::filesystem::exists(_dbfile_backup))
        {
            std::filesystem::remove(_dbfile_backup);
        }
    }
    catch (const std::exception& e)
    {
        _logger->error("cannot remove existing backup: {}", e.what());
        return false;
    }

    try
    {
        if (std::filesystem::exists(_dbfile))
        {
            auto backup = _dbfile;
            backup += ".backup";
            std::filesystem::rename(_dbfile, backup);
        }
    }
    catch (const std::exception& e)
    {
        _logger->error("cannot rename existing database: {}", e.what());
        return false;
    }

    return true;
}

namespace
{

constexpr auto CREATE_REGION_TABLE = R"(
CREATE TABLE "region" (
    "id" INTEGER NOT NULL UNIQUE,
    "parent" INTEGER,
    "name" TEXT,
    "type" TEXT,
    PRIMARY KEY("id" AUTOINCREMENT)
    );
)";

constexpr auto CREATE_FACTOID_TABLE = R"(
CREATE TABLE "factoid" (
    "id" INTEGER NOT NULL UNIQUE,
    "rid" INTEGER NOT NULL,
    "text" TEXT NOT NULL,
    PRIMARY KEY("id" AUTOINCREMENT)
    );
)";

constexpr auto CREATE_STATS_TABLE = R"(
CREATE TABLE "stats" (
    "id" INTEGER NOT NULL UNIQUE,
    "rid" INTEGER,
    "name" TEXT,
    "value" TEXT,
    "order" INTEGER,
    PRIMARY KEY("id" AUTOINCREMENT)
    );
)";


std::string hashString(const gs::Region& value) 
{
    std::size_t hashValue = std::hash<gs::Region>{}(value);
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hashValue;
    return ss.str();
}

}

bool RegionDatabaseCompiler::createNewDB()
{
    int rc = sqlite3_open_v2(_dbfile.string().c_str(), &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK)
    {
        _logger->error("cannot open database: {}", sqlite3_errmsg(_db));
        sqlite3_close(_db);
        _db = nullptr;
        return false;
    }

    _logger->debug("created database: {}", _dbfile.string());
    return true;
}

bool RegionDatabaseCompiler::createTables()
{
    assert(_db);
    char* errMsg = 0;

    auto result = sqlite3_exec(_db, CREATE_REGION_TABLE, 0, 0, &errMsg);
    if (result != SQLITE_OK)
    {
        _logger->error("Error creating REGION table: {}", errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    result = sqlite3_exec(_db, CREATE_FACTOID_TABLE, 0, 0, &errMsg);
    if (result != SQLITE_OK)
    {
        _logger->error("Error creating FACTOID table: {}", errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    result = sqlite3_exec(_db, CREATE_STATS_TABLE, 0, 0, &errMsg);
    if (result != SQLITE_OK)
    {
        _logger->error("Error creating STATS table: {}", errMsg);
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

// -r F:\src\lulzapps\geosketch\resources -w  1360x765 --mute  -c F:\src\lulzapps\geosketch\geosketch-data\region-data

bool RegionDatabaseCompiler::importData()
{
    static const std::vector<std::string> special_fields = 
        { "parent", "type", "factoids", "subregions", "name" };

    std::vector<nl::json> region_files;

    for (const auto& file : std::filesystem::directory_iterator(_source))
    {
        if (file.is_regular_file() && file.path().extension() == ".dat")
        {
            _logger->info("importing data from file: {}", file.path().string());
            std::ifstream ifs(file.path().string().c_str());
            if (!ifs.is_open())
            {
                _logger->error("cannot open file: {}", file.path().string());
                continue;
            }

            try
            {
                region_files.emplace_back(nl::json::parse(ifs));
            }
            catch (const std::exception& e)
            {
                _logger->error("cannot parse json from file: {}", e.what());
                continue;
            }
        }
    }
    _logger->debug("loaded {} region files", region_files.size());

    using ContentRecordPtr = gs::RegionRecord<gs::Continent>::Ptr;
    std::map<std::string, ContentRecordPtr> regions;

    constexpr auto INSERT_REGION = R"(INSERT INTO region (name, type, parent) VALUES (?, ?, ?);)";

    // process continents
    for (const auto& jdat : region_files)
    {
        if (jdat["region"].is_null())
        {
            _logger->error("no region data in json");
            continue;
        }

        const auto& region = jdat["region"];
        if (region["type"].is_null() || region["type"].get<std::string>() != "continent")
        {
            continue;
        }

        auto continent = std::make_shared<gs::Continent>();
        *continent = jdat["region"].get<gs::Continent>();

        _logger->debug("processing continent: {}", region["name"].get<std::string>());
        for (const auto& [key,value] : region.items())
        {
            // make sure that `key` is not in `special_fields`
            if (std::find(special_fields.begin(), special_fields.end(), key) != special_fields.end())
            {
                continue;
            }

            _logger->debug("key: {}, value: {}", key, value.dump());
        }

        // insert continent into database
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(_db, INSERT_REGION, -1, &stmt, nullptr) != SQLITE_OK)
        {
            _logger->error("cannot prepare statement: {}", sqlite3_errmsg(_db));
            return false;
        }

        if (sqlite3_bind_text(stmt, 1, continent->name().c_str(), static_cast<int>(continent->name().size()), SQLITE_TRANSIENT) != SQLITE_OK
            || sqlite3_bind_text(stmt, 2, continent->typeString().c_str(),static_cast<int>(continent->typeString().size()), SQLITE_TRANSIENT) != SQLITE_OK
            || sqlite3_bind_int(stmt, 3, 456) != SQLITE_OK)
        {
            _logger->error("cannot bind values: {}", sqlite3_errmsg(_db));
            sqlite3_finalize(stmt);
            return false;
        }
        
        if (sqlite3_step(stmt) != SQLITE_DONE) 
        {
            _logger->error("cannot execute statement: {}", sqlite3_errmsg(_db));
            sqlite3_finalize(stmt);
            return false;
        }

        sqlite3_finalize(stmt);

        const auto lastInsertId = sqlite3_last_insert_rowid(_db);
        _logger->debug("added {} '{}' as record ID {}", continent->typeString(), continent->name(), lastInsertId);

        regions[hashString(*continent)] = std::make_shared<gs::RegionRecord<gs::Continent>>(lastInsertId, continent);
    }


     return true;
}

} // namespace gs
