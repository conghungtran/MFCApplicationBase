#include "pch.h"
#include "BookService.h"

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
