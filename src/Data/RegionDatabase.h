#pragma once
#include <memory>
#include <string>
#include <filesystem>

#include <sqlite3.h> 

#include "../GeoSketchLogger.h"

#include "Regions.h"


namespace gs
{

class RegionDatabase
{
public:
    explicit RegionDatabase(const std::string& folder);

private:
    std::string _folder;
};

class RegionDatabaseCompiler
{
public:
    enum class Result
    {
        SUCCESS = 0,
        UNKNOWN_ERROR,
        BACKUP_ERROR,
        CREATE_ERROR,
        CREATE_TABLES_ERROR,
        DATA_IMPORT_ERROR,
    };

    explicit RegionDatabaseCompiler(const std::string& folder, const std::string& source);
    
    
    Result compile();

private:
    bool backup();
    bool createNewDB();
    bool createTables();
    bool importData();

    sqlite3* _db = nullptr;
    std::string _folder;
    std::string _source;

    std::filesystem::path   _dbfile;
    log::SpdLogPtr          _logger;
};


class RegionDB
{
    
public:
    RegionDB(const std::vector<std::string>& data);

    RegionPtr current()
    {
        if (_index >= _countries.size()) return nullptr;
        return (_countries[_index]);
    }

    RegionPtr next()
    {
        _index++;
        if (_index >= _countries.size()) return nullptr;
        return (_countries[_index]);
    }

private:
    std::vector<RegionPtr> _countries;
    std::size_t _index { 0 };
};

using RegionDBPtr = std::unique_ptr<RegionDB>;

} // namespace gs

