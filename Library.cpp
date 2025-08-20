#include "Database.h"
#include "Library.h"
#include "Utils.h"
#include "Student.h"
#include "Admin.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sqlite3.h>
using namespace std;

void HomePage(Database&);
void StudentPortal(Database&);
void AdminPortal(Database&);


void AdminPortal(Database& DB)
{
    
    Admin A;
    clearScreen();
    Title("ADMIN  PORTAL"); 
    
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM Admin;";

    if(sqlite3_prepare_v2(DB.getDB(),sql,-1,&stmt,nullptr) != SQLITE_OK)
    {
      cerr<<"Failed to prepare statement"<<sqlite3_errmsg(DB.getDB())<<endl;
      return;
    }

    if(sqlite3_step(stmt) == SQLITE_ROW) 
      {
        sqlite3_finalize(stmt);
        A.AdminLogin(DB);
        return;
      }
    
    else
    {
      sqlite3_finalize(stmt);
      A.setIdPassword();
      A.StoreAdminData(DB);
      A.AdminLogin(DB);
    }
}


 
void StudentPortal(Database& DB)
{
    int choice;
    Student S;
    clearScreen();
    
    Title("STUDENT  PORTAL");
    cout<<"1) Login\n\n2) Signup\n\n3) Home Page"<<endl;
    cin>>choice;
    switch(choice)
    {
        case 1: S.Login(DB);
                break;

        case 2: S.Signup(DB);
                break;

        case 3: return;

        default:cout<<"Inavalid Choice\nEnter valid choice\n";
                this_thread::sleep_for(chrono::milliseconds(2000));
                clearScreen();
                return;
    }
}

void HomePage(Database& DB)
{
    int choice;
    clearScreen();
    Title("WELCOME TO MINI LIBRARY MANAGEMENT");
    cout<<"1) Admin Portal\n\n2) Student Portal\n\n3) Exit\n\n";

    cout<<"Enter your choice\n";
    cin>>choice;

    switch(choice)
    {
    case 1: clearScreen();
            AdminPortal(DB);
            break;

    case 2: clearScreen();
            StudentPortal(DB);
            break;

    default: cout<<"Invalid Choice\n";
            break;
    
    case 3: clearScreen();
            this_thread::sleep_for(chrono::milliseconds(2000));
            cout<<"W E L C O M E   A G A I N\n";
            exit(0);

   
    }
}



int main() 
{
    Database DB("libray.db");

    if(DB.createTables())
      cout<<"Tables created successfully"<<endl;

    if(DB.insertDefaultAdmin())
      cout<<"Default Admin credentials inserted successfully"<<endl;

    HomePage(DB);
    return 0;
}