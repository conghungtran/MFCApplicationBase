#pragma once
#include "CCustomButton.h"

//#define WM_SEARCH (WM_USER + 200)

class CSearchCtrl : public CWnd
{
    DECLARE_DYNAMIC(CSearchCtrl)

public:
    CSearchCtrl();
    BOOL Create(CWnd* pParent, UINT nID, const CRect& rect);
    CString GetSearchText() const;

private:
    CEdit   m_edit;
    CCustomButton m_btn;

    void LayoutChildren();

protected:
    afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    afx_msg void OnPaint();
    afx_msg void OnBtnSearch();
    afx_msg void OnEditChange();
    DECLARE_MESSAGE_MAP()
};