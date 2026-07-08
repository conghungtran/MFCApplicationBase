#pragma once

#include "pch.h"
#include "DBConfig.h"
#include "IDatabaseManager.h"

class CDatabaseManager : public IDatabaseManager {
public:
    CDatabaseManager();
    ~CDatabaseManager() override;

    bool Connect(const DBConfig& config) override;
    void Disconnect() override;

    CDatabase& GetDatabase() override;
    bool IsConnected() const override;

private:
    CDatabaseManager(const CDatabaseManager&) = delete;
    CDatabaseManager& operator=(const CDatabaseManager&) = delete;

    CDatabase m_database;
    bool      m_bConnected;
};