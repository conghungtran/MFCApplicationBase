#pragma once
#include "afxdialogex.h"
#include "CCustomListCtrl.h"
#include "CCustomButton.h"
#include "BookPageToolBar.h"
#include "CPaginationBar.h"
#include "Book.h"
#include <memory>
#include "BookService.h"
#include "ImportExportService.h"

// CPageBook dialog

class PageBook : public CDialogEx
{
	DECLARE_DYNAMIC(PageBook)

public:
	PageBook(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PageBook();
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void SetBookService(std::shared_ptr<CBookService> service) { 
		m_bookService = service;
	}

	void SetImportExportService(std::shared_ptr<CImportExportService> importService) {
		m_wndToolBar.SetImportExportService(importService);
	}
	
	BOOL InitToolBar();

	void InitControlList();
	void InitTable();
	void LoadData();
	void UpdateSortArrow(int nSortedCol, bool bAscending);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_PRINTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnEditItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddBook(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnImportBook(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExportBook(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLvnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);


	afx_msg LRESULT OnBnClickedBtnClear(WPARAM wParam, LPARAM lParam);
	void AddBookToListCtrl(const Book& book);

	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	CCustomListCtrl m_listCtrl;

	CImageList m_imageList;
	BookPageToolBar m_wndToolBar;
	std::shared_ptr<CBookService> m_bookService;


protected:
	CBrush m_brushWhite;

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CPaginationBar  m_pagination;

	// Phân trang
	int m_nCurrentPage = 1;
	int m_nPageSize = 20;
	int m_nTotalRecords = 0;

	afx_msg LRESULT OnPageChanged(WPARAM wParam, LPARAM lParam);
	void LoadPage(int nPage);
	void RefreshTotalCount();


private:
	int  m_nSortColumn = -1;      // cột đang sort, -1 = chưa sort
	bool m_bSortAscending = true; // chiều sort hiện tại

};
