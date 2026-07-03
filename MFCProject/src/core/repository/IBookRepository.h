#pragma once
#include <vector>
#include "Model/Book.h"

// ==============================
// Repository interface
// (giúp Service không phụ thuộc trực tiếp
//  vào CDatabase/CRecordset -> dễ mock khi unit test)
// ==============================
class IBookRepository
{
public:
    virtual ~IBookRepository() {}

    virtual bool Add(Book& book) = 0;                 // ID được set ngược lại sau khi insert
    virtual bool Update(const Book& book) = 0;
    virtual bool Delete(long id) = 0;

    virtual std::vector<Book> GetAll() = 0;
    virtual std::vector<Book> Search(const CString& keyword) = 0;
    virtual std::vector<Book> GetSorted(const CString& column, bool ascending) = 0;
};
