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

bool CDatabaseManager::Connect(const CString& dsn, const CString& user, const CString& password)
{
    if (m_bConnected)
        return true;

    TRY
    {
        CString connectStr;
        connectStr.Format(_T("ODBC;DSN=%s;UID=%s;PWD=%s;"), dsn, user, password);

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