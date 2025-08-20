#include "Book.h"
#include "Library.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <sqlite3.h>
#include <chrono>
#include <thread>
using namespace std;

void Book::SetBookData()
{
  clearScreen();
  Title("SET  BOOKDATA");

  cout<<"Enter Book name\n";
  fflush(stdin);
  getline(cin,BookName);
  fflush(stdin);
  cout<<"\nEnter Book Genre\n";
  cin>>BookGenre;
  fflush(stdin);
  cout<<"\nEnter Book Author\n";
  getline(cin,BookAuthor);
  fflush(stdin);
  cout<<"\nEnter BookId\n";
  cin>>BookID;
  fflush(stdin);
  cout<<"\nEnter Book Price\n";
  fflush(stdin);
  cin>>BookPrice;

  time_t t;
  time(&t);
  BookInclusionTime=ctime(&t);
}



void Book::ListBookData(Database& DB)
{
  clearScreen();
  Title("LIST  OF  BOOKS");

  sqlite3_stmt* stmt;
  int count =0;
  const char* sql = "SELECT * FROM Book;";

  if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
  {
    cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;
    return;
  }

  while(true)
  {
      int rc = sqlite3_step(stmt);
      
      if (rc == SQLITE_ROW) 
      {
          BookID    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
          BookName  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
          BookAuthor     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
          BookGenre  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
          BookPrice = sqlite3_column_int(stmt, 4);
          count++;
      }

      else if (rc == SQLITE_DONE) 
        {
          cout<<endl<<"Total "<<count<<" Book's data fetched : "<<count<<endl;
          break;
        }
      else 
        {
            cerr << "Error fetching row: " << sqlite3_errmsg(DB.getDB()) << endl;
            break;
        }
  }
  sqlite3_finalize(stmt);
  char ch;
  cout<<"Enter 'Y/y' to go back :"<<endl;
  cin>>ch;
  if(ch == 'Y' || 'y')
    return;
  else
    HomePage(DB);
}



void Book::DeleteBookData(Database& DB)
{
  int choice,rc;
  string value;
  
  sqlite3_stmt* stmt;
  sqlite3_stmt* delStmt;
  char confirm,ch;
  const char* sql = nullptr;
  
  clearScreen();
  Title("DELETE  BOOK");

  cout<<"Delete Book By :"<<endl<<endl;
  cout<<"1) Book Name\t\t\t2) Book ID"<<endl;
  cout<<"Enter your choice"<<endl;
  cin>>choice;

  switch(choice)
    {
      case 1: cout<<"Enter Book Name"<<endl;
              cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
              getline(cin,value);
  
              sql = "SELECT * FROM Book WHERE Name = ?;";

              if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
                cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;
              
              sqlite3_bind_text(stmt,1,value.c_str(),-1,SQLITE_STATIC);
              
              rc = sqlite3_step(stmt);
              if (rc == SQLITE_ROW) 
                  {
                      BookID    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                      BookName  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                      BookAuthor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                      BookID  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                      BookPrice = sqlite3_column_int(stmt, 4);
                      ShowBookData();

                      cout<<endl<<"Do you really want to delete this Book."<<endl<<"Enter 'Y'/'y' to confirm."<<endl;
                      cin>>confirm;
                        if (confirm == 'y' || 'Y')
                            {
                              const char* delSql = "DELETE FROM Book WHERE Name = ?;";
                            
                              if(sqlite3_prepare_v2(DB.getDB(),delSql,-1,&delStmt,nullptr) != SQLITE_OK) 
                                  cerr<<"Failed to prepare delete statement : "<<sqlite3_errmsg(DB.getDB())<<endl;
                              
                              sqlite3_bind_text(delStmt,1,value.c_str(),-1,SQLITE_STATIC);
                              
                              rc = sqlite3_step(delStmt);
                              if(rc != SQLITE_DONE) cerr<<"Failed to delete book record : "<<sqlite3_errmsg(DB.getDB())<<endl;
                              else cout<<"Record Deleted successfully"<<endl;
                            }
                          else
                              cout<<"Unknown choice"<<endl;
                  }
              else 
                  cerr << "Error fetching row: " << sqlite3_errmsg(DB.getDB()) << endl;
              
              sqlite3_finalize(stmt);
              if (delStmt) sqlite3_finalize(delStmt);

              cout<<"Enter 'Y/y' to go back to AdminMenu"<<endl;
              cin>>ch;
              if(ch == 'Y' || 'y')
                return;
              else
                HomePage(DB);

              break;
              
      case 2: cout<<"Enter Book ID"<<endl;
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              getline(cin,value);

              sql = "SELECT * FROM Book WHERE BookID = ?;";

              if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
                cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;
              
              sqlite3_bind_text(stmt,1,value.c_str(),-1,SQLITE_STATIC);
              
              rc = sqlite3_step(stmt);
              if (rc == SQLITE_ROW) 
              {
                  BookID    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                  BookName  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                  BookAuthor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                  BookID  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                  BookPrice = sqlite3_column_int(stmt, 4);
                  ShowBookData();

                  cout<<endl<<"Do you really want to delete this Book record."<<endl<<"Enter 'Y'/'y' to confirm."<<endl;
                  cin>>confirm;
                  if (confirm == 'y' || 'Y')
                      {
                        const char* delSql = "DELETE FROM Book WHERE BookID = ?;";
                      
                        if(sqlite3_prepare_v2(DB.getDB(),delSql,-1,&delStmt,nullptr) != SQLITE_OK) 
                            cerr<<"Failed to prepare delete statement : "<<sqlite3_errmsg(DB.getDB())<<endl;
                        
                        sqlite3_bind_text(delStmt,1,value.c_str(),-1,SQLITE_STATIC);
                        
                        rc = sqlite3_step(delStmt);

                        if(rc != SQLITE_DONE) cerr<<"Failed to delete book record : "<<sqlite3_errmsg(DB.getDB())<<endl;
                        else cout<<"Record Deleted successfully"<<endl;
                      }
                    else
                        cout<<"Unknown choice"<<endl;

              }
            
              else 
                 cerr << "Error fetching row: " << sqlite3_errmsg(DB.getDB()) << endl;
              
              sqlite3_finalize(stmt);
              if (delStmt) sqlite3_finalize(delStmt);

              cout<<"Enter 'Y/y' to go back to AdminMenu"<<endl;
              cin>>ch;
              if(ch == 'Y' || 'y')
                return;
              else
                HomePage(DB);       
                
              break;

      default :cout<<"Invalid Choice"<<endl;
               this_thread::sleep_for(chrono::milliseconds(2000));
               clearScreen();
   }
 
}




