#include "pch.h"

struct DBConfig {
	CString driverName;
	CString server;
	CString port;
	CString database;
	CString user;
	CString password;
};

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