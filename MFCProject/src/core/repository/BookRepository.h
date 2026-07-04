#pragma once
#include "IBookRepository.h"
#include <afxdb.h>

// ==============================
// Cài đặt Repository dùng CDatabase / CRecordset (ODBC) - cho MySQL
// ==============================
class CBookRepository : public IBookRepository
{
public:
    CBookRepository();
    virtual ~CBookRepository();

    virtual bool Add(Book& book) override;
    virtual bool Update(const Book& book) override;
    virtual bool Delete(long id) override;

    virtual std::vector<Book> GetAll() override;
    virtual std::vector<Book> Search(const CString& keyword) override;
    virtual std::vector<Book> GetSorted(const CString& column, bool ascending) override;

private:
    Book ReadRow(CRecordset& rs);
    std::vector<Book> ExecuteQuery(const CString& sql);
    static CString EscapeSql(const CString& input); // chống lỗi khi Name chứa dấu nháy đơn
};