void Book::GenerateToken(int value,int day,int penalty)
{
  clearScreen();
  Title("BOOK   TOKEN");
  ShowBookData();

  cout<<"Token no. : "<<rand()<<endl;
  cout<<"Time      : "<<asctime(Time())<<endl<<endl;
  if(value==1)
  {
  cout<<"1) Print this Token for further use."<<endl;
  cout<<"2) Using this Token You can borrow this Book  Library."<<endl;
  cout<<"3) One should have to return the borrowed book within 30 days."<<endl;
  cout<<"4) After 30 days, Late fine will be charged for each book on account of 2 Rupee per day."<<endl;
  cout<<"5) Only the borrower will responsible in case of damage or stolen of Books and he will have to pay the amount equal to the price of book."<<endl;
  cout<<endl<<"\t\t\t\t\t\t  T H A N K   Y O U"<<endl;
  }

  if(value==2)
  {
  cout<<"You are "<<day<<" late in returning this book"<<endl;
  cout<<"Penalty of Rs."<<penalty<<" is charged on you"<<endl;
  cout<<"Kindly submit this Book ASAP along with the Penalty Charges"<<endl<<endl;
  cout<<"1) Print this Token for further use."<<endl;
  cout<<"2) Using this Token You can Return this Book to Library."<<endl;
  cout<<"3) One should have to return the borrowed book within 30 Days."<<endl;
  cout<<"4) After 30 Days, Late fine will be charged for each book on account of 2 Rupee per day."<<endl;
  cout<<"5) Only the borrower will responsible in case of damage or stolen of Books and he will have to pay the amount equal to the price of book."<<endl;
  cout<<endl<<"\t\t\t\t\t\t  T H A N K   Y O U"<<endl;
  }

  cout<<endl<<endl<<"Press 0 to go to Student Menu"<<endl;
  fflush(stdin);
  cin.get();
}



