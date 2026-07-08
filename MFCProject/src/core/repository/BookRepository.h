#pragma once

#include "pch.h"
#include "IBookRepository.h"
#include "IDatabaseManager.h"


class CBookRepository : public IBookRepository
{
public:
    CBookRepository(IDatabaseManager* pDbManager);
    virtual ~CBookRepository();

    virtual bool Add(Book& book) override;
    virtual bool Update(const Book& book) override;
    virtual bool Delete(long id) override;
    virtual bool DeleteAll() override;

    virtual std::vector<Book> GetAll() override;
    virtual std::vector<Book> Search(const CString& keyword) override;
    virtual std::vector<Book> GetSorted(const CString& column, bool ascending) override;

private:
    Book ReadRow(CRecordset& rs);
    std::vector<Book> ExecuteQuery(const CString& sql);
    static CString EscapeSql(const CString& input);

	IDatabaseManager* m_pDbManager;
};