#include "Database.h"
#include <sqlite3.h>
#include <iostream>
#include <string>
using namespace std;

Database::Database(const string& filename) : db(nullptr), dbName(filename)
{
    if ( sqlite3_open(filename.c_str(), &db) != SQLITE_OK)
        {
            cerr<<"Error Openeing Database"<<sqlite3_errmsg(db)<<endl;
            db = nullptr;
        } 
}

Database::~Database()
{
    if(db) sqlite3_close(db);
} 

sqlite3* Database::getDB()
{
    return db;
}

bool Database::createTables()
{
    // SQL for creating tables
    const char *createTables = 
        "CREATE TABLE IF NOT EXISTS Admin ("
        "adminID INTEGER PRIMARY KEY,"
        "adminPassword TEXT NOT NULL);"

        "CREATE TABLE IF NOT EXISTS Student ("
        "Name TEXT NOT NULL ,"
        "DOB TEXT NOT NULL,"
        "Gender TEXT NOT NULL,"
        "Mobile TEXT NOT NULL,"
        "Email TEXT PRIMARY KEY,"
        "Password TEXT NOT NULL);"

        "CREATE TABLE IF NOT EXISTS Books ("
        "BookID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "Name TEXT NOT NULL,"
        "Author TEXT NOT NULL,"
        "Genre TEXT NOT NULL,"
        "Price INTEGER NOT NULL);";
    char *errMsg = 0;
    int rc = sqlite3_exec(db, createTables, 0,0, &errMsg);
    if (rc != SQLITE_OK)
        {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg); 
            return false;
        }
    else 
        {
            sqlite3_free(errMsg);
            return true;
        }
}

bool Database::insertDefaultAdmin()
{
    char *errMsg = 0;
    // Insert default admin only if it doesn’t exist
    const char *insertAdmin = 
        "INSERT OR IGNORE INTO Admin (adminID, adminPassword) "
        "VALUES (24210115, 'Admin@123');";

    int rc = sqlite3_exec(db, insertAdmin, 0, 0, &errMsg);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    } 
    else 
    {
        sqlite3_free(errMsg);
        return true;
    }
}