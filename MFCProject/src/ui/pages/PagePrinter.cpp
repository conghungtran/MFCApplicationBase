// PagePrinter.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "PagePrinter.h"
#include "Resource.h"
#include <iostream>

// PagePrinter dialog

IMPLEMENT_DYNAMIC(PagePrinter, CDialogEx)

PagePrinter::PagePrinter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_PRINTER, pParent)
{

}

PagePrinter::~PagePrinter()
{
}

//void PagePrinter::OnSize(UINT nType, int cx, int cy)
//{
//    CDialog::OnSize(nType, cx, cy);
//
//    if (m_listCtrl.GetSafeHwnd())
//    {
//        m_listCtrl.MoveWindow(0, 0, cx, cy);
//    }
//
//    if (m_wndToolBar.m_hWnd != NULL && m_listCtrl.m_hWnd != NULL)
//    {
//        // Đặt toolbar ở trên cùng, chiếm toàn bộ chiều rộng
//        CRect toolbarRect;
//        m_wndToolBar.GetWindowRect(&toolbarRect);
//        ScreenToClient(&toolbarRect);
//
//        // Chiều cao toolbar không đổi, chỉ thay đổi chiều rộng
//        int toolbarHeight = toolbarRect.Height();
//        m_wndToolBar.SetWindowPos(NULL, 0, 0, cx, toolbarHeight, SWP_NOZORDER);
//
//        // Đặt list control bên dưới toolbar
//        CRect rect;
//        rect.left = 10;
//        rect.right = cx - 10;
//        rect.top = toolbarHeight + 5;
//        rect.bottom = cy - 10;
//
//        m_listCtrl.SetWindowPos(NULL, rect.left, rect.top,
//            rect.Width(), rect.Height(), SWP_NOZORDER);
//    }
//}


void PagePrinter::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (!m_wndToolBar.GetSafeHwnd() || !m_listCtrl.GetSafeHwnd())
        return;

    // Reposition toolbar trước
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

    // Lấy chiều cao toolbar sau reposition
    CRect toolbarRect;
    m_wndToolBar.GetWindowRect(&toolbarRect);
    ScreenToClient(&toolbarRect);

    int top = toolbarRect.bottom + 5;

    m_listCtrl.SetWindowPos(
        NULL,
        10, top,
        cx - 20, cy - top - 10,
        SWP_NOZORDER
    );
}

void PagePrinter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PagePrinter, CDialogEx)
	ON_MESSAGE(WM_EDIT_ITEM, &PagePrinter::OnEditItem)
	ON_MESSAGE(WM_DELETE_ITEM, &PagePrinter::OnDeleteItem)
    ON_WM_SIZE()
END_MESSAGE_MAP()



// OnInit ToolBar + ControlList
 
BOOL PagePrinter::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ============================================================
    // 1. TẠO TOOLBAR
    // ============================================================
    if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP,
        AFX_IDW_TOOLBAR))
    {
        TRACE0("Failed to create toolbar\n");
        return FALSE;
    }

    // Tạo image list
    m_imageList.Create(32, 32, ILC_COLOR32 | ILC_MASK, 4, 4);


    // Load icon từ resource
    HICON hIconAdd = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_ADD1));
    HICON hIconDelete = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_DELETE1));
    HICON hIconRefresh = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_REFRESH1));

    // Thêm icon vào image list
    if (hIconAdd) m_imageList.Add(hIconAdd);
    if (hIconDelete) m_imageList.Add(hIconDelete);
    if (hIconRefresh) m_imageList.Add(hIconRefresh);

    // Gán image list cho toolbar
    m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imageList);

    // Thêm các nút vào toolbar
    TBBUTTON buttons[3] = {};
    buttons[0].iBitmap = 0;  // Index trong image list
    buttons[0].idCommand = ID_ADD_PRINTER;
    buttons[0].fsState = TBSTATE_ENABLED;
    buttons[0].fsStyle = TBSTYLE_BUTTON;

    buttons[1].iBitmap = 1;
    buttons[1].idCommand = ID_DELETE_PRINTER;
    buttons[1].fsState = TBSTATE_ENABLED;
    buttons[1].fsStyle = TBSTYLE_BUTTON;

    buttons[2].iBitmap = 2;
    buttons[2].idCommand = ID_REFRESH;
    buttons[2].fsState = TBSTATE_ENABLED;
    buttons[2].fsStyle = TBSTYLE_BUTTON;


    // Gán image list
    m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imageList);

    // 3. BitmapSize = 32, ButtonSize = 32 + 12
    m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(32, 32));
    m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(44, 44));

    m_wndToolBar.GetToolBarCtrl().AddButtons(3, buttons);

    // ============================================================
    // 2. TẠO LIST CONTROL (nằm bên dưới toolbar)
    // ============================================================

    // Lấy kích thước của vùng client (đã trừ đi toolbar)
    CRect rect;
    GetClientRect(&rect);

    // 👇 QUAN TRỌNG: Lấy vị trí của toolbar để tính toán offset
    CRect toolbarRect;
    m_wndToolBar.GetWindowRect(&toolbarRect);
    ScreenToClient(&toolbarRect);

    // Điều chỉnh vùng rect cho list control
    // - Top: bắt đầu từ dưới toolbar (toolbarRect.bottom)
    // - Bottom: sát đáy dialog
    rect.top = toolbarRect.bottom + 5;  // +5 là khoảng cách padding
    rect.left += 10;
    rect.right -= 10;
    rect.bottom -= 10;

    // Tạo list control
    m_listCtrl.Create(
        WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
        rect, this, IDC_LIST_PRINTER
    );

    // Thiết lập style cho list control
    m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // Thêm cột
    m_listCtrl.InsertColumn(COL_NAME, _T("Printer Name"), LVCFMT_LEFT, 200);
    m_listCtrl.InsertColumn(COL_STATUS, _T("Status"), LVCFMT_LEFT, 120);
    m_listCtrl.InsertColumn(COL_ACTION, _T("Action"), LVCFMT_LEFT, 150);

    // Thêm dữ liệu mẫu
    int idx = m_listCtrl.InsertItem(0, _T("HP LaserJet 1020"));
    m_listCtrl.SetItemText(idx, COL_STATUS, _T("Online"));

    idx = m_listCtrl.InsertItem(1, _T("Canon LBP2900"));
    m_listCtrl.SetItemText(idx, COL_STATUS, _T("Offline"));

    return TRUE;
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

    if (AfxMessageBox(_T("Xóa ") + strName + _T("?"),
        MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        m_listCtrl.DeleteItem(nRow);
    }
    return 0;
}

BOOL PagePrinter::InitToolBar() {
    
    // Tạo toolbar
    if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP,
        AFX_IDW_TOOLBAR))
    {
        TRACE0("Failed to create toolbar\n");
        return FALSE;
    }

    // Tạo image list
    m_imageList.Create(16, 16, ILC_COLOR24, 4, 4);

    // Load icon từ resource
    HICON hIconAdd = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_ADD1));
    HICON hIconDelete = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_DELETE1));
    HICON hIconRefresh = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_REFRESH1));

    // Thêm icon vào image list
    m_imageList.Add(hIconAdd);
    m_imageList.Add(hIconDelete);
    m_imageList.Add(hIconRefresh);

    // Gán image list cho toolbar
    m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imageList);

    // Thêm các nút vào toolbar
    TBBUTTON buttons[3] = {};
    buttons[0].iBitmap = 0;  // Index trong image list
    buttons[0].idCommand = ID_ADD_PRINTER;
    buttons[0].fsState = TBSTATE_ENABLED;
    buttons[0].fsStyle = TBSTYLE_BUTTON;

    buttons[1].iBitmap = 1;
    buttons[1].idCommand = ID_DELETE_PRINTER;
    buttons[1].fsState = TBSTATE_ENABLED;
    buttons[1].fsStyle = TBSTYLE_BUTTON;

    buttons[2].iBitmap = 2;
    buttons[2].idCommand = ID_REFRESH;
    buttons[2].fsState = TBSTATE_ENABLED;
    buttons[2].fsStyle = TBSTYLE_BUTTON;

    // 1. Set size TRƯỚC
    m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(24, 24));
    m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(36, 36));

    // 2. Thêm button SAU
    m_wndToolBar.GetToolBarCtrl().AddButtons(3, buttons);

    // 3. Reposition cuối cùng
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}


void PagePrinter::InitControlList() {

    // Control List

    CRect rect;
    GetClientRect(&rect);
    rect.DeflateRect(10, 10);

    // Tạo list ctrl bằng code, không cần SubclassDlgItem
    m_listCtrl.Create(
        WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
        rect, this, IDC_LIST_PRINTER
    );

    // Subclass list control từ resource (IDC_LIST_PRINTER)
    //m_listCtrl.SubclassDlgItem(IDC_LIST_PRINTER, this);
    m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // Thêm cột
    m_listCtrl.InsertColumn(COL_NAME, _T("Printer Name"), LVCFMT_LEFT, 200);
    m_listCtrl.InsertColumn(COL_STATUS, _T("Status"), LVCFMT_LEFT, 120);
    m_listCtrl.InsertColumn(COL_ACTION, _T("Action"), LVCFMT_LEFT, 150);

    // Thêm dữ liệu mẫu
    int idx = m_listCtrl.InsertItem(0, _T("HP LaserJet 1020"));
    m_listCtrl.SetItemText(idx, COL_STATUS, _T("Online"));

    idx = m_listCtrl.InsertItem(1, _T("Canon LBP2900"));
    m_listCtrl.SetItemText(idx, COL_STATUS, _T("Offline"));
}



// PagePrinter message handlers
