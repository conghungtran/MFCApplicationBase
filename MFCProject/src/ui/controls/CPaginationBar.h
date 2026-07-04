#pragma once
#include <memory>
#include <vector>
#include "CCustomButton.h"

// Khai báo trong AppMessages.h
// #define WM_PAGE_CHANGED (WM_USER + 110)

class CPaginationBar : public CWnd
{
    DECLARE_DYNAMIC(CPaginationBar)

public:
    BOOL Create(CWnd* pParent, UINT nID, const CRect& rect);

    // Gọi từ bên ngoài để cập nhật trạng thái
    void SetPageInfo(int nCurrentPage, int nTotalPages);

    int GetCurrentPage() const { return m_nCurrentPage; }
    int GetTotalPages()  const { return m_nTotalPages; }

protected:
    afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnBtnPrev();
    afx_msg void OnBtnNext();
    afx_msg void OnBtnPage(UINT nID);

    DECLARE_MESSAGE_MAP()

private:
    static const int ID_BTN_PREV = 2001;
    static const int ID_BTN_NEXT = 2002;
    static const int ID_PAGE_BTN_BASE = 2010;
    static const int MAX_PAGE_BTNS = 7;

    int m_nCurrentPage = 1;
    int m_nTotalPages = 1;

    CCustomButton m_btnPrev;
    CCustomButton m_btnNext;
    std::vector<std::unique_ptr<CCustomButton>> m_pageBtns;

    void RebuildPageButtons();
    void LayoutChildren();
    void NotifyParent();
};