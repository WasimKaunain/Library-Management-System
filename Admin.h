#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <ctime>    // for struct tm
#include "Book.h"   // forward declare or include Book

class Admin 
{
private:
    std::string adminId;
    std::string adminPassword;

public:
    bool verifyAdmin(Database& DB,const string&,const string&);
    void StoreAdminData(Database&);
    void setIdPassword();
    void AdminMenu(Database&);
    void AdminLogin(Database&);
};

#endif 