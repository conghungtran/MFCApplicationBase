#pragma once
#include "afxdialogex.h"


// PageFunction dialog

class PageFunction : public CDialogEx
{
	DECLARE_DYNAMIC(PageFunction)

public:
	PageFunction(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PageFunction();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_FIRMWARE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
