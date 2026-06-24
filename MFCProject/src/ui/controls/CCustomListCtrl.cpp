#include "pch.h"
#include "CCustomListCtrl.h"

IMPLEMENT_DYNAMIC(CCustomListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CCustomListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CCustomListCtrl::OnNMCustomdraw)
    ON_NOTIFY_REFLECT(NM_CLICK, &CCustomListCtrl::OnNMClick)
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// ─────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────
CCustomListCtrl::CCustomListCtrl() {}
CCustomListCtrl::~CCustomListCtrl() {}

// ─────────────────────────────────────────
// Helper: tính rect nút Edit trong 1 row
// ─────────────────────────────────────────
void CCustomListCtrl::GetEditBtnRect(int nRow, CRect& rect)
{
    CRect cellRect;
    GetSubItemRect(nRow, COL_ACTION, LVIR_BOUNDS, cellRect);

    rect = cellRect;
    rect.left += 4;
    rect.right = rect.left + 55;
    rect.top += 3;
    rect.bottom -= 3;
}

// ─────────────────────────────────────────
// Helper: tính rect nút Delete trong 1 row
// ─────────────────────────────────────────
void CCustomListCtrl::GetDeleteBtnRect(int nRow, CRect& rect)
{
    CRect editRect;
    GetEditBtnRect(nRow, editRect);

    rect = editRect;
    rect.left = editRect.right + 6;
    rect.right = rect.left + 65;
}

// ─────────────────────────────────────────
// Vẽ button Edit + Delete cho 1 row
// ─────────────────────────────────────────
void CCustomListCtrl::DrawActionButtons(CDC* pDC, int nRow, CRect cellRect)
{
    // --- Chuẩn bị font ---
    CFont font;
    font.CreatePointFont(85, _T("Segoe UI"));
    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->SetBkMode(TRANSPARENT);

    // ── Nút EDIT ──
    CRect editRect;
    GetEditBtnRect(nRow, editRect);

    pDC->FillSolidRect(editRect, RGB(0, 123, 255));     // Nền xanh

    // Viền nhẹ
    CPen penEdit(PS_SOLID, 1, RGB(0, 86, 179));
    CPen* pOldPen = pDC->SelectObject(&penEdit);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(editRect);
    pDC->SelectObject(pOldPen);

    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->DrawText(_T("Edit"), editRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // ── Nút DELETE ──
    CRect delRect;
    GetDeleteBtnRect(nRow, delRect);

    pDC->FillSolidRect(delRect, RGB(220, 53, 69));      // Nền đỏ

    CPen penDel(PS_SOLID, 1, RGB(176, 42, 55));
    pOldPen = pDC->SelectObject(&penDel);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->Rectangle(delRect);
    pDC->SelectObject(pOldPen);

    pDC->SetTextColor(RGB(255, 255, 255));
    pDC->DrawText(_T("Delete"), delRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    pDC->SelectObject(pOldFont);
}

// ─────────────────────────────────────────
// Custom Draw
// ─────────────────────────────────────────
void CCustomListCtrl::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLVCUSTOMDRAW pDraw = (LPNMLVCUSTOMDRAW)pNMHDR;
    *pResult = CDRF_DODEFAULT;

    switch (pDraw->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
        // Tô màu xen kẽ row
        if (pDraw->nmcd.dwItemSpec % 2 == 0)
            pDraw->clrTextBk = RGB(245, 245, 245);
        else
            pDraw->clrTextBk = RGB(255, 255, 255);

        pDraw->clrText = RGB(33, 33, 33);
        *pResult = CDRF_NOTIFYSUBITEMDRAW | CDRF_NEWFONT;
        break;

    case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
        if (pDraw->iSubItem == COL_ACTION)
        {
            CDC* pDC = CDC::FromHandle(pDraw->nmcd.hdc);

            // Xóa nền cell trước
            CRect cellRect;
            GetSubItemRect((int)pDraw->nmcd.dwItemSpec,
                COL_ACTION, LVIR_BOUNDS, cellRect);
            pDC->FillSolidRect(cellRect,
                pDraw->nmcd.dwItemSpec % 2 == 0
                ? RGB(245, 245, 245)
                : RGB(255, 255, 255));

            // Vẽ button
            DrawActionButtons(pDC, (int)pDraw->nmcd.dwItemSpec, cellRect);

            *pResult = CDRF_SKIPDEFAULT;
        }
        break;
    }
}

// ─────────────────────────────────────────
// Bắt click — xác định Edit hay Delete
// ─────────────────────────────────────────
void CCustomListCtrl::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pIA = (LPNMITEMACTIVATE)pNMHDR;
    *pResult = 0;

    if (pIA->iItem < 0 || pIA->iSubItem != COL_ACTION)
        return;

    CPoint pt = pIA->ptAction;

    CRect editRect, delRect;
    GetEditBtnRect(pIA->iItem, editRect);
    GetDeleteBtnRect(pIA->iItem, delRect);

    if (editRect.PtInRect(pt))
    {
        // Gửi message lên parent kèm index row
        GetParent()->PostMessage(WM_EDIT_ITEM, (WPARAM)pIA->iItem, 0);
    }
    else if (delRect.PtInRect(pt))
    {
        GetParent()->PostMessage(WM_DELETE_ITEM, (WPARAM)pIA->iItem, 0);
    }
}

// ─────────────────────────────────────────
// Đổi cursor khi hover vào vùng button
// ─────────────────────────────────────────
void CCustomListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    // Hit test xem có đang hover button nào không
    LVHITTESTINFO ht = {};
    ht.pt = point;
    SubItemHitTest(&ht);

    if (ht.iItem >= 0 && ht.iSubItem == COL_ACTION)
    {
        CRect editRect, delRect;
        GetEditBtnRect(ht.iItem, editRect);
        GetDeleteBtnRect(ht.iItem, delRect);

        if (editRect.PtInRect(point) || delRect.PtInRect(point))
        {
            SetCursor(LoadCursor(nullptr, IDC_HAND));
            return;
        }
    }

    CListCtrl::OnMouseMove(nFlags, point);
}