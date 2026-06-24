// PagePrinter.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "PagePrinter.h"
#include "Resource.h"

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

    if (m_listCtrl.GetSafeHwnd())
    {
        m_listCtrl.MoveWindow(0, 0, cx, cy);
    }
}

void PagePrinter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST_PRINTER, m_listCtrl);
}


BEGIN_MESSAGE_MAP(PagePrinter, CDialogEx)
	ON_MESSAGE(WM_EDIT_ITEM, &PagePrinter::OnEditItem)
	ON_MESSAGE(WM_DELETE_ITEM, &PagePrinter::OnDeleteItem)
    ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL PagePrinter::OnInitDialog()
{
    CDialogEx::OnInitDialog();
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

    return TRUE;
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

    if (AfxMessageBox(_T("Xóa ") + strName + _T("?"),
        MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
        m_listCtrl.DeleteItem(nRow);
    }
    return 0;
}

// PagePrinter message handlers
