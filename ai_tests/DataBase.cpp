// DataBase.cpp
#include "DataBase.hpp"
#include <iostream>

DataBase::DataBase(const std::string& dbName)
{
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
    else
    {
        const char* sqlCreateTable =
            "CREATE TABLE IF NOT EXISTS GameResults ("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "AI1 TEXT NOT NULL,"
            "AI2 TEXT NOT NULL,"
            "Result INTEGER);"; // Result 0, 1, 2

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sqlCreateTable, nullptr, nullptr, &errMsg) != SQLITE_OK)
        {
            std::cerr << "Failed to create table: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
    }
}

DataBase::~DataBase()
{
    if (db)
    {
        sqlite3_close(db);
    }
}

bool DataBase::insertGameResult(const std::string& ai1Name, const std::string& ai2Name, const std::string& result)
{
    std::string sql = "INSERT INTO GameResults (AI1, AI2, Restult) VALUES ('" + ai1Name + "', '" + ai2Name + "', '" + result + "');";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "Failed to insert game result: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}