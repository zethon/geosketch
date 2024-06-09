#pragma once
#include <memory>
#include <string>
#include <filesystem>

#include <sqlite3.h> 

#include "../GeoSketchLogger.h"

#include "Regions.h"


namespace gs
{

template<typename T>
concept RegionClassType = std::is_base_of<gs::Region, T>::value;

template<RegionClassType T>
class RegionRecord
{
    std::int64_t    _id;
    T::Ptr          _data;   

public:
    using Ptr = std::shared_ptr<RegionRecord<T>>;
    RegionRecord(std::int64_t recordId, T::Ptr data)
        : _id{ recordId }, _data{ data }
    {
    }

    T data() const { return _data; }

    std::uint64_t id() const { return _id; }
    void setId(std::uint64_t id) { _id = id; }
};

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


/// OLD CODE BELOW HERE

class RegionDB
{
    
public:
    RegionDB(const std::vector<std::string>& data);

    Region::Ptr current()
    {
        if (_index >= _countries.size()) return nullptr;
        return (_countries[_index]);
    }

    Region::Ptr next()
    {
        _index++;
        if (_index >= _countries.size()) return nullptr;
        return (_countries[_index]);
    }

private:
    Region::List _countries;
    std::size_t _index { 0 };
};

using RegionDBPtr = std::unique_ptr<RegionDB>;

} // namespace gs

