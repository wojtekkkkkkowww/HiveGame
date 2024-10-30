#pragma once
#include <sqlite3.h>
#include <string>

/**
 * @brief The Database class is responsible for storing game results in a SQLite database.
 */
class DataBase
{
public:
    DataBase(const std::string& dbName);
    ~DataBase();

    bool insertGameResult(const std::string& ai1Name, const std::string& ai2Name, const std::string& winner);

private:
    sqlite3* db;
};