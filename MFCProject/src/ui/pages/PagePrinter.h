#pragma once
#include "afxdialogex.h"
#include "CCustomListCtrl.h"
#include "CCustomButton.h"


// CPagePrinter dialog

class PagePrinter : public CDialogEx
{
	DECLARE_DYNAMIC(PagePrinter)

public:
	PagePrinter(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PagePrinter();
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	
	BOOL InitToolBar();

	void InitControlList();
	void AddSampleData();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_PRINTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnEditItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteItem(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	CCustomListCtrl m_listCtrl;

	CImageList m_imageList;
	CToolBarCtrl m_wndToolBar;

	CCustomButton m_btnAdd;
	CCustomButton m_btnDelete;
	CCustomButton m_btnRefresh;
};
