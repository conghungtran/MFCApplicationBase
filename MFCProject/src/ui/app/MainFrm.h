
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "CustomTaskPane.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	void OnFileOpen();
	void ReplaceToolbarWithShellIcons();

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);
	

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

	CMFCTasksPane m_wndTasksPane;

	CSidePanelPane m_wndSidePane;

	BOOL CreateSidePane();
// Generated message map functions
protected:
	BOOL CreateTaskPane();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);

	afx_msg void OnPagePrinter();
	afx_msg void OnPageTicket();
	afx_msg void OnPageFirmware();




	DECLARE_MESSAGE_MAP()

};


