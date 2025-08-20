#include "Student.h"
#include "Database.h"
#include "Library.h"
#include "Admin.h"
#include "Book.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>   // for formatting output
#include <ctime>     // for time functions
#include <sqlite3.h> // if DB needed
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <strings.h>
#include <cstring>
#include <cmath>

using namespace std;

//Constructor
Student:: Student()
  {
    BorrowedStatus=false;
    name="";
    DOB="";
    gender="";
    EmailID="";
    Student_Pswd="";
  }

//Function Definitions
void Student::Signup(Database& DB)
{

  GetData();

  if(StoreStudentData(DB))
    {
        cout<<endl<<"Student data has been updated to the server..."<<endl;
        this_thread::sleep_for(chrono::milliseconds(2000));
        clearScreen();
    }
    else
    {
        cout<<"Updation failed..."<<endl;
        this_thread::sleep_for(chrono::milliseconds(2000));
        clearScreen();
    }
}



void Student::Login(Database& DB)
{
  string USER,PASSWORD;
  int counter=0,totalAttempts =3,flag =0;
  sqlite3_stmt* stmt;
  const char* sql = "SELECT * FROM Student WHERE (Email = ? OR Mobile = ?) AND Password = ?;";

  while(true)
  {
    clearScreen();
    Title("STUDENT  LOGIN");
    cout<<"Enter Student Mobile No./Email ID"<<endl;
    cin>>USER;
    cout<<"Enter Password"<<endl;
    cin.ignore();
    PASSWORD.clear();
    PASSWORD = getHiddenInput();

    
    if(sqlite3_prepare_v2(DB.getDB(),sql,-1,&stmt,nullptr) == SQLITE_OK)
      {
         sqlite3_bind_text(stmt,1,USER.c_str(),-1,SQLITE_STATIC);
         sqlite3_bind_text(stmt,2,USER.c_str(),-1,SQLITE_STATIC);
         sqlite3_bind_text(stmt,3,PASSWORD.c_str(),-1,SQLITE_STATIC);

        if(sqlite3_step(stmt) == SQLITE_ROW)
            {
              flag++;
              break; 
            }
        else
            {
              cout<<"Invalid credentials..."<<endl;
              counter++;
              if(counter < totalAttempts)
                 {
                  cout<<"Only "<<totalAttempts - counter<<" attempt left"<<endl;
                  this_thread::sleep_for(chrono::milliseconds(4000));
                  continue;
                 }
              else
                 {
                  cout<<"Too many attempts failed\n";
                  this_thread::sleep_for(chrono::milliseconds(4000));
                  break;
                 }
               
            }
      }

      else
      {
        cerr<<"Statement Preparation failed..."<<sqlite3_errmsg(DB.getDB())<<endl;
        this_thread::sleep_for(chrono::milliseconds(4000));
        clearScreen();
        break;
      }
  }

  if(flag==1)
      {
        cout<<endl<<"Login Successfull..."<<endl;
        this_thread::sleep_for(chrono::milliseconds(3000));
        StudentMenu(DB,USER);
      }
  HomePage(DB);
}



void Student::GetData()
{
    clearScreen();
    Title("SIGNUP");
    int count=0;
    cout<<"Enter your full name : ";
    cin.ignore();
    getline(cin,name);


    while(true)
    {
      cout<<"\nEnter your Date of Birth in dd/mm/yyyy format : ";
      cin>>DOB;

      if(!DOBVerifier(DOB))
        {
          cout<<"Invalid Date of Birth\n";
          this_thread::sleep_for(chrono::milliseconds(2000));
          clearScreen();
          continue;
        }
      else
          break;
    }

  
    while(true)
    {
      cout<<"\nMale or Female : ";
      cin.ignore();
      cin>>gender;

      if(!GenderVerifier(gender))
        {
          cout<<"Entered value is incorrect\n";
          cout<<"Please Enter in correct format \n";
          this_thread::sleep_for(chrono::milliseconds(2000));
          clearScreen();
          continue;
        }
      else
          break;
    }

    while(true)
    {
      cout<<"\nEnter your mobile no. without ISD code : ";
      cin.ignore();
      cin>>mobile;

      if(!MobileVerifier(mobile))
        {
          cout<<"Enter a valid mobile number\n";
          this_thread::sleep_for(chrono::milliseconds(2000));
          clearScreen();
          continue;
        }
      else
          break;
    }
    
    while(true)
    {
      cout<<"\nEnter your email Id : ";
      cin.ignore();
      cin>>EmailID;

      if(!EmailVerifier(EmailID))
        {
            cout<<"Please enter a valid email ID\n";
            this_thread::sleep_for(chrono::milliseconds(2000));
            clearScreen();
            continue;
        }
      else
          break;
    }
    
    
    while(true)
    {
      cout<<"\nEnter your password : ";
      string PASSWORD;
      PASSWORD = getHiddenInput();
      if(!PasswordVerifier(PASSWORD))
        {
            cout<<"\nYour Password should have \n1.Atleast 8 character long\n2.Should have alphabet in both upper and lower case\n3.Should contain numbers\n4.Should have special characters\n";
            cout<<endl;
            continue;
        }
      else
        Student_Pswd = PASSWORD;
        break;
    }

    time_t t;
    time(&t);
    PROFILE_TIME = string(ctime(&t));
    PROFILE_TIME.pop_back(); // remove '\n'

    
}



