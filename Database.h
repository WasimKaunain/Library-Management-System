#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <iostream>
using namespace std;


class Database
{
private:
    sqlite3* db;
    string dbName;

public:
    Database(const string&);
    ~Database();

    //Disble copy (to avoid multiple handles for same DB)
    Database(const Database&) = delete;
    Database& operator = (const Database&) = delete;

    //DB access
    sqlite3* getDB();

    //Utility
    bool createTables();
    bool insertDefaultAdmin();
};

#endif 