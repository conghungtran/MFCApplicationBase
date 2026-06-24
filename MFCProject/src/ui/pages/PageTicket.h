#pragma once
#include "afxdialogex.h"


// PageTicket dialog

class PageTicket : public CDialogEx
{
	DECLARE_DYNAMIC(PageTicket)

public:
	PageTicket(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PageTicket();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_TICKET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