bool Student::StoreStudentData(Database& DB)
{

  sqlite3_stmt* stmt;
  const char* sql = "INSERT INTO Student (Name,DOB,Gender,Mobile,Email,Password) VALUES (?,?,?,?,?,?);";

  if (sqlite3_prepare_v2(DB.getDB(),sql,-1,&stmt,nullptr) != SQLITE_OK)
    {
      cerr<<"Failed to prepare statement"<<sqlite3_errmsg(DB.getDB())<<endl;
      return false;
    }
  sqlite3_bind_text(stmt,1,this->name.c_str(),-1,SQLITE_STATIC);
  sqlite3_bind_text(stmt,2,this->DOB.c_str(),-1,SQLITE_STATIC);
  sqlite3_bind_text(stmt,3,this->gender.c_str(),-1,SQLITE_STATIC);
  sqlite3_bind_text(stmt,4,this->mobile.c_str(),-1,SQLITE_STATIC);
  sqlite3_bind_text(stmt,5,this->EmailID.c_str(),-1,SQLITE_STATIC);
  sqlite3_bind_text(stmt,6,this->Student_Pswd.c_str(),-1,SQLITE_STATIC);
  
  if (sqlite3_step(stmt) == SQLITE_DONE)
    {
      sqlite3_finalize(stmt);
      return true;
    }
  else
    {
      cerr<<"Failed to store in database"<<sqlite3_errmsg(DB.getDB())<<endl;
      return false;
    }
}




void Student::StudentMenu(Database& DB, string USER = " ")
{ sqlite3_stmt* stmt;
  string name ="";
  const char* sql = "SELECT Name FROM Student WHERE Email = ? OR Mobile = ?";
  if(sqlite3_prepare_v2(DB.getDB(),sql,-1,&stmt,nullptr) != SQLITE_OK) cerr<<"Failed to prepare statement..."<<sqlite3_errmsg(DB.getDB())<<endl;
  sqlite3_bind_text(stmt,1,USER.c_str(),-1,SQLITE_STATIC);
  sqlite3_bind_text(stmt,2,USER.c_str(),-1,SQLITE_STATIC);
  int rc = sqlite3_step(stmt);
  
  if (rc == SQLITE_ROW)
  {
    const unsigned char* text = sqlite3_column_text(stmt,0);
    if (text) 
      name = reinterpret_cast<const char*>(text);
  }
      else if (rc == SQLITE_DONE) cerr<<"No rows found..."<<sqlite3_errmsg(DB.getDB())<<endl;
  sqlite3_finalize(stmt);
  int choice;
  Book B;
  while(1)
  {
          clearScreen();
          string title = "HELLO "+ name + ", WELCOME  TO  STUDENT  PORTAL";
          Title(title);
          cout<<"1) View Profile\n2) List Of Books\n3) Borrow Book\n4) Return Book\n5) Search Book\n6) Main Menu\n";
          cout<<endl<<"Enter your choice\n";
          fflush(stdin);
          cin>>choice;
          
          switch(choice)
          {
            case 1: clearScreen();
                    ViewProfile(DB,USER);
                    break;
        
            case 2: clearScreen();
                    B.ListBookData(DB);
                    break;
        
            case 3: clearScreen();
                    if(BorrowedStatus==false)
                    BorrowBook(DB,USER);
                    else
                    {
                      cout<<"Unable to Borrow Book since your previous Book is yet to be returned"<<endl;
                      this_thread::sleep_for(chrono::milliseconds(2000));
                      clearScreen(); 
                    }
                    break;
        
            case 4: clearScreen();
                    if(BorrowedStatus==true)
                    ReturnBook(DB,USER);
                    else
                      {
                        cout<<"Unable to Return Book since you have not borrowed any book"<<endl;
                        this_thread::sleep_for(chrono::milliseconds(2000));
                        clearScreen();
                      }
                    break;
        
            case 5: clearScreen();
                    B.SearchBookData(DB);
                    break;
        
            case 6: clearScreen();
                    HomePage(DB);
                    break;
        
            default:clearScreen();
                    cout<<"Invalid Choice\n";
                    this_thread::sleep_for(chrono::milliseconds(2000));
                    clearScreen();
            
          }
       cout<<"Press 0 to go back to Student Menu"<<endl;
       fflush(stdin);
       if(!'0'== cin.get())
        HomePage(DB);
                    
  }
}



