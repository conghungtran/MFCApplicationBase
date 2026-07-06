// AppToolBar.h
#pragma once
#include "CSearchCtrl.h"
#include "CCustomButton.h"

class BookPageToolBar : public CWnd
{
    DECLARE_DYNAMIC(BookPageToolBar)

public:
    BOOL Create(CWnd* pParent, UINT nID, const CRect& rect);

protected:
    CCustomButton  m_btnAdd;
    CCustomButton  m_btnDelete;
    CCustomButton  m_btnRefresh;
    CSearchCtrl    m_searchCtrl;

    afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg LRESULT OnSearch(WPARAM wParam, LPARAM lParam);
    afx_msg void OnAddClicked();
    afx_msg void OnDeleteClicked();

    DECLARE_MESSAGE_MAP()


private:
    void RepositionControls();
    void CreateButtons();
};