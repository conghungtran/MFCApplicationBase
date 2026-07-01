#include "pch.h"
#include "CPaginationBar.h"
#include <memory>
#include "Resource.h"

IMPLEMENT_DYNAMIC(CPaginationBar, CWnd)

BEGIN_MESSAGE_MAP(CPaginationBar, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_BN_CLICKED(2001, &CPaginationBar::OnBtnPrev)
    ON_BN_CLICKED(2002, &CPaginationBar::OnBtnNext)
    ON_COMMAND_RANGE(2010, 2010 + 7, &CPaginationBar::OnBtnPage)
END_MESSAGE_MAP()

// ─── Create ──────────────────────────────────────────────────────────────────

BOOL CPaginationBar::Create(CWnd* pParent, UINT nID, const CRect& rect)
{
    LPCTSTR cls = AfxRegisterWndClass(
        CS_HREDRAW | CS_VREDRAW,
        LoadCursor(nullptr, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1)
    );
    return CWnd::Create(cls, nullptr,
        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
        rect, pParent, nID);
}

int CPaginationBar::OnCreate(LPCREATESTRUCT lpcs)
{
    if (CWnd::OnCreate(lpcs) == -1) return -1;

    // ── Prev button ───────────────────────────────────────────────────────────
    m_btnPrev.Create(L"\u25c4 Prev",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
        CRect(0, 0, 0, 0), this, ID_BTN_PREV);
    m_btnPrev.SetColors(
        RGB(255, 255, 255),
        RGB(220, 235, 255),
        RGB(190, 215, 255),
        RGB(50, 50, 50)
    );

    // ── Next button ───────────────────────────────────────────────────────────
    m_btnNext.Create(L"Next \u25ba",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
        CRect(0, 0, 0, 0), this, ID_BTN_NEXT);
    m_btnNext.SetColors(
        RGB(255, 255, 255),
        RGB(220, 235, 255),
        RGB(190, 215, 255),
        RGB(50, 50, 50)
    );

    // ── Page number buttons ───────────────────────────────────────────────────
    for (int i = 0; i < MAX_PAGE_BTNS; ++i)
    {
        auto btn = std::make_unique<CCustomButton>();
        btn->Create(L"",
            WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,  // ẩn ban đầu (chưa WS_VISIBLE)
            CRect(0, 0, 0, 0), this, ID_PAGE_BTN_BASE + i);
        btn->SetColors(
            RGB(255, 255, 255),
            RGB(220, 235, 255),
            RGB(190, 215, 255),
            RGB(50, 50, 50)
        );
        m_pageBtns.push_back(std::move(btn));
    }

    return 0;
}

// ─── Public API ──────────────────────────────────────────────────────────────

void CPaginationBar::SetPageInfo(int nCurrentPage, int nTotalPages)
{
    m_nTotalPages = max(1, nTotalPages);
    m_nCurrentPage = max(1, min(nCurrentPage, m_nTotalPages));

    RebuildPageButtons();
    LayoutChildren();
}

// ─── Internal ────────────────────────────────────────────────────────────────

void CPaginationBar::RebuildPageButtons()
{
    // Tính dải trang hiển thị — căn giữa quanh trang hiện tại
    int half = MAX_PAGE_BTNS / 2;
    int startPage = max(1, m_nCurrentPage - half);
    int endPage = min(m_nTotalPages, startPage + MAX_PAGE_BTNS - 1);
    startPage = max(1, endPage - MAX_PAGE_BTNS + 1);  // điều chỉnh nếu endPage bị cắt

    for (int i = 0; i < MAX_PAGE_BTNS; ++i)
    {
        int page = startPage + i;
        auto& btn = m_pageBtns[i];

        if (page <= m_nTotalPages)
        {
            CString label;
            label.Format(L"%d", page);
            btn->SetWindowText(label);
            btn->ShowWindow(SW_SHOW);

            // Highlight trang hiện tại
            if (page == m_nCurrentPage)
                btn->SetColors(
                    RGB(0, 120, 215),   // Normal  — xanh
                    RGB(0, 100, 195),   // Hover
                    RGB(0, 80, 175),   // Pressed
                    RGB(255, 255, 255)    // Text trắng
                );
            else
                btn->SetColors(
                    RGB(255, 255, 255),
                    RGB(220, 235, 255),
                    RGB(190, 215, 255),
                    RGB(50, 50, 50)
                );

            btn->Invalidate();
        }
        else
        {
            btn->ShowWindow(SW_HIDE);
        }
    }

    // Disable Prev / Next ở đầu / cuối danh sách
    m_btnPrev.EnableWindow(m_nCurrentPage > 1);
    m_btnNext.EnableWindow(m_nCurrentPage < m_nTotalPages);
}

void CPaginationBar::LayoutChildren()
{
    CRect rc;
    GetClientRect(&rc);

    const int BTN_W = 72;   // Prev / Next
    const int PAGE_W = 36;   // nút số trang
    const int BTN_H = 30;
    const int GAP = 4;

    int y = (rc.Height() - BTN_H) / 2;

    // Đếm nút đang hiển thị để tính tổng width và căn giữa
    int visibleCount = 0;
    for (auto& btn : m_pageBtns)
        if (btn->IsWindowVisible()) visibleCount++;

    int totalW = BTN_W + GAP
        + visibleCount * (PAGE_W + GAP)
        + BTN_W;

    int x = (rc.Width() - totalW) / 2;

    // Prev
    m_btnPrev.MoveWindow(x, y, BTN_W, BTN_H);
    x += BTN_W + GAP;

    // Số trang
    for (auto& btn : m_pageBtns)
    {
        if (btn->IsWindowVisible())
        {
            btn->MoveWindow(x, y, PAGE_W, BTN_H);
            x += PAGE_W + GAP;
        }
    }

    // Next
    m_btnNext.MoveWindow(x, y, BTN_W, BTN_H);
}

void CPaginationBar::NotifyParent()
{
    if (GetParent())
        GetParent()->PostMessage(WM_PAGE_CHANGED,
            (WPARAM)m_nCurrentPage,
            (LPARAM)m_nTotalPages);
}

// ─── Handlers ────────────────────────────────────────────────────────────────

void CPaginationBar::OnBtnPrev()
{
    if (m_nCurrentPage > 1)
    {
        m_nCurrentPage--;
        RebuildPageButtons();
        LayoutChildren();
        NotifyParent();
    }
}

void CPaginationBar::OnBtnNext()
{
    if (m_nCurrentPage < m_nTotalPages)
    {
        m_nCurrentPage++;
        RebuildPageButtons();
        LayoutChildren();
        NotifyParent();
    }
}

void CPaginationBar::OnBtnPage(UINT nID)
{
    int idx = (int)nID - ID_PAGE_BTN_BASE;
    int half = MAX_PAGE_BTNS / 2;
    int startPage = max(1, m_nCurrentPage - half);
    int endPage = min(m_nTotalPages, startPage + MAX_PAGE_BTNS - 1);
    startPage = max(1, endPage - MAX_PAGE_BTNS + 1);

    int page = startPage + idx;

    if (page >= 1 && page <= m_nTotalPages && page != m_nCurrentPage)
    {
        m_nCurrentPage = page;
        RebuildPageButtons();
        LayoutChildren();
        NotifyParent();
    }
}

// ─── Paint ───────────────────────────────────────────────────────────────────

void CPaginationBar::OnPaint()
{
    CPaintDC dc(this);
    CRect rc;
    GetClientRect(&rc);

    // Nền trắng
    dc.FillSolidRect(rc, RGB(255, 255, 255));

    // Đường kẻ trên (phân cách với CListCtrl)
    CPen pen(PS_SOLID, 1, RGB(220, 220, 220));
    CPen* pOld = dc.SelectObject(&pen);
    dc.MoveTo(rc.left, rc.top);
    dc.LineTo(rc.right, rc.top);
    dc.SelectObject(pOld);
}

BOOL CPaginationBar::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;  // OnPaint lo hết
}

void CPaginationBar::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    if (m_btnPrev.GetSafeHwnd())
        LayoutChildren();
}