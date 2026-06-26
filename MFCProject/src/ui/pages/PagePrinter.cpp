// PagePrinter.cpp : implementation file
//

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
	: CDialogEx(IDD_PAGE_PRINTER, pParent)
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
    StretchSeparators();
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

    //int top = toolbarRect.bottom + 5;

    //// Resize list bên dưới
    //m_listCtrl.SetWindowPos(NULL, 10, 57, cx - 20, cy - 67, SWP_NOZORDER);
}

void PagePrinter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PagePrinter, CDialogEx)
	ON_MESSAGE(WM_EDIT_ITEM, &PagePrinter::OnEditItem)
	ON_MESSAGE(WM_DELETE_ITEM, &PagePrinter::OnDeleteItem)
    ON_WM_SIZE()
    ON_WM_SETCURSOR()
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


// OnInit ToolBar + ControlList
 
//BOOL PagePrinter::OnInitDialog()
//{
//    CDialogEx::OnInitDialog();
//
//    // ============================================================
//    // 1. TẠO TOOLBAR
//    // ============================================================
//    if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP,
//        AFX_IDW_TOOLBAR))
//    {
//        TRACE0("Failed to create toolbar\n");
//        return FALSE;
//    }
//
//    // Tạo image list
//    m_imageList.Create(32, 32, ILC_COLOR32 | ILC_MASK, 4, 4);
//
//
//    // Load icon từ resource
//    HICON hIconAdd = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_ADD1));
//    HICON hIconDelete = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_DELETE1));
//    HICON hIconRefresh = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_REFRESH1));
//
//    // Thêm icon vào image list
//    if (hIconAdd) m_imageList.Add(hIconAdd);
//    if (hIconDelete) m_imageList.Add(hIconDelete);
//    if (hIconRefresh) m_imageList.Add(hIconRefresh);
//
//    // Gán image list cho toolbar
//    m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imageList);
//
//    // Thêm các nút vào toolbar
//    TBBUTTON buttons[3] = {};
//    buttons[0].iBitmap = 0;  // Index trong image list
//    buttons[0].idCommand = ID_ADD_PRINTER;
//    buttons[0].fsState = TBSTATE_ENABLED;
//    buttons[0].fsStyle = TBSTYLE_BUTTON;
//
//    buttons[1].iBitmap = 1;
//    buttons[1].idCommand = ID_DELETE_PRINTER;
//    buttons[1].fsState = TBSTATE_ENABLED;
//    buttons[1].fsStyle = TBSTYLE_BUTTON;
//
//    buttons[2].iBitmap = 2;
//    buttons[2].idCommand = ID_REFRESH;
//    buttons[2].fsState = TBSTATE_ENABLED;
//    buttons[2].fsStyle = TBSTYLE_BUTTON;
//
//
//    // Gán image list
//    m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imageList);
//
//    // 3. BitmapSize = 32, ButtonSize = 32 + 12
//    m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(32, 32));
//    m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(44, 44));
//
//    m_wndToolBar.GetToolBarCtrl().AddButtons(3, buttons);
//
//    // ============================================================
//    // 2. TẠO LIST CONTROL (nằm bên dưới toolbar)
//    // ============================================================
//
//    // Lấy kích thước của vùng client (đã trừ đi toolbar)
//    CRect rect;
//    GetClientRect(&rect);
//
//    // 👇 QUAN TRỌNG: Lấy vị trí của toolbar để tính toán offset
//    CRect toolbarRect;
//    m_wndToolBar.GetWindowRect(&toolbarRect);
//    ScreenToClient(&toolbarRect);
//
//    // Điều chỉnh vùng rect cho list control
//    // - Top: bắt đầu từ dưới toolbar (toolbarRect.bottom)
//    // - Bottom: sát đáy dialog
//    rect.top = toolbarRect.bottom + 5;  // +5 là khoảng cách padding
//    rect.left += 10;
//    rect.right -= 10;
//    rect.bottom -= 10;
//
//    // Tạo list control
//    m_listCtrl.Create(
//        WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
//        rect, this, IDC_LIST_PRINTER
//    );
//
//    // Thiết lập style cho list control
//    m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
//
//    // Thêm cột
//    m_listCtrl.InsertColumn(COL_NAME, _T("Printer Name"), LVCFMT_LEFT, 200);
//    m_listCtrl.InsertColumn(COL_STATUS, _T("Status"), LVCFMT_LEFT, 120);
//    m_listCtrl.InsertColumn(COL_ACTION, _T("Action"), LVCFMT_LEFT, 150);
//
//    // Thêm dữ liệu mẫu
//    int idx = m_listCtrl.InsertItem(0, _T("HP LaserJet 1020"));
//    m_listCtrl.SetItemText(idx, COL_STATUS, _T("Online"));
//
//    idx = m_listCtrl.InsertItem(1, _T("Canon LBP2900"));
//    m_listCtrl.SetItemText(idx, COL_STATUS, _T("Offline"));
//
//    return TRUE;
//}


BOOL PagePrinter::OnInitDialog()
{
    CDialogEx::OnInitDialog();



    // ============================================================
    // 1. TẠO TOOLBAR bằng CToolBarCtrl
    // ============================================================
    CRect clientRect;
    GetClientRect(&clientRect);
    
    // Tạo toolbar chiếm toàn bộ chiều rộng, cao 52px
    m_wndToolBar.Create(
        WS_CHILD | WS_VISIBLE | CCS_TOP | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
        CRect(0, 0, clientRect.Width(), 52),
        this,
        IDC_TOOLBAR_PRINTER
    );
    // Image list
    m_imageList.Create(40, 40, ILC_COLOR32 | ILC_MASK, 4, 4);

    HICON hIconAdd = GetShellIcon(SIID_PRINTER);
    HICON hIconDelete = GetShellIcon(SIID_DELETE);
    HICON hIconRefresh = GetShellIcon(SIID_FIND);

    if (hIconAdd)     m_imageList.Add(hIconAdd);
    if (hIconDelete)  m_imageList.Add(hIconDelete);
    if (hIconRefresh) m_imageList.Add(hIconRefresh);
    if (hIconRefresh) m_imageList.Add(hIconRefresh);
    if (hIconRefresh) m_imageList.Add(hIconRefresh);

    m_wndToolBar.SetImageList(&m_imageList);

    // Set size TRƯỚC AddButtons
    m_wndToolBar.SetBitmapSize(CSize(40, 40));
    m_wndToolBar.SetButtonSize(CSize(52, 52));

    // Thêm buttons — có separator ở giữa
    TBBUTTON buttons[7] = {};

    // ── TRÁI ──
    buttons[0].iBitmap = 0;
    buttons[0].idCommand = ID_ADD_PRINTER;
    buttons[0].fsState = TBSTATE_ENABLED;
    buttons[0].fsStyle = TBSTYLE_BUTTON;

    buttons[1].iBitmap = 1;
    buttons[1].idCommand = ID_DELETE_PRINTER;
    buttons[1].fsState = TBSTATE_ENABLED;
    buttons[1].fsStyle = TBSTYLE_BUTTON;

    // ── SPACER trái → giữa ──
    buttons[2].iBitmap = -1;
    buttons[2].idCommand = 0;
    buttons[2].fsState = TBSTATE_ENABLED;
    buttons[2].fsStyle = TBSTYLE_SEP;

    // ── GIỮA ──
    buttons[3].iBitmap = 2;
    buttons[3].idCommand = ID_REFRESH;
    buttons[3].fsState = TBSTATE_ENABLED;
    buttons[3].fsStyle = TBSTYLE_BUTTON;

    // ── SPACER giữa → phải ──
    buttons[4].iBitmap = -1;
    buttons[4].idCommand = 0;
    buttons[4].fsState = TBSTATE_ENABLED;
    buttons[4].fsStyle = TBSTYLE_SEP;

    // ── PHẢI ──
    buttons[5].iBitmap = 3;
    buttons[5].idCommand = ID_APP_ABOUT;
    buttons[5].fsState = TBSTATE_ENABLED;
    buttons[5].fsStyle = TBSTYLE_BUTTON;

    buttons[6].iBitmap = 4;
    buttons[6].idCommand = ID_HELP;
    buttons[6].fsState = TBSTATE_ENABLED;
    buttons[6].fsStyle = TBSTYLE_BUTTON;

    m_wndToolBar.AddButtons(7, buttons);
    m_wndToolBar.AutoSize();

    // Gọi SAU AutoSize
    StretchSeparators();

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

    m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    AddSampleData();

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

//BOOL PagePrinter::OnInitDialog()
//{
//    CDialogEx::OnInitDialog();
//
//    
//
//
//    //InitToolBar();
//    //InitControlList();
//
//    return TRUE;
//}

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


void PagePrinter::StretchSeparators()
{
    CRect rcToolbar;
    m_wndToolBar.GetClientRect(&rcToolbar);
    int totalWidth = rcToolbar.Width();
    if (totalWidth <= 0) return;

    // Tính tổng width button thật
    int btnTotalWidth = 0;
    int separatorCount = 0;
    int count = m_wndToolBar.GetButtonCount();

    for (int i = 0; i < count; i++)
    {
        TBBUTTON tb = {};
        m_wndToolBar.GetButton(i, &tb);

        if (tb.fsStyle & TBSTYLE_SEP)
            separatorCount++;
        else
        {
            CRect rc;
            m_wndToolBar.GetItemRect(i, &rc);
            btnTotalWidth += rc.Width();
        }
    }

    if (separatorCount == 0) return;

    int separatorWidth = max((totalWidth - btnTotalWidth) / separatorCount, 8);

    for (int i = 0; i < count; i++)
    {
        TBBUTTON tb = {};
        m_wndToolBar.GetButton(i, &tb);

        if (tb.fsStyle & TBSTYLE_SEP)
        {
            TBBUTTONINFO tbi = {};
            tbi.cbSize = sizeof(tbi);
            tbi.dwMask = TBIF_SIZE | TBIF_BYINDEX;  // BYINDEX = dùng index
            tbi.cx = (WORD)separatorWidth;
            m_wndToolBar.SetButtonInfo(i, &tbi);    // truyền index i
        }
    }

    m_wndToolBar.AutoSize();
}

//BOOL PagePrinter::InitToolBar() {
//    
//    // Tạo toolbar
//    if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP,
//        AFX_IDW_TOOLBAR))
//    {
//        TRACE0("Failed to create toolbar\n");
//        return FALSE;
//    }
//
//    // Tạo image list
//    m_imageList.Create(16, 16, ILC_COLOR24, 4, 4);
//
//    // Load icon từ resource
//    HICON hIconAdd = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_ADD1));
//    HICON hIconDelete = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_DELETE1));
//    HICON hIconRefresh = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_REFRESH1));
//
//    // Thêm icon vào image list
//    m_imageList.Add(hIconAdd);
//    m_imageList.Add(hIconDelete);
//    m_imageList.Add(hIconRefresh);
//
//    // Gán image list cho toolbar
//    m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imageList);
//
//    // Thêm các nút vào toolbar
//    TBBUTTON buttons[3] = {};
//    buttons[0].iBitmap = 0;  // Index trong image list
//    buttons[0].idCommand = ID_ADD_PRINTER;
//    buttons[0].fsState = TBSTATE_ENABLED;
//    buttons[0].fsStyle = TBSTYLE_BUTTON;
//
//    buttons[1].iBitmap = 1;
//    buttons[1].idCommand = ID_DELETE_PRINTER;
//    buttons[1].fsState = TBSTATE_ENABLED;
//    buttons[1].fsStyle = TBSTYLE_BUTTON;
//
//    buttons[2].iBitmap = 2;
//    buttons[2].idCommand = ID_REFRESH;
//    buttons[2].fsState = TBSTATE_ENABLED;
//    buttons[2].fsStyle = TBSTYLE_BUTTON;
//
//    // 1. Set size TRƯỚC
//    m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(24, 24));
//    m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(36, 36));
//
//    // 2. Thêm button SAU
//    m_wndToolBar.GetToolBarCtrl().AddButtons(3, buttons);
//
//    // 3. Reposition cuối cùng
//    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
//}
//
//
//void PagePrinter::InitControlList() {
//
//    // Control List
//
//    CRect rect;
//    GetClientRect(&rect);
//    rect.DeflateRect(10, 10);
//
//    // Tạo list ctrl bằng code, không cần SubclassDlgItem
//    m_listCtrl.Create(
//        WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
//        rect, this, IDC_LIST_PRINTER
//    );
//
//    // Subclass list control từ resource (IDC_LIST_PRINTER)
//    //m_listCtrl.SubclassDlgItem(IDC_LIST_PRINTER, this);
//    m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
//
//    // Thêm cột
//    m_listCtrl.InsertColumn(COL_NAME, _T("Printer Name"), LVCFMT_LEFT, 200);
//    m_listCtrl.InsertColumn(COL_STATUS, _T("Status"), LVCFMT_LEFT, 120);
//    m_listCtrl.InsertColumn(COL_ACTION, _T("Action"), LVCFMT_LEFT, 150);
//
//    // Thêm dữ liệu mẫu
//    int idx = m_listCtrl.InsertItem(0, _T("HP LaserJet 1020"));
//    m_listCtrl.SetItemText(idx, COL_STATUS, _T("Online"));
//
//    idx = m_listCtrl.InsertItem(1, _T("Canon LBP2900"));
//    m_listCtrl.SetItemText(idx, COL_STATUS, _T("Offline"));
//}
//


// PagePrinter message handlers
