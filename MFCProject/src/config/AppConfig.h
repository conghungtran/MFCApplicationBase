#include "pch.h"
#include "DBConfig.h"

class AppConfig final {
public:
	static AppConfig& Instance();

	const DBConfig& GetDBConfig() const;

	AppConfig(const AppConfig&) = delete;
	AppConfig& operator=(const AppConfig&) = delete;

private:
	AppConfig();

	DBConfig m_dbConfig;
};