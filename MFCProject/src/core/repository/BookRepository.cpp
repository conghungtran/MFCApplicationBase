#include "pch.h"
#include "BookRepository.h"
#include "../Database/DatabaseManager.h"

CBookRepository::CBookRepository()
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
    long    lVal;
    double  dVal;
    COleDateTime dtVal;

    rs.GetFieldValue(_T("ID"), lVal);
    book.ID = lVal;

    rs.GetFieldValue(_T("NAME"), strVal);
    book.Name = strVal;

    rs.GetFieldValue(_T("PRICE"), dVal);
    book.Price = dVal;

    rs.GetFieldValue(_T("QTY"), lVal);
    book.Qty = (int)lVal;

    rs.GetFieldValue(_T("CREATED_DATE"), dtVal);
    book.CreatedDate = dtVal;

    return book;
}

std::vector<Book> CBookRepository::ExecuteQuery(const CString& sql)
{
    std::vector<Book> result;
    CDatabase& db = CDatabaseManager::Instance().GetDatabase();

    TRY
    {
        CRecordset rs(&db);
        rs.Open(CRecordset::forwardOnly, sql, CRecordset::readOnly);

        while (!rs.IsEOF())
        {
            result.push_back(ReadRow(rs));
            rs.MoveNext();
        }
        rs.Close();
    }
        CATCH(CDBException, e)
    {
        CString msg;
        msg.Format(_T("Query failed: %s"), e->m_strError);
        AfxMessageBox(msg);
    }
    END_CATCH

        return result;
}

bool CBookRepository::Add(Book& book)
{
    CDatabase& db = CDatabaseManager::Instance().GetDatabase();
    bool success = false;

    TRY
    {
        CString sql;
        sql.Format(_T("INSERT INTO BOOK (NAME, PRICE, QTY) VALUES ('%s', %f, %d)"),
            EscapeSql(book.Name), book.Price, book.Qty);
        db.ExecuteSQL(sql);

        // MySQL: lấy ID vừa insert bằng LAST_INSERT_ID()
        // (SQL Server dùng @@IDENTITY, SQLite dùng sqlite3_last_insert_rowid)
        CRecordset rs(&db);
        rs.Open(CRecordset::forwardOnly, _T("SELECT LAST_INSERT_ID() AS NEWID"), CRecordset::readOnly);
        if (!rs.IsEOF())
        {
            long newId = 0;
            rs.GetFieldValue(_T("NEWID"), newId);
            book.ID = newId;
        }
        rs.Close();

        success = true;
    }
        CATCH(CDBException, e)
    {
        CString msg;
        msg.Format(_T("Add book failed: %s"), e->m_strError);
        AfxMessageBox(msg);
        success = false;
    }
    END_CATCH

        return success;
}

bool CBookRepository::Update(const Book& book)
{
    CDatabase& db = CDatabaseManager::Instance().GetDatabase();
    bool success = false;

    TRY
    {
        CString sql;
        sql.Format(_T("UPDATE BOOK SET NAME='%s', PRICE=%f, QTY=%d WHERE ID=%ld"),
            EscapeSql(book.Name), book.Price, book.Qty, book.ID);
        db.ExecuteSQL(sql);
        success = true;
    }
        CATCH(CDBException, e)
    {
        CString msg;
        msg.Format(_T("Update book failed: %s"), e->m_strError);
        AfxMessageBox(msg);
        success = false;
    }
    END_CATCH

        return success;
}

bool CBookRepository::Delete(long id)
{
    CDatabase& db = CDatabaseManager::Instance().GetDatabase();
    bool success = false;

    TRY
    {
        CString sql;
        sql.Format(_T("DELETE FROM BOOK WHERE ID=%ld"), id);
        db.ExecuteSQL(sql);
        success = true;
    }
        CATCH(CDBException, e)
    {
        CString msg;
        msg.Format(_T("Delete book failed: %s"), e->m_strError);
        AfxMessageBox(msg);
        success = false;
    }
    END_CATCH

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
    // column nên whitelist ở Service layer (NAME / PRICE) để tránh SQL injection qua tên cột
    CString sql;
    sql.Format(_T("SELECT ID, NAME, PRICE, QTY, CREATED_DATE FROM BOOK ORDER BY %s %s"),
        column, ascending ? _T("ASC") : _T("DESC"));
    return ExecuteQuery(sql);
}