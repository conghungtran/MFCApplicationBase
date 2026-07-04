#include "pch.h"
#include "DatabaseManager.h"

CDatabaseManager& CDatabaseManager::Instance()
{
    static CDatabaseManager instance;
    return instance;
}

CDatabaseManager::CDatabaseManager()
    : m_bConnected(false)
{
}

CDatabaseManager::~CDatabaseManager()
{
    Disconnect();
}

bool CDatabaseManager::Connect(const CString& dsn, const CString& user, const CString& password, const CString& database)
{
    if (m_bConnected)
        return true;

    TRY
    {
        CString connectStr;
        if (database.IsEmpty())
        {
            // Không truyền database -> dùng đúng database đã cấu hình sẵn trong DSN
            connectStr.Format(_T("ODBC;DSN=%s;UID=%s;PWD=%s;"),
                (LPCTSTR)dsn, (LPCTSTR)user, (LPCTSTR)password);
        }
        else
        {
            // Truyền tường minh -> override database đã cấu hình trong DSN
            connectStr.Format(_T("ODBC;DSN=%s;UID=%s;PWD=%s;DATABASE=%s;"),
                (LPCTSTR)dsn, (LPCTSTR)user, (LPCTSTR)password, (LPCTSTR)database);
        }

        // noOdbcDialog: không hiện popup chọn DSN, dùng thẳng connectStr
        m_database.OpenEx(connectStr, CDatabase::noOdbcDialog);
        m_bConnected = true;
    }
        CATCH(CDBException, e)
    {
        CString msg;
        msg.Format(_T("Database connection failed: %s"), e->m_strError);
        AfxMessageBox(msg);
        m_bConnected = false;
    }
    END_CATCH

        return m_bConnected;
}

bool CDatabaseManager::ConnectDirect(const CString& driverName, const CString& server, const CString& port,
    const CString& database, const CString& user, const CString& password)
{
    if (m_bConnected)
        return true;

    TRY
    {
        CString connectStr;
        connectStr.Format(_T("DRIVER={%s};SERVER=%s;PORT=%s;DATABASE=%s;UID=%s;PWD=%s;"),
            (LPCTSTR)driverName, (LPCTSTR)server, (LPCTSTR)port,
            (LPCTSTR)database, (LPCTSTR)user, (LPCTSTR)password);

        m_database.OpenEx(connectStr, CDatabase::noOdbcDialog);
        m_bConnected = true;
    }
        CATCH(CDBException, e)
    {
        CString msg;
        msg.Format(_T("Database connection failed: %s"), e->m_strError);
        AfxMessageBox(msg);
        m_bConnected = false;
    }
    END_CATCH

        return m_bConnected;
}

void CDatabaseManager::Disconnect()
{
    if (m_bConnected && m_database.IsOpen())
    {
        m_database.Close();
    }
    m_bConnected = false;
}

CDatabase& CDatabaseManager::GetDatabase()
{
    return m_database;
}

bool CDatabaseManager::IsConnected() const
{
    return m_bConnected;
}