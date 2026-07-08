
// MFCProject.h : main header file for the MFCProject application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "IDatabaseManager.h"
#include <memory>


// CMFCProjectApp:
// See MFCProject.cpp for the implementation of this class
//

class CMFCProjectApp : public CWinAppEx
{
public:
	//CMFCProjectApp() noexcept;
	CMFCProjectApp(std::unique_ptr<IDatabaseManager> pDatabaseManager) noexcept;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
	std::unique_ptr<IDatabaseManager> m_pDatabaseManager;

public:
	IDatabaseManager* GetDatabaseManager();
};

extern CMFCProjectApp theApp;
