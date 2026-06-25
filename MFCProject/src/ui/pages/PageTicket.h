#pragma once
#include "afxdialogex.h"
#include "CSearchCtrl.h"


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
	virtual BOOL OnInitDialog();
	LRESULT OnSearch(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()


public:
	CSearchCtrl m_search;
	CBrush m_brBackground;
};
