#include "pch.h"
#include "BookRepository.h"
#include "DatabaseManager.h"

CBookRepository::CBookRepository(IDatabaseManager* pDbManager)
    : m_pDbManager(pDbManager)
{
}

CBookRepository::~CBookRepository()
{
}

CString CBookRepository::EscapeSql(const CString& input)
{
    CString result = input;
    result.Replace(_T("'"), _T("''"));
    return result;
}

Book CBookRepository::ReadRow(CRecordset& rs)
{
    Book book;
    CString strVal;

    rs.GetFieldValue(_T("ID"), strVal);
    book.ID = _ttol(strVal);

    rs.GetFieldValue(_T("NAME"), strVal);
    book.Name = strVal;

    rs.GetFieldValue(_T("PRICE"), strVal);
    book.Price = _ttof(strVal);

    rs.GetFieldValue(_T("QTY"), strVal);
    book.Qty = _ttoi(strVal);

    rs.GetFieldValue(_T("CREATED_DATE"), strVal);
    book.CreatedDate.ParseDateTime(strVal);

    return book;
}

std::vector<Book> CBookRepository::ExecuteQuery(const CString& sql)
{
    std::vector<Book> result;
	auto& db = m_pDbManager->GetDatabase();

    try {
        CRecordset rs(&db);
        rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);

        while (!rs.IsEOF())
        {
            result.push_back(ReadRow(rs));
            rs.MoveNext();
        }
        rs.Close();
    } catch(CDBException* e) {
        CString msg;
        msg.Format(_T("Query failed: %s"), e->m_strError);
        AfxMessageBox(msg);
        
        e->Delete();
    }

    return result;
}

bool CBookRepository::Add(Book& book)
{
    bool success = false;
	auto& db = m_pDbManager->GetDatabase();

    try {
        CString sql;
        sql.Format(_T("INSERT INTO BOOK (NAME, PRICE, QTY) VALUES ('%s', %f, %d)"),
            EscapeSql(book.Name), book.Price, book.Qty);
        db.ExecuteSQL(sql);

        CRecordset rs(&db);
        rs.Open(CRecordset::forwardOnly, _T("SELECT LAST_INSERT_ID() AS NEWID"), CRecordset::readOnly);
        if (!rs.IsEOF())
        {
            CString strNewId;
            rs.GetFieldValue(_T("NEWID"), strNewId);
            book.ID = _ttol(strNewId);
        }
        rs.Close();

        success = true;
    } catch (CDBException* e) {
        CString msg;
        msg.Format(_T("Add book failed: %s"), e->m_strError);
        AfxMessageBox(msg);
        success = false;

        e->Delete();
    }

    return success;
}

bool CBookRepository::Update(const Book& book)
{
    bool success = false;
	auto& db = m_pDbManager->GetDatabase();

    try {
        CString sql;
        sql.Format(_T("UPDATE BOOK SET NAME='%s', PRICE=%f, QTY=%d WHERE ID=%ld"),
            EscapeSql(book.Name), book.Price, book.Qty, book.ID);
        db.ExecuteSQL(sql);
        success = true;
    } catch (CDBException* e) {
        CString msg;
        msg.Format(_T("Update book failed: %s"), e->m_strError);
        AfxMessageBox(msg);
        success = false;

        e->Delete();
    }

    return success;
}

bool CBookRepository::DeleteAll()
{
    bool success = false;
	auto& db = m_pDbManager->GetDatabase();

    try {
        CString sql;
        sql.Format(_T("DELETE FROM BOOK"));
        db.ExecuteSQL(sql);
        success = true;
    } catch (CDBException* e) {
        CString msg;
        msg.Format(_T("Clear books failed: %s"), e->m_strError);
        AfxMessageBox(msg);
        success = false;

        e->Delete();
    }

    return success;
}

bool CBookRepository::Delete(long id)
{
    bool success = false;
	auto& db = m_pDbManager->GetDatabase();

    try {
        CString sql;
        sql.Format(_T("DELETE FROM BOOK WHERE ID=%ld"), id);
        db.ExecuteSQL(sql);
        success = true;
    } catch (CDBException* e) {
        CString msg;
        msg.Format(_T("Delete book failed: %s"), e->m_strError);
        AfxMessageBox(msg);
        success = false;

        e->Delete();
    }

    return success;
}

std::vector<Book> CBookRepository::GetAll()
{
    return ExecuteQuery(_T("SELECT ID, NAME, PRICE, QTY, CREATED_DATE FROM BOOK ORDER BY ID"));
}

std::vector<Book> CBookRepository::Search(const CString& keyword)
{
    CString sql;
    sql.Format(_T("SELECT ID, NAME, PRICE, QTY, CREATED_DATE FROM BOOK WHERE NAME LIKE '%%%s%%' ORDER BY ID"),
        EscapeSql(keyword));
    return ExecuteQuery(sql);
}

std::vector<Book> CBookRepository::GetSorted(const CString& column, bool ascending)
{
    CString sql;
    sql.Format(_T("SELECT ID, NAME, PRICE, QTY, CREATED_DATE FROM BOOK ORDER BY %s %s"),
        column, ascending ? _T("ASC") : _T("DESC"));
    return ExecuteQuery(sql);
}