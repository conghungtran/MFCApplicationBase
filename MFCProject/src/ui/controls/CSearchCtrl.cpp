#include "pch.h"
#include "CSearchCtrl.h"
#include <iostream>
#include "Resource.h"

IMPLEMENT_DYNAMIC(CSearchCtrl, CWnd)

BEGIN_MESSAGE_MAP(CSearchCtrl, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_BN_CLICKED(1001, &CSearchCtrl::OnBtnSearch)
    ON_EN_CHANGE(1002, &CSearchCtrl::OnEditChange)

END_MESSAGE_MAP()

CSearchCtrl::CSearchCtrl() {}

BOOL CSearchCtrl::Create(CWnd* pParent, UINT nID, const CRect& rect)
{
    LPCTSTR cls = AfxRegisterWndClass(
        CS_HREDRAW | CS_VREDRAW,
        LoadCursor(nullptr, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1)
    );
    return CWnd::Create(cls, nullptr,
        WS_CHILD | WS_VISIBLE, rect, pParent, nID);
}

int CSearchCtrl::OnCreate(LPCREATESTRUCT lpcs)
{
    if (CWnd::OnCreate(lpcs) == -1) return -1;

    // Tạo Edit — không có border (tự vẽ)
    m_edit.Create(
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        CRect(0, 0, 0, 0), this, 1002
    );

    // Placeholder hint
    m_edit.SetCueBanner(L"Searching...", TRUE);

    m_btn.Create(_T(""),
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,  // 👈 QUAN TRỌNG: BS_OWNERDRAW
        CRect(10, 10, 90, 50), this, 1001);
    m_btn.SetColors(
        RGB(0, 120, 215),   // Normal
        RGB(0, 160, 240),   // Hover
        RGB(0, 80, 180),    // Pressed
        RGB(255, 255, 255)  // Text
    );
    SHSTOCKICONINFO sii = { sizeof(sii) };
    SHGetStockIconInfo(SIID_FIND, SHGSI_ICON | SHGSI_SMALLICON, &sii);
    m_btn.SetIcon(sii.hIcon);
    // Font chung
    CFont* pFont = GetParent()->GetFont();
    m_edit.SetFont(pFont);
    m_btn.SetFont(pFont);

    return 0;
}

void CSearchCtrl::LayoutChildren()
{
    CRect rc;
    GetClientRect(&rc);

    const int PAD_H = 10;
    const int PAD_V = 6;
    const int BTN_W = 60;
    const int GAP = 6;

    int editLeft = PAD_H + 24; // chừa chỗ icon search
    int editRight = rc.right - PAD_H - GAP - BTN_W;
    int editTop = PAD_V;
    int editBot = rc.bottom - PAD_V;

    m_edit.MoveWindow(editLeft, editTop,
        editRight - editLeft, editBot - editTop);

    m_btn.MoveWindow(editRight + GAP, PAD_V - 2,
        BTN_W, rc.Height() - (PAD_V - 2) * 2);
}

void CSearchCtrl::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    if (m_edit.GetSafeHwnd()) LayoutChildren();
}

void CSearchCtrl::OnPaint()
{
    CPaintDC dc(this);
    CRect rc;
    GetClientRect(&rc);

    // Nền trắng
    dc.FillSolidRect(rc, RGB(255, 255, 255));

    // Viền bo tròn
    CPen pen(PS_SOLID, 1, RGB(180, 180, 180));
    CPen* pOld = dc.SelectObject(&pen);
    CBrush brush(RGB(255, 255, 255));
    CBrush* pOldBrush = dc.SelectObject(&brush);

    dc.RoundRect(rc, CPoint(rc.Height(), rc.Height()));

    dc.SelectObject(pOld);
    dc.SelectObject(pOldBrush);

    // Icon search (dùng Shell icon)
    SHSTOCKICONINFO sii = {};
    sii.cbSize = sizeof(sii);
    SHGetStockIconInfo(SIID_FIND, SHGSI_ICON | SHGSI_SMALLICON, &sii);

    if (sii.hIcon)
    {
        DrawIconEx(dc.m_hDC, 8, (rc.Height() - 16) / 2,
            sii.hIcon, 16, 16, 0, nullptr, DI_NORMAL);
        DestroyIcon(sii.hIcon);
    }
}

CString CSearchCtrl::GetSearchText() const
{
    CString str;
    m_edit.GetWindowText(str);
    return str;
}

void CSearchCtrl::OnBtnSearch()
{
    CString text = GetSearchText();
    // Gửi lên parent kèm text
    GetParent()->PostMessage(WM_SEARCH, 0, (LPARAM)(LPCTSTR)text);
}

void CSearchCtrl::OnEditChange()
{
    // Gửi realtime nếu muốn search live
    // GetParent()->PostMessage(WM_SEARCH, 1, (LPARAM)(LPCTSTR)GetSearchText());
}
