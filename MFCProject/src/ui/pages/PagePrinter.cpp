#include "pch.h"
#include "afxdialogex.h"
#include "PagePrinter.h"
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

// PagePrinter dialog

IMPLEMENT_DYNAMIC(PagePrinter, CDialogEx)

PagePrinter::PagePrinter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_BOOK, pParent)
{
   
}

PagePrinter::~PagePrinter()
{
}


void PagePrinter::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (!m_wndToolBar.GetSafeHwnd() || !m_listCtrl.GetSafeHwnd())
        return;

    // Resize toolbar
    m_wndToolBar.SetWindowPos(NULL, 0, 0, cx, 52, SWP_NOZORDER);
    // Reposition toolbar trước
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

    // Lấy chiều cao toolbar sau reposition
    CRect toolbarRect;
    m_wndToolBar.GetWindowRect(&toolbarRect);
    ScreenToClient(&toolbarRect);

    m_listCtrl.SetWindowPos(nullptr,
        10, toolbarRect.bottom + 5,
        cx - 20, cy - toolbarRect.bottom - 15,
        SWP_NOZORDER);

    const int TOOLBAR_H = 56;
    const int PAGINATION_H = 44;

    m_wndToolBar.MoveWindow(0, 0, cx, TOOLBAR_H);
    m_listCtrl.MoveWindow(0, TOOLBAR_H, cx, cy - TOOLBAR_H - PAGINATION_H);
    m_pagination.MoveWindow(0, cy - PAGINATION_H, cx, PAGINATION_H);
    int top = toolbarRect.bottom + 5;

    // Resize list bên dưới
    m_listCtrl.SetWindowPos(NULL, 10, 57, cx - 20, cy - 67, SWP_NOZORDER);
}

void PagePrinter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void PagePrinter::RefreshTotalCount()
{
    // //Gọi repository đếm tổng
    //m_nTotalRecords = m_printerRepo->Count();

    //int nTotalPages = max(1, (m_nTotalRecords + m_nPageSize - 1) / m_nPageSize);
    //m_pagination.SetPageInfo(m_nCurrentPage, nTotalPages);
}

void PagePrinter::LoadPage(int nPage)
{
    //m_nCurrentPage = nPage;
    //int offset = (nPage - 1) * m_nPageSize;

    //// Gọi repository với LIMIT/OFFSET
    //auto printers = m_printerRepo->GetPaged(offset, m_nPageSize);

    //m_listCtrl.DeleteAllItems();
    //int row = 0;
    //for (const auto& p : printers)
    //{
    //    m_listCtrl.InsertItem(row, p.GetName().c_str());
    //    m_listCtrl.SetItemText(row, 1, p.GetStatus().c_str());
    //    m_listCtrl.SetItemText(row, 2, p.GetIP().c_str());
    //    row++;
    //}
}

LRESULT PagePrinter::OnPageChanged(WPARAM wParam, LPARAM lParam)
{
    int nPage = (int)wParam;
    LoadPage(nPage);
    return 0;
}


BEGIN_MESSAGE_MAP(PagePrinter, CDialogEx)
	ON_MESSAGE(WM_EDIT_ITEM, &PagePrinter::OnEditItem)
	ON_MESSAGE(WM_DELETE_ITEM, &PagePrinter::OnDeleteItem)
    ON_MESSAGE(WM_PAGE_CHANGED, &PagePrinter::OnPageChanged)

    ON_WM_SIZE()
    ON_WM_SETCURSOR()
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL PagePrinter::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
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


BOOL PagePrinter::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    const int TOOLBAR_H = 56;
    const int PAGINATION_H = 44;


    // ============================================================
    // 1. TẠO TOOLBAR bằng CToolBarCtrl
    // ============================================================
    CRect clientRect;
    GetClientRect(&clientRect);
    m_brushWhite.CreateSolidBrush(RGB(255, 255, 255));

    m_wndToolBar.Create(this, IDC_TOOLBAR_PRINTER,
        CRect(0, 0, clientRect.Width(), 56));

    // Image list
    m_imageList.Create(40, 40, ILC_COLOR32 | ILC_MASK, 4, 4);


    // ============================================================
    // 2. TẠO LIST CONTROL bên dưới toolbar
    // ============================================================
    CRect toolbarRect;
    m_wndToolBar.GetWindowRect(&toolbarRect);
    ScreenToClient(&toolbarRect);

    CRect listRect;
    listRect.left = 10;
    listRect.top = toolbarRect.bottom + 5;
    listRect.right = clientRect.right - 10;
    listRect.bottom = clientRect.bottom - 10;

    m_listCtrl.Create(
        WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
        listRect, this, IDC_LIST_PRINTER
    );

    // Create Pagging
    m_pagination.Create(this, ID_PAGINATION,
        CRect(0, clientRect.Height() - PAGINATION_H, clientRect.Width(), clientRect.Height()));

    m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    AddSampleData();
    RefreshTotalCount();
    LoadPage(1);
    return TRUE;
}

HBRUSH PagePrinter::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(RGB(255, 255, 255));
        return (HBRUSH)m_brushWhite.GetSafeHandle();
    }

    return hbr;
}

BOOL PagePrinter::OnEraseBkgnd(CDC* pDC)
{
    CRect rc;
    GetClientRect(&rc);
    pDC->FillSolidRect(&rc, RGB(255, 255, 255));
    return TRUE;
}

void PagePrinter::AddSampleData() {
    m_listCtrl.InsertColumn(COL_NAME, _T("Printer Name"), LVCFMT_LEFT, 200);
    m_listCtrl.InsertColumn(COL_STATUS, _T("Status"), LVCFMT_LEFT, 120);
    m_listCtrl.InsertColumn(COL_ACTION, _T("Action"), LVCFMT_LEFT, 150);

    int idx = m_listCtrl.InsertItem(0, _T("HP LaserJet 1020"));
    m_listCtrl.SetItemText(idx, COL_STATUS, _T("Online"));

    idx = m_listCtrl.InsertItem(1, _T("Canon LBP2900"));
    m_listCtrl.SetItemText(idx, COL_STATUS, _T("Offline"));

    idx = m_listCtrl.InsertItem(2, _T("Toshiba GBX113"));
    m_listCtrl.SetItemText(idx, COL_STATUS, _T("Offline"));
}


LRESULT PagePrinter::OnEditItem(WPARAM wParam, LPARAM lParam)
{
    int nRow = (int)wParam;
    CString strName = m_listCtrl.GetItemText(nRow, COL_NAME);

    // Mở dialog Edit
    AfxMessageBox(_T("Edit: ") + strName);
    return 0;
}

LRESULT PagePrinter::OnDeleteItem(WPARAM wParam, LPARAM lParam)
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


// PagePrinter message handlers