void Student::ViewProfile(Database& DB,string USER = " ")
{
  sqlite3_stmt* stmt;
  string name ="";
  const char* sql = "SELECT Name FROM Student WHERE Email = ? OR Mobile = ?";
  if(sqlite3_prepare_v2(DB.getDB(),sql,-1,&stmt,nullptr) != SQLITE_OK) cerr<<"Failed to prepare statement..."<<sqlite3_errmsg(DB.getDB())<<endl;
  sqlite3_bind_text(stmt,1,USER.c_str(),-1,SQLITE_STATIC);
  sqlite3_bind_text(stmt,2,USER.c_str(),-1,SQLITE_STATIC);
  int rc = sqlite3_step(stmt);
  
  if (rc == SQLITE_ROW)
  {
    const unsigned char* text = sqlite3_column_text(stmt,0);
    if (text) 
      name = reinterpret_cast<const char*>(text);
  }
      else if (rc == SQLITE_DONE) cerr<<"No rows found..."<<sqlite3_errmsg(DB.getDB())<<endl;
  sqlite3_finalize(stmt);

  int choice;
  clearScreen();
  Title(name);
  
  ShowStudentData(BookRecord); 

   cout<<"1) Edit Profile\t\t\t2) Delete Profile\t\t\t3) Student Menu\t\t\t4) Home Page"<<endl<<endl;
   cout<<"Enter your choice"<<endl;
   cin>>choice;
     switch(choice)
     {
       case 1: EditProfile(DB,USER);
               break;

       case 2: DeleteProfile(DB,USER);
               break;

       case 3: StudentMenu(DB,USER);
               break;

       case 4: HomePage(DB);
               break;

       default: cout<<"Invalid choice..."<<endl;
                this_thread::sleep_for(chrono::milliseconds(2000));
                clearScreen();
                StudentMenu(DB,USER); 
     }
}



void Student::ShowStudentData(Book B)
{
   cout<<"Name                : "<<name<<endl;
   cout<<"Gender              : "<<gender<<endl;
   cout<<"Date Of Birth       : "<<DOB<<endl;
   cout<<"Mobile No.          : "<<mobile<<endl;
   cout<<"Email ID            : "<<EmailID<<endl;
   //cout<<"Profile created on  : "<<PROFILE_TIME<<endl;
   cout<<"Borrowed status     : "<<BorrowedStatus<<endl<<endl<<endl;
   if(BorrowedStatus==true)
      {
          cout<<"Borrowed Book -> "<<endl; 
          B.ShowBookData();
          cout<<"Borrowed Time       : "<<BORROWED_TIME<<endl;
          Line();
      }
}




