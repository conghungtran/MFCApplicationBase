
// MFCProjectView.h : interface of the MFCProjectView class
//

#pragma once
#include "CCustomButton.h"
#include "CCustomEdit.h"


class MFCProjectView : public CFormView
{
protected: // create from serialization only
	MFCProjectView() noexcept;
	DECLARE_DYNCREATE(MFCProjectView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_TESTPROJECT_FORM };
#endif

// Attributes
public:
	CMFCProjectDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~MFCProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CCustomButton m_btnAddPrinter;
	CCustomEdit m_editModel;
};

#ifndef _DEBUG  // debug version in MFCProjectView.cpp
inline CMFCProjectDoc* MFCProjectView::GetDocument() const
   { return reinterpret_cast<CMFCProjectDoc*>(m_pDocument); }
#endif

