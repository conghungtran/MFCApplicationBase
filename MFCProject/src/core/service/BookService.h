#pragma once
#include <memory>
#include "IBookRepository.h"

// ==============================
// Service layer: chứa business rule / validation
// UI chỉ được gọi vào đây, không gọi thẳng Repository
// ==============================
class CBookService
{
public:
    explicit CBookService(std::shared_ptr<IBookRepository> repository);

    bool AddBook(Book& book, CString& errorMsg);
    bool UpdateBook(const Book& book, CString& errorMsg);
    bool DeleteBook(long id, CString& errorMsg);

    std::vector<Book> GetAllBooks();
    std::vector<Book> SearchBooks(const CString& keyword);

    // sortColumn: "NAME" hoặc "PRICE" (whitelist để tránh SQL injection)
    std::vector<Book> GetSortedBooks(const CString& sortColumn, bool ascending);

private:
    bool Validate(const Book& book, CString& errorMsg);

    std::shared_ptr<IBookRepository> m_repository;
};
