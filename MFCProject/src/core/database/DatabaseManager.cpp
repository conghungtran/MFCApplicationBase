#include "pch.h"
#include "DatabaseManager.h"
#include "DBConfig.h"

CDatabaseManager::CDatabaseManager()
	: m_bConnected(false)
{
}

CDatabaseManager::~CDatabaseManager()
{
	Disconnect();
}

bool CDatabaseManager::Connect(const DBConfig& config) {
	if (IsConnected())
		return true;

	try {
		CString connectStr;
		connectStr.Format(_T("DRIVER={%s};SERVER=%s;PORT=%s;DATABASE=%s;UID=%s;PWD=%s;"),
			(LPCTSTR)config.driverName,
			(LPCTSTR)config.server,
			(LPCTSTR)config.port,
			(LPCTSTR)config.database,
			(LPCTSTR)config.user,
			(LPCTSTR)config.password
		);

		m_database.OpenEx(connectStr, CDatabase::noOdbcDialog);
		m_bConnected = true;
	} catch (CDBException* e) {
		m_bConnected = false;
		e->Delete();
	}

	return m_bConnected;
}

void CDatabaseManager::Disconnect() {
	if (m_database.IsOpen())
		m_database.Close();
	m_bConnected = false;
}

CDatabase& CDatabaseManager::GetDatabase() {
	return m_database;
}

bool CDatabaseManager::IsConnected() const {
	return m_bConnected && m_database.IsOpen();
}