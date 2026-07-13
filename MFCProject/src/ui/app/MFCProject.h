
// MFCProject.h : main header file for the MFCProject application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "BookRepository.h"
#include "BookService.h"
#include "ImportExportService.h"

// CMFCProjectApp:
// See MFCProject.cpp for the implementation of this class
//

class CMFCProjectApp : public CWinAppEx
{
public:
	CMFCProjectApp() noexcept;

	// Attributes
public:
	// Composition Root - services dùng chung toàn app
	std::shared_ptr<CBookService> GetBookService() const { return m_bookService; }
	std::shared_ptr<CImportExportService> GetImportExportService() const { return m_importExportService; }

private:
	std::shared_ptr<CBookRepository> m_bookRepository;
	std::shared_ptr<CBookService> m_bookService;
	std::shared_ptr<CImportExportService> m_importExportService;

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
};

extern CMFCProjectApp theApp;
