#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <ctime>    // for struct tm
#include "Book.h"   // forward declare or include Book

class Student {
private:
    std::string name;
    std::string DOB;
    std::string gender;
    std::string mobile;
    std::string EmailID;
    std::string Student_Pswd;
    Book BookRecord;          // association with Book class
    bool BorrowedStatus;
    std::string PROFILE_TIME;
    struct tm* BorrowedTime;
    std::string BORROWED_TIME;

public:
    // constructor
    Student();

    // member functions
    void GetData();
    void ViewProfile(Database&,std::string user);
    bool StoreStudentData(Database&);
    void ShowStudentData(Book book);
    void StudentMenu(Database&,std::string user);
    void BorrowBook(Database&,std::string bookID);
    void ReturnBook(Database&,std::string bookID);
    bool DeleteProfile(Database&,std::string user);
    void EditProfile(Database&,std::string user);
    void Login(Database&);
    void Signup(Database&);
    void DeleteStudentData(Database&);
    void SearchStudentData(Database&);
    void ListStudentData(Database&);
    void UpdateBookRecord(Database&,Book book);
    void ResetBookRecord(Database&);
};

#endif // STUDENT_H