void Student::DeleteStudentData(Database& DB)
{
  int choice,rc;
  string value;
  Admin A;
  Book B;
  sqlite3_stmt* stmt;
  sqlite3_stmt* delStmt;
  char confirm,ch;
  const char* sql = nullptr;
  
  clearScreen();
  Title("DELETE  STUDENT");

  cout<<"Delete student By :"<<endl<<endl;
  cout<<"1) Mobile No.\t\t\t2) Email ID"<<endl;
  cout<<"Enter your choice"<<endl;
  cin>>choice;

  switch(choice)
    {
      case 1: cout<<"Enter Student Mobile No. withoud ISD code"<<endl;
              cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
              getline(cin,value);
  
              sql = "SELECT * FROM Student WHERE Mobile = ?;";

              if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
                cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;
              
              sqlite3_bind_text(stmt,1,value.c_str(),-1,SQLITE_STATIC);
              
              rc = sqlite3_step(stmt);
              if (rc == SQLITE_ROW) 
                  {
                      name    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                      gender  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                      DOB     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                      mobile  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                      EmailID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                  
                      BorrowedStatus = sqlite3_column_int(stmt, 5);
                      if (BorrowedStatus)
                          BORROWED_TIME = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
                  
                      ShowStudentData(B);

                      cout<<endl<<"Do you really want to delete this record."<<endl<<"Enter 'Y'/'y' to confirm."<<endl;
                      cin>>confirm;
                        if (confirm == 'y' || 'Y')
                            {
                              const char* delSql = "DELETE FROM Student WHERE Mobile = ?;";
                            
                              if(sqlite3_prepare_v2(DB.getDB(),delSql,-1,&delStmt,nullptr) != SQLITE_OK) 
                                  cerr<<"Failed to prepare delete statement : "<<sqlite3_errmsg(DB.getDB())<<endl;
                              
                              sqlite3_bind_text(delStmt,1,value.c_str(),-1,SQLITE_STATIC);
                              
                              rc = sqlite3_step(delStmt);
                              if(rc != SQLITE_DONE) cerr<<"Failed to delete student record : "<<sqlite3_errmsg(DB.getDB())<<endl;
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
                A.AdminMenu(DB);
              else
                HomePage(DB);

              break;
              
      case 2: cout<<"Enter Student Email ID"<<endl;
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              getline(cin,value);

              sql = "SELECT * FROM Student WHERE Email = ?;";

              if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
                cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;
              
              sqlite3_bind_text(stmt,1,value.c_str(),-1,SQLITE_STATIC);
              
              rc = sqlite3_step(stmt);
              if (rc == SQLITE_ROW) 
              {
                  name    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                  gender  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                  DOB     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                  mobile  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                  EmailID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
              
                  BorrowedStatus = sqlite3_column_int(stmt, 5);
                  if (BorrowedStatus)
                      BORROWED_TIME = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
              
                  ShowStudentData(B);
                  cout<<endl<<"Do you really want to delete this record."<<endl<<"Enter 'Y'/'y' to confirm."<<endl;
                  cin>>confirm;
                  if (confirm == 'y' || 'Y')
                      {
                        const char* delSql = "DELETE FROM Student WHERE Email = ?;";
                      
                        if(sqlite3_prepare_v2(DB.getDB(),delSql,-1,&delStmt,nullptr) != SQLITE_OK) 
                            cerr<<"Failed to prepare delete statement : "<<sqlite3_errmsg(DB.getDB())<<endl;
                        
                        sqlite3_bind_text(delStmt,1,value.c_str(),-1,SQLITE_STATIC);
                        
                        rc = sqlite3_step(delStmt);

                        if(rc != SQLITE_DONE) cerr<<"Failed to delete student record : "<<sqlite3_errmsg(DB.getDB())<<endl;
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
                A.AdminMenu(DB);
              else
                HomePage(DB);       
                
              break;

      default :cout<<"Invalid Choice"<<endl;
               this_thread::sleep_for(chrono::milliseconds(2000));
               clearScreen();
   }
 }




 void Student::ListStudentData(Database& DB)
{
  Title("STUDENT   RECORD");
  Book B;
  Admin A;
  sqlite3_stmt* stmt;
  int count =0;
  const char* sql = "SELECT * FROM Student;";

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
          name    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
          gender  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
          DOB     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
          mobile  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
          EmailID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

          BorrowedStatus = sqlite3_column_int(stmt, 5);
          if (BorrowedStatus)
              BORROWED_TIME = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

          ShowStudentData(B);
          count++;
      }

      else if (rc == SQLITE_DONE) 
        {
          cout<<endl<<"Total Student's data fetched : "<<count<<endl;
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
  cout<<"Enter 'Y/y' to go back to AdminMenu"<<endl;
  cin>>ch;
  if(ch == 'Y' || 'y')
    A.AdminMenu(DB);
  else
    HomePage(DB);
}




void Student::SearchStudentData(Database& DB)
{
  int choice,count=0,rc;
  string value;
  Admin A;
  Book B;
  sqlite3_stmt* stmt;
  const char* sql = nullptr;
  char ch;
  
  clearScreen();
  Title("SEARCH  STUDENT");

  cout<<"Search student By :"<<endl<<endl;
  cout<<"1)Name\t\t\t2) Mobile No.\t\t\t3) Date Of Birth\t\t\t4) Email ID"<<endl;
  cout<<"Enter your choice"<<endl;
  cin>>choice;

  switch(choice)
    {
      case 1: cout<<"Enter Student Name"<<endl;
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              getline(cin,value);
  
              sql = "SELECT * FROM Student WHERE Name = ?;";

              if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
                cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;
              
              sqlite3_bind_text(stmt,1,value.c_str(),-1,SQLITE_STATIC);
              
              while(true)
              {
                  rc = sqlite3_step(stmt);

                  if (rc == SQLITE_ROW) 
                  {
                      name    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                      gender  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                      DOB     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                      mobile  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                      EmailID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                  
                      BorrowedStatus = sqlite3_column_int(stmt, 5);
                      if (BorrowedStatus)
                          BORROWED_TIME = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
                  
                      ShowStudentData(B);
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
                A.AdminMenu(DB);
              else
                HomePage(DB);
              break;
              
      case 2: cout<<"Enter Student Mobile No. withoud ISD code"<<endl;
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              getline(cin,value);

              sql = "SELECT * FROM Student WHERE Mobile = ?;";

              if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
                cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;

              
              sqlite3_bind_text(stmt,1,value.c_str(),-1,SQLITE_STATIC);
              
              while(true)
              {
                  rc = sqlite3_step(stmt);

                  if (rc == SQLITE_ROW) 
                  {
                      name    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                      gender  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                      DOB     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                      mobile  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                      EmailID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                  
                      BorrowedStatus = sqlite3_column_int(stmt, 5);
                      if (BorrowedStatus)
                          BORROWED_TIME = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
                  
                      ShowStudentData(B);
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
                A.AdminMenu(DB);
              else
                HomePage(DB);        
              break;

      case 3: cout<<"Enter Student's Date Of Birth in dd/mm/yyyy format"<<endl;
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              getline(cin,value);

              sql = "SELECT * FROM Student WHERE DOB = ?;";

              if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
                cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;
              
              sqlite3_bind_text(stmt,1,value.c_str(),-1,SQLITE_STATIC);
              
              while(true)
              {
                  rc = sqlite3_step(stmt);

                  if (rc == SQLITE_ROW) 
                  {
                      name    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                      gender  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                      DOB     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                      mobile  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                      EmailID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                  
                      BorrowedStatus = sqlite3_column_int(stmt, 5);
                      if (BorrowedStatus)
                          BORROWED_TIME = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
                  
                      ShowStudentData(B);
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
                A.AdminMenu(DB);
              else
                HomePage(DB);
              break;

      case 4: cout<<"Enter Student's Email ID"<<endl;
              cin.ignore(numeric_limits<streamsize>::max(), '\n');
              getline(cin,value);

              sql = "SELECT * FROM Student WHERE Email = ?;";

              if(sqlite3_prepare_v2(DB.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
                cerr << "Failed to prepare statement: " << sqlite3_errmsg(DB.getDB()) << endl;
              
              sqlite3_bind_text(stmt,1,value.c_str(),-1,SQLITE_STATIC);
              
              while(true)
              {
                  rc = sqlite3_step(stmt);

                  if (rc == SQLITE_ROW) 
                  {
                      name    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                      gender  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                      DOB     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                      mobile  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                      EmailID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                  
                      BorrowedStatus = sqlite3_column_int(stmt, 5);
                      if (BorrowedStatus)
                          BORROWED_TIME = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
                  
                      ShowStudentData(B);
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
                A.AdminMenu(DB);
              else
                HomePage(DB);
              break;

      default :cout<<"Invalid Choice"<<endl;
               this_thread::sleep_for(chrono::milliseconds(2000));
               clearScreen();
               A.AdminMenu(DB);
   }
 }




 void Student::BorrowBook(Database& DB, string USER)
{
  Book B;
  char ch;
  clearScreen();
  Title("BORROW   BOOK");
   cout<<"Search the Book you want to borrow"<<endl;
   B.SearchBookData(DB);
   cout<<"Do you want to borrow this Book"<<endl;
   cout<<"Enter Y/N"<<endl;
   cin>>ch;
   fflush(stdin);
   if('Y' ==ch || 'y'== ch)
    {
      this->UpdateBookRecord(DB,B);
      B.GenerateToken(1,0,0);
    }
   else
    {
      cout<<"Try Again..."<<endl;
      this_thread::sleep_for(chrono::milliseconds(2000));
      clearScreen();
      StudentMenu(DB,USER);
    }
}


void Student::ReturnBook(Database& DB, string USER)
{
  time_t now;
  time(&now);
  double Seconds;
  int Penalty=0;
  Seconds=difftime(now,mktime(BorrowedTime));
  float day=0;
  if(Seconds>2592000)
    {
      Seconds-=2592000;
      day = Seconds/86400;
    }
    day=ceil(day);
    Penalty = 2*day;
    
  clearScreen();
  Title("RETURN   BOOK");
  cout<<"Book You had borrowed :"<<endl;
  this->BookRecord.ShowBookData();
  cout<<"DO you want to Return this Book"<<endl;
  cout<<"Enter Y/N"<<endl;
  fflush(stdin);
   if('Y'== cin.get())
     {
         clearScreen();
         BookRecord.GenerateToken(2,day,Penalty);
         ResetBookRecord(DB);
     }
    else
       StudentMenu(DB,USER);
}




void Student::ResetBookRecord(Database& DB)
{
   Book B;
   BookRecord=B;
   BorrowedStatus=false;
   BorrowedTime=NULL;
   BORROWED_TIME="00:00:00";
}




void Student::UpdateBookRecord(Database& DB, Book B)
{
  ifstream fin;
  ofstream fout;
  Student S;

  BookRecord=B;
  BorrowedTime=Time();
  //cout<<endl<<asctime(BorrowedTime)<<endl;
  fin.open("Student_lib.txt",ios::in|ios::binary);
  fout.open("Tempfile.dat",ios::out|ios::app|ios::binary);
  
  if(fin)
    {
      fin.read((char*)&S,sizeof(S));
      while(!fin.eof())
        {
          if(S.name != name)
              fout.write((char*)&S,sizeof(S));
          else
              fout.write((char*)this,sizeof(*this));

          fin.read((char*)&S,sizeof(S));
        }
        fout.close();
        fin.close();
        remove("Student_lib.txt");
        rename("Tempfile.dat","Student_lib.txt");
    }
    else
    {
      cout<<"Data updation failed..."<<endl;
      this_thread::sleep_for(chrono::milliseconds(2000));
      clearScreen();
    }

  }



  void Student::DeleteProfile(Database& DB, string USER)
{
  ifstream fin;
  ofstream fout;
  Student S;
  
   fin.open("Student_lib.txt",ios::in);
      if(fin)
        {
          fout.open("Tempfile.txt",ios::out|ios::app);
          fin.read((char*)&S,sizeof(S));
          while(!fin.eof())
            {
                if(S.name != name)
                    fout.write((char*)&S,sizeof(S));

                fin.read((char*)&S,sizeof(S));
            }
          fout.close();       
          fin.close();
          remove("Student_lib.txt");
          rename("Tempfile.dat","Student_lib.dat");
          cout<<"Profile Deleted...";
          this_thread::sleep_for(chrono::milliseconds(2000));
          clearScreen();
          }

        else
          {
            cout<<"Student_lib.txt file error"<<endl;
            this_thread::sleep_for(chrono::milliseconds(2000));
            clearScreen();
          }
  HomePage(DB);
}





void Student::EditProfile(Database& DB,string USER)
  {
    ifstream fin;
    ofstream fout;
    Student S;

     fin.open("Student_lib.txt",ios::in);
        if(fin)
          {
            fout.open("Tempfile.txt",ios::out|ios::app);
            fin.read((char*)&S,sizeof(S));
            while(!fin.eof())
              {
                  if(S.EmailID != EmailID)
                      fout.write((char*)&S,sizeof(S));

                  else
                  {
                    S.GetData();
                    fout.write((char*)&S,sizeof(S));
                  }
                  fin.read((char*)&S,sizeof(S));
              }
            fout.close();       
            fin.close();
            remove("Student_lib.txt");
            rename("Tempfile.txt","Student_lib.txt");
            cout<<"Profile Updated...";
            this_thread::sleep_for(chrono::milliseconds(2000));
            clearScreen();
          }

          else
            {
              cout<<"Student_lib.txt file error"<<endl;
              this_thread::sleep_for(chrono::milliseconds(2000));
              clearScreen();
            }
      StudentMenu(DB, USER);

  }




