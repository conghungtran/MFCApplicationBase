#pragma once

#include "pch.h"
#include "DBConfig.h"

class IDatabaseManager {
public:
	virtual ~IDatabaseManager() = default;
	virtual bool Connect(const DBConfig& config) = 0;
	virtual void Disconnect() = 0;
	virtual CDatabase& GetDatabase() = 0;
	virtual bool IsConnected() const = 0;
};
