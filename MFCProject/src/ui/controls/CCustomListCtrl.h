#pragma once
#include <afxcmn.h>

// Custom messages
#define WM_EDIT_ITEM   (WM_USER + 100)
#define WM_DELETE_ITEM (WM_USER + 101)

// Column indices
#define COL_NAME    0
#define COL_STATUS  1
#define COL_ACTION  2

class CCustomListCtrl : public CListCtrl
{
    DECLARE_DYNAMIC(CCustomListCtrl)

public:
    CCustomListCtrl();
    virtual ~CCustomListCtrl();

private:
    // Tính toán rect của từng button trong cột Action
    void GetEditBtnRect(int nRow, CRect& rect);
    void GetDeleteBtnRect(int nRow, CRect& rect);

    // Vẽ button giả
    void DrawActionButtons(CDC* pDC, int nRow, CRect cellRect);

protected:
    afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    DECLARE_MESSAGE_MAP()
};