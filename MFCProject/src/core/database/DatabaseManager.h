#pragma once
#include <afxdb.h>

// ==============================
// Quản lý kết nối ODBC (Singleton)
// Dùng chung cho SQL Server / MySQL / bất kỳ DB nào có driver ODBC
// - Với MySQL: DSN trỏ tới driver "MySQL ODBC 8.x Driver"
// ==============================
class CDatabaseManager
{
public:
    static CDatabaseManager& Instance();

    // dsn = tên ODBC Data Source Name đã khai báo trong ODBC Data Source Administrator
    bool Connect(const CString& dsn, const CString& user = _T(""), const CString& password = _T(""));
    void Disconnect();

    CDatabase& GetDatabase();
    bool IsConnected() const;

private:
    CDatabaseManager();
    ~CDatabaseManager();
    CDatabaseManager(const CDatabaseManager&) = delete;
    CDatabaseManager& operator=(const CDatabaseManager&) = delete;

    CDatabase m_database;
    bool      m_bConnected;
};