#pragma once
#include <vector>
#include "Model/Book.h"

class IBookRepository
{
public:
    virtual ~IBookRepository() {}

    virtual bool Add(Book& book) = 0;
    virtual bool Update(const Book& book) = 0;
    virtual bool Delete(long id) = 0;
    virtual bool DeleteAll() = 0;

    virtual std::vector<Book> GetAll() = 0;
    virtual std::vector<Book> Search(const CString& keyword) = 0;
    virtual std::vector<Book> GetSorted(const CString& column, bool ascending) = 0;
};
