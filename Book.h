#ifndef BOOK_H
#define BOOK_H
#include "Database.h"
#include <string>
#include <ctime>   


class Book
{
private:
    std::string BookName;
    std::string BookID;
    std::string BookAuthor;
    std::string BookGenre;
    std::string BookInclusionTime;
    int BookPrice;

public:
    void SetBookData();
    bool StoreBookData(Database&);
    void SearchBookData(Database&);
    void ShowBookData();
    void DeleteBookData(Database&);
    void ListBookData(Database&);
    void GenerateToken(int,int,int);

};

#endif