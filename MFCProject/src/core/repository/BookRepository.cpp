#include "pch.h"
#include "BookRepository.h"
#include "DatabaseManager.h"

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
    // GetFieldValue() của CRecordset "trần" (không DoFieldExchange) chỉ có overload
    // nhận CString&, CDBVariant& hoặc CByteArray& - KHÔNG có overload cho long/double/
    // COleDateTime trực tiếp. Vì vậy lấy tất cả ra CString rồi tự convert cho chắc chắn,
    // không phụ thuộc kiểu dữ liệu nội bộ mà driver ODBC trả về.
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
        // Lưu ý: GetFieldValue() không có overload nhận long& - phải lấy ra
        // CString rồi tự convert (giống ReadRow()).
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

bool CBookRepository::DeleteAll()
{
    CDatabase& db = CDatabaseManager::Instance().GetDatabase();
    bool success = false;

    TRY
    {
        CString sql;
        sql.Format(_T("DELETE FROM BOOK"));
        db.ExecuteSQL(sql);
        success = true;
    }
        CATCH(CDBException, e)
    {
        CString msg;
        msg.Format(_T("Clear books failed: %s"), e->m_strError);
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
    sql.Format(_T("SELECT ID, NAME, PRICE, QTY, CREATED_DATE FROM BOOK WHERE NAME LIKE '%%%s%%' ORDER BY ID LIMIT 20;"),
        EscapeSql(keyword));
    return ExecuteQuery(sql);
}

std::vector<Book> CBookRepository::GetSorted(const CString& column, bool ascending)
{
    // column nên whitelist ở Service layer (NAME / PRICE) để tránh SQL injection qua tên cột
    CString sql;
    sql.Format(_T("SELECT ID, NAME, PRICE, QTY, CREATED_DATE FROM BOOK ORDER BY %s %s LIMIT 20;"),
        column, ascending ? _T("ASC") : _T("DESC"));
    return ExecuteQuery(sql);
}


std::vector<Book> CBookRepository::GetPaged(int offset, int limit)
{
    // offset/limit là int (không phải chuỗi người dùng nhập trực tiếp) nên build
    // bằng Format an toàn, không lo SQL injection như với NAME/keyword.
    CString sql;
    sql.Format(_T("SELECT ID, NAME, PRICE, QTY, CREATED_DATE FROM BOOK ORDER BY ID LIMIT %d OFFSET %d"),
        limit, offset);
    return ExecuteQuery(sql);
}

int CBookRepository::GetTotalCount()
{
    CDatabase& db = CDatabaseManager::Instance().GetDatabase();
    int total = 0;

    TRY
    {
        CRecordset rs(&db);
        rs.Open(CRecordset::forwardOnly, _T("SELECT COUNT(*) AS TOTAL FROM BOOK"), CRecordset::readOnly);

        if (!rs.IsEOF())
        {
            CString strTotal;
            rs.GetFieldValue(_T("TOTAL"), strTotal);   // GetFieldValue chỉ có overload CString - xem ghi chú trước
            total = _ttoi(strTotal);
        }
        rs.Close();
    }
        CATCH(CDBException, e)
    {
        CString msg;
        msg.Format(_T("Count query failed: %s"), e->m_strError);
        AfxMessageBox(msg);
    }
    END_CATCH

        return total;
}
int CBookRepository::GetTotalPage(int pageSize)
{
    CDatabase& db = CDatabaseManager::Instance().GetDatabase();
    int totalPages = 0;

    TRY
    {
        CRecordset rs(&db);
        CString strSQL;
        strSQL.Format(_T("SELECT CEIL(COUNT(*) / %d) AS TOTAL_PAGES FROM BOOK"), pageSize);
        rs.Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);

        if (!rs.IsEOF())
        {
            CString strTotalPages;
            rs.GetFieldValue(_T("TOTAL_PAGES"), strTotalPages);
            totalPages = _ttoi(strTotalPages);
        }
        rs.Close();
    }
        CATCH(CDBException, e)
    {
        CString msg;
        msg.Format(_T("GetTotalPage failed: %s"), e->m_strError);
        AfxMessageBox(msg);
    }
    END_CATCH

        return totalPages;
}