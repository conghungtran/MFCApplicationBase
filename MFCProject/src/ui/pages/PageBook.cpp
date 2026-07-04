#include "pch.h"
#include "afxdialogex.h"
#include "PageBook.h"
#include "Resource.h"
#include <iostream>
#include <commoncontrols.h>
#pragma comment(lib, "comctl32.lib")
static HICON GetShellIcon(SHSTOCKICONID id, int size = 40)
{
    SHSTOCKICONINFO sii = {};
    sii.cbSize = sizeof(sii);
    SHGetStockIconInfo(id, SHGSI_ICON | SHGSI_LARGEICON, &sii);
    return sii.hIcon;
};

// PageBook dialog

IMPLEMENT_DYNAMIC(PageBook, CDialogEx)

PageBook::PageBook(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_BOOK, pParent)
{
   
}

PageBook::~PageBook()
{
}


void PageBook::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (!m_wndToolBar.GetSafeHwnd() ||
        !m_listCtrl.GetSafeHwnd() ||
        !m_pagination.GetSafeHwnd())
        return;

    const int TOOLBAR_H = 56;
    const int PAGINATION_H = 44;

    m_wndToolBar.MoveWindow(0, 0, cx, TOOLBAR_H);
    m_listCtrl.MoveWindow(0, TOOLBAR_H, cx, cy - TOOLBAR_H - PAGINATION_H);
    m_pagination.MoveWindow(0, cy - PAGINATION_H, cx, PAGINATION_H);
}

void PageBook::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void PageBook::RefreshTotalCount()
{
    // TODO: đổi thành m_printerRepo->Count() khi có repository
    m_nTotalRecords = m_listCtrl.GetItemCount();

    int nTotalPages = max(1, (m_nTotalRecords + m_nPageSize - 1) / m_nPageSize);
    m_pagination.SetPageInfo(m_nCurrentPage, nTotalPages);
}
void PageBook::LoadPage(int nPage)
{
    m_nCurrentPage = nPage;

    // TODO: đổi thành repo->GetPaged(offset, m_nPageSize) khi có repository
    // Hiện tại data đã load hết trong AddSampleData() nên chỉ update pagination UI
    int nTotalPages = max(1, (m_nTotalRecords + m_nPageSize - 1) / m_nPageSize);
    m_pagination.SetPageInfo(m_nCurrentPage, nTotalPages);
}

LRESULT PageBook::OnPageChanged(WPARAM wParam, LPARAM lParam)
{
    int nPage = (int)wParam;
    LoadPage(nPage);
    return 0;
}



BEGIN_MESSAGE_MAP(PageBook, CDialogEx)
	ON_MESSAGE(WM_EDIT_ITEM, &PageBook::OnEditItem)
	ON_MESSAGE(WM_DELETE_ITEM, &PageBook::OnDeleteItem)
    ON_MESSAGE(WM_PAGE_CHANGED, &PageBook::OnPageChanged)
    ON_WM_SIZE()
    ON_WM_SETCURSOR()
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL PageBook::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // Nếu con trỏ đang ở trên toolbar
    if (pWnd->GetSafeHwnd() == m_wndToolBar.GetSafeHwnd())
    {
        ::SetCursor(LoadCursor(NULL, IDC_HAND));
        return TRUE; // Đã xử lý
    }

    // Nếu không phải toolbar, gọi base class
    return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

BOOL PageBook::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CRect clientRect;
    GetClientRect(&clientRect);
    m_brushWhite.CreateSolidBrush(RGB(255, 255, 255));

    const int TOOLBAR_H = 56;
    const int PAGINATION_H = 44;

    // ── Toolbar ───────────────────────────────────────────────────
    m_wndToolBar.Create(this, IDC_TOOLBAR_BOOK,
        CRect(0, 0, clientRect.Width(), TOOLBAR_H));

    m_imageList.Create(40, 40, ILC_COLOR32 | ILC_MASK, 4, 4);

    // ── List Ctrl ─────────────────────────────────────────────────
    m_listCtrl.Create(
        WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
        CRect(0, TOOLBAR_H, clientRect.Width(),
            clientRect.Height() - PAGINATION_H),
        this, IDC_LIST_BOOK
    );
    m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // ── Pagination ────────────────────────────────────────────────
    m_pagination.Create(this, ID_PAGINATION,
        CRect(0, clientRect.Height() - PAGINATION_H,
            clientRect.Width(), clientRect.Height()));

    AddSampleData();
    RefreshTotalCount();
    LoadPage(1);

    return TRUE;
}

HBRUSH PageBook::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(RGB(255, 255, 255));
        return (HBRUSH)m_brushWhite.GetSafeHandle();
    }

    return hbr;
}

BOOL PageBook::OnEraseBkgnd(CDC* pDC)
{
    CRect rc;
    GetClientRect(&rc);
    pDC->FillSolidRect(&rc, RGB(255, 255, 255));
    return TRUE;
}

void PageBook::AddSampleData()
{
    m_listCtrl.InsertColumn(COL_NAME, _T("Printer Name"), LVCFMT_LEFT, 200);
    m_listCtrl.InsertColumn(COL_STATUS, _T("Status"), LVCFMT_LEFT, 120);
    m_listCtrl.InsertColumn(COL_ACTION, _T("Action"), LVCFMT_LEFT, 150);

    // Thêm 25 dòng để test pagination (page size = 20 → 2 trang)
    CString name, status;
    for (int i = 0; i < 25; ++i)
    {
        name.Format(_T("Printer %02d"), i + 1);
        status = (i % 3 == 0) ? _T("Offline") : _T("Online");

        int idx = m_listCtrl.InsertItem(i, name);
        m_listCtrl.SetItemText(idx, COL_STATUS, status);
    }

    // Cập nhật total sau khi add data
    m_nTotalRecords = m_listCtrl.GetItemCount();
}

LRESULT PageBook::OnEditItem(WPARAM wParam, LPARAM lParam)
{
    int nRow = (int)wParam;
    CString strName = m_listCtrl.GetItemText(nRow, COL_NAME);

    // Mở dialog Edit
    AfxMessageBox(_T("Edit: ") + strName);
    return 0;
}

LRESULT PageBook::OnDeleteItem(WPARAM wParam, LPARAM lParam)
{
    int nRow = (int)wParam;
    CString strName = m_listCtrl.GetItemText(nRow, COL_NAME);

    if (AfxMessageBox(_T("Delete ") + strName + _T("?"),
        MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        m_listCtrl.DeleteItem(nRow);
    }
    return 0;
}


// PageBook message handlers
