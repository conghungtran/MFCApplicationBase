#include "pch.h"
#include "BookService.h"
#include <iostream>

CBookService::CBookService(std::shared_ptr<IBookRepository> repository)
    : m_repository(repository)
{
}

bool CBookService::Validate(const Book& book, CString& errorMsg)
{
    if (book.Name.IsEmpty())
    {
        errorMsg = _T("Book Name không được để trống.");
        return false;
    }
    if (book.Price <= 0)
    {
        errorMsg = _T("Price phải lớn hơn 0.");
        return false;
    }
    if (book.Qty < 0)
    {
        errorMsg = _T("Quantity không được âm.");
        return false;
    }
    return true;
}

bool CBookService::AddBook(Book& book, CString& errorMsg)
{
    if (!Validate(book, errorMsg))
        return false;

    if (!m_repository->Add(book))
    {
        errorMsg = _T("Không thể thêm sách vào Database.");
        return false;
    }
    return true;
}

bool CBookService::UpdateBook(const Book& book, CString& errorMsg)
{
    if (book.ID <= 0)
    {
        errorMsg = _T("Chưa chọn sách cần cập nhật.");
        return false;
    }
    if (!Validate(book, errorMsg))
        return false;

    if (!m_repository->Update(book))
    {
        errorMsg = _T("Không thể cập nhật sách.");
        return false;
    }
    return true;
}

bool CBookService::DeleteBook(long id, CString& errorMsg)
{
    if (id <= 0)
    {
        errorMsg = _T("Chưa chọn sách cần xóa.");
        return false;
    }
    if (!m_repository->Delete(id))
    {
        errorMsg = _T("Không thể xóa sách.");
        return false;
    }
    return true;
}

bool CBookService::DeleteAllBook(CString& errorMsg)
{

    if (!m_repository->DeleteAll())
    {
        errorMsg = _T("Không thể xóa sách.");
        return false;
    }
    return true;
}


std::vector<Book> CBookService::GetAllBooks()
{
    return m_repository->GetAll();
}

std::vector<Book> CBookService::SearchBooks(const CString& keyword)
{
    return m_repository->Search(keyword);
}

std::vector<Book> CBookService::GetSortedBooks(const CString& sortColumn, bool ascending)
{
    // Whitelist cột hợp lệ để tránh SQL injection qua tên cột
    CString column = sortColumn;
    column.MakeUpper();
    if (column != _T("NAME") && column != _T("PRICE"))
        column = _T("ID");

    return m_repository->GetSorted(column, ascending);
}

std::vector<Book> CBookService::GetBooksPage(int pageNumber, int pageSize, int& totalPages)
{
    if (pageNumber < 1)
        pageNumber = 1;
    if (pageSize < 1)
        pageSize = 20;   // mặc định 20 dòng/trang, khớp comment "page size = 20" bạn từng có

    int totalRecords = m_repository->GetTotalCount();
    totalPages = (totalRecords + pageSize - 1) / pageSize;   // làm tròn lên

    std::cout << "After:  " << totalPages << "\n";

    if (totalPages < 1)
        totalPages = 1;

    int offset = (pageNumber - 1) * pageSize;
    return m_repository->GetPaged(offset, pageSize);
}

int CBookService::GetCount() {
    return m_repository->GetTotalCount();
}

int CBookService::GetTotalPage(int pageSize) {
    return m_repository->GetTotalPage(pageSize);
}