void Book::SearchBookData(Database& DB)
{
  int choice,count=0,rc;
  string value;
  
  sqlite3_stmt* stmt;
  const char* sql = nullptr;
  char ch;
  
  clearScreen();
  Title("SEARCH  BOOK");

  cout<<"Search Book By :"<<endl<<endl;
  cout<<"1)Name\t\t\t2) BookID\t\t\t3) Author"<<endl;
  cout<<"Enter your choice"<<endl;
  cin>>choice;

  switch(choice)
    {
      case 1: cout<<"Enter Book Name"<<endl;
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              getline(cin,value);
  
              sql = "SELECT * FROM Book WHERE Name = ?;";

              if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
                cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;
              
              sqlite3_bind_text(stmt,1,value.c_str(),-1,SQLITE_STATIC);
              
              while(true)
              {
                  rc = sqlite3_step(stmt);

                  if (rc == SQLITE_ROW) 
                  {
                      BookID  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                      BookName    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                      BookAuthor     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                      BookGenre  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                      BookPrice = sqlite3_column_int(stmt, 4);
                      ShowBookData();
                      count++;
                  }
                
                  else if (rc == SQLITE_DONE) 
                    {
                      cout<<endl<<count<<" record/s were found with same name."<<endl;
                      break;
                    }
                  else 
                    {
                        cerr << "Error fetching row: " << sqlite3_errmsg(DB.getDB()) << endl;
                        break;
                    }
              }
              sqlite3_finalize(stmt);
              
              cout<<"Enter 'Y/y' to go back :"<<endl;
              cin>>ch;
              if(ch == 'Y' || 'y')
                return;
              else
                HomePage(DB);
              break;
              
      case 2: cout<<"Enter Book ID :"<<endl;
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              getline(cin,value);

              sql = "SELECT * FROM Book WHERE BookID = ?;";

              if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
                cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;

              
              sqlite3_bind_text(stmt,1,value.c_str(),-1,SQLITE_STATIC);
              
              while(true)
              {
                  rc = sqlite3_step(stmt);

                  if (rc == SQLITE_ROW) 
                  {
                      BookID  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                      BookName    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                      BookAuthor     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                      BookGenre  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                      BookPrice = sqlite3_column_int(stmt, 4);
                      ShowBookData();
                      count++;
                  }
                
                  else if (rc == SQLITE_DONE) 
                    {
                      cout<<endl<<count<<" record/s were found with same name."<<endl;
                      break;
                    }
                  else 
                    {
                        cerr << "Error fetching row: " << sqlite3_errmsg(DB.getDB()) << endl;
                        break;
                    }
              }
              sqlite3_finalize(stmt);
          
              cout<<"Enter 'Y/y' to go back to AdminMenu"<<endl;
              cin>>ch;
              if(ch == 'Y' || 'y')
                return;
              else
                HomePage(DB);        
              break;

      case 3: cout<<"Enter Book's Author Name :"<<endl;
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              getline(cin,value);

              sql = "SELECT * FROM Book WHERE Author = ?;";

              if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
                cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;
              
              sqlite3_bind_text(stmt,1,value.c_str(),-1,SQLITE_STATIC);
              
              while(true)
              {
                  rc = sqlite3_step(stmt);

                  if (rc == SQLITE_ROW) 
                  {
                      BookID  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                      BookName    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                      BookAuthor     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                      BookGenre  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                      BookPrice = sqlite3_column_int(stmt, 4);
                      ShowBookData();
                      count++;
                  }
                
                  else if (rc == SQLITE_DONE) 
                    {
                      cout<<endl<<count<<" record/s were found with same name."<<endl;
                      break;
                    }
                  else 
                    {
                        cerr << "Error fetching row: " << sqlite3_errmsg(DB.getDB()) << endl;
                        break;
                    }
              }
              sqlite3_finalize(stmt);
          
              cout<<"Enter 'Y/y' to go back to AdminMenu"<<endl;
              cin>>ch;
              if(ch == 'Y' || 'y')
                return;
              else
                HomePage(DB);
              break;

      default :cout<<"Invalid Choice"<<endl;
               this_thread::sleep_for(chrono::milliseconds(2000));
               clearScreen();
               return;
   }   
}





bool Book::StoreBookData(Database& DB)
{
  sqlite3_stmt* stmt;
  const char* sql = "INSER INTO Book (BookID,Name,Author,Genre,Price) VALUES (?,?,?,?,?);";

  if(sqlite3_prepare_v2(DB.getDB(),sql,-1,&stmt,nullptr) != SQLITE_OK)
    {
      cerr<<"Failed to prepare statement : "<<sqlite3_errmsg(DB.getDB())<<endl;
      return false;
    }
  sqlite3_bind_text(stmt,1,BookID.c_str(),-1,SQLITE_STATIC);
  sqlite3_bind_text(stmt,2,BookName.c_str(),-1,SQLITE_STATIC);
  sqlite3_bind_text(stmt,3,BookAuthor.c_str(),-1,SQLITE_STATIC);
  sqlite3_bind_text(stmt,4,BookGenre.c_str(),-1,SQLITE_STATIC);
  sqlite3_bind_int(stmt,5,BookPrice);

  if(sqlite3_step(stmt) != SQLITE_DONE)
    {
      cerr<<"Failed to store book data : "<<sqlite3_errmsg(DB.getDB())<<endl;
      return false;
    }
  else
      return true;
    

}