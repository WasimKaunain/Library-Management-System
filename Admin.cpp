#include "Database.h"
#include "Library.h"
#include "Admin.h"
#include "Utils.h"
#include "Student.h"
#include "Book.h"
#include <iostream>
#include <strings.h>
#include <sqlite3.h>

using namespace std;


void Admin::AdminLogin(Database& DB)
{
     clearScreen();
     Title("ADMIN  LOGIN");
      string userID;
      string password;
      cout<<"Enter Admin ID\n";
      cin>>userID;
      cout<<"Enter Admin Password\n";
      cin>>password;
    
     if(verifyAdmin(DB,userID,password))
      {
          cout<<"Login successfull..."<<endl;
          this_thread::sleep_for(chrono::milliseconds(2000));
          clearScreen();
          AdminMenu(DB);
      }

      else
      {
          cout<<"Invalid UserId or password"<<endl;
          this_thread::sleep_for(chrono::milliseconds(2000));
          clearScreen();
      }
}


void Admin::AdminMenu(Database& DB)
{
    int choice;
    Student S;
    Book B;
  while(1)
  {
        clearScreen();
        Title("ADMIN  PORTAL");
        cout<<"1) Display All Students Record\n2) Search a specific student\n3) Delete student Record\n4) Add Book\n5) Diplay All Books\n6) Search Book\n7) Delete Book\n8) Main Menu\n";
        
        cout<<endl<<"Enter your choice\n";
        cin>>choice;

        switch(choice)
         {
           case 1: clearScreen();
                   cout<<"Before Calling StudentList"<<endl;
                   S.ListStudentData(DB);
                   break;
       
           case 2: clearScreen();
                   S.SearchStudentData(DB);
                   break;
       
           case 3: clearScreen();
                   S.DeleteStudentData(DB);
                   break;
       
           case 4: clearScreen();
                   B.SetBookData();
                   if(B.StoreBookData(DB))
                    {
                        Message("Book added successfully...\n");
                        cout<<"Added time : "<<B.TIME;
                        this_thread::sleep_for(chrono::milliseconds(3000));
                        clearScreen();
                    }
                   else
                   {
                       cout<<"Data updation error..."<<endl;
                       this_thread::sleep_for(chrono::milliseconds(2000));
                       clearScreen();
                   } 
                   break;
       
           case 5: clearScreen();
                   B.ListBookData(DB);
                   break;
       
           case 6: clearScreen();
                   B.SearchBookData(DB);
                   break;
       
           case 7: clearScreen();
                   B.DeleteBookData(DB);
                   break;
       
           case 8: clearScreen();
                   HomePage(DB);
                   break;
       
           default:clearScreen();
                   cout<<"Invalid Choice\n";
                   this_thread::sleep_for(chrono::milliseconds(2000));
                   clearScreen();
          } 
                   cout<<"Enter 0 to go back to Admin Menu"<<endl;
                   fflush(stdin);
                       if('0'!= cin.get())
                       HomePage(DB);
           
  }
                  
}


bool Admin::verifyAdmin(Database& DB,const string& UID, const string& PASSWORD) 
    {
      sqlite3_stmt* stmt;
      const char* sql = "SELECT * FROM Admin WHERE adminID = ? AND adminPassword = ?;";

      // Prepare the SQL statement
      if (sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
          cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;
          return false;
      }

      // Bind parameters (UID, PASSWORD) to the statement
      sqlite3_bind_text(stmt, 1, UID.c_str(), -1, SQLITE_STATIC);
      sqlite3_bind_text(stmt, 2, PASSWORD.c_str(), -1, SQLITE_STATIC);

      // Execute the query and check if any row exists
      bool isValid = false;
      if (sqlite3_step(stmt) == SQLITE_ROW) {
          isValid = true;  // Found a matching admin
      }

      // Finalize the statement
      sqlite3_finalize(stmt);

      return isValid;
    }


void Admin::StoreAdminData(Database& DB)
  {
    // SQL query for inserting admin data
    string sql = "INSERT INTO Admin (AdminID, Password) VALUES ('" + 
                  this->adminId + "', '" + this->adminPassword + "');";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(DB.getDB(), sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) 
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else 
    {
        cout << "Admin Registered successfully..." << endl;
        cout<<"Redirecting to login page..."<<endl;
        this_thread::sleep_for(chrono::milliseconds(3000));
        // Clear screen
        clearScreen();
    }
  }


void Admin::setIdPassword()
  {
    cout<<"Enter Admin ID\n";
    cin>>adminId;
    cout<<"Enter Admin Password\n";
    cin>>adminPassword;
  }
