#include "pch.h"
#include "AppConfig.h"

AppConfig& AppConfig::Instance()
{
    static AppConfig instance;
    return instance;
}

AppConfig::AppConfig()
{
    CString iniPath;
    TCHAR buf[MAX_PATH];

    GetModuleFileName(nullptr, buf, MAX_PATH);
    CString dir(buf);
    int pos = dir.ReverseFind(_T('\\'));
    if (pos >= 0)
        dir = dir.Left(pos + 1);

    while (iniPath.IsEmpty())
    {
        CString candidate = dir + _T("config.ini");
        WIN32_FIND_DATA fd;
        HANDLE h = FindFirstFile(candidate, &fd);
        if (h != INVALID_HANDLE_VALUE)
        {
            FindClose(h);
            iniPath = candidate;
            break;
        }
        FindClose(h);

        CString parent = dir.Left(dir.GetLength() - 1);
        pos = parent.ReverseFind(_T('\\'));
        if (pos < 0)
            break;
        CString next = dir.Left(pos + 1);
        if (next == dir)
            break;
        dir = next;
    }

    if (iniPath.IsEmpty())
    {
        AfxMessageBox(_T("config.ini not found"));
        return;
    }

    auto read = [&](const TCHAR* key, CString& out) {
        TCHAR buffer[256];
        GetPrivateProfileString(_T("Database"), key, _T(""),
            buffer, 256, iniPath);
        out = buffer;
        };

    read(_T("Driver"),   m_dbConfig.driverName);
    read(_T("Server"),   m_dbConfig.server);
    read(_T("Port"),     m_dbConfig.port);
    read(_T("Database"), m_dbConfig.database);
    read(_T("UID"),      m_dbConfig.user);
    read(_T("PWD"),      m_dbConfig.password);
}

const DBConfig& AppConfig::GetDBConfig() const
{
    return m_dbConfig;
}
