#pragma once
#include <afxdb.h>
#include "DBConfig.h"

class CDatabaseManager
{
public:
    CDatabaseManager();
    ~CDatabaseManager();

    bool Connect(const DBConfig& config);
    void Disconnect();

    CDatabase& GetDatabase();
    bool IsConnected() const;

private:
    CDatabaseManager(const CDatabaseManager&) = delete;
    CDatabaseManager& operator=(const CDatabaseManager&) = delete;

    CDatabase m_database;
    bool      m_bConnected;
};