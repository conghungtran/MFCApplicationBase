#include "pch.h"
#include "BookPageToolBar.h"
// BookPageToolBar.cpp
#include "Resource.h"
#include <iostream>
#include "ImportExportService.h"

IMPLEMENT_DYNAMIC(BookPageToolBar, CWnd)

BEGIN_MESSAGE_MAP(BookPageToolBar, CWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CTLCOLOR()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_BN_CLICKED(ID_BTN_ADD, &BookPageToolBar::OnAddClicked)
    ON_BN_CLICKED(ID_BTN_DELETE, &BookPageToolBar::OnDeleteClicked)
    ON_MESSAGE(WM_SEARCH, &BookPageToolBar::OnSearch)

    ON_BN_CLICKED(ID_BTN_IMPORT, &BookPageToolBar::OnImportClicked)
    ON_BN_CLICKED(ID_BTN_EXPORT, &BookPageToolBar::OnExportClicked)
END_MESSAGE_MAP()

BOOL BookPageToolBar::Create(CWnd* pParent, UINT nID, const CRect& rect)
{
    LPCTSTR cls = AfxRegisterWndClass(
        CS_HREDRAW | CS_VREDRAW,
        LoadCursor(nullptr, IDC_ARROW),
        (HBRUSH)(COLOR_BTNFACE + 1)
    );
    return CWnd::Create(cls, nullptr,
        WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
        rect, pParent, nID);
}

void BookPageToolBar::OnAddClicked()
{
    // Toolbar không tự mở dialog - chỉ báo lên Parent
    // (Parent mới là nơi có CListCtrl + BookService để xử lý)
    
    if (GetParent())
        GetParent()->PostMessage(WM_ADD_BOOK, 0, 0);
}

void BookPageToolBar::OnDeleteClicked()
{
    if (GetParent())
        GetParent()->PostMessage(WM_DELETE_BOOK, 0, 0);
}

int BookPageToolBar::OnCreate(LPCREATESTRUCT lpcs)
{
    if (CWnd::OnCreate(lpcs) == -1) return -1;

    CreateButtons();

    m_searchCtrl.Create(this, ID_SEARCH_CTRL, CRect(0, 0, 260, 36));

    return 0;
}

void BookPageToolBar::CreateButtons()
{
    auto MakeBtn = [&](CCustomButton& btn, UINT nID, SHSTOCKICONID iconID, LPCTSTR label)
        {
            btn.Create(label,
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
                CRect(0, 0, 0, 0), this, nID);

            btn.SetColors(
                RGB(255, 255, 255),  // Normal  ← trắng, khớp nền toolbar
                RGB(220, 235, 255),  // Hover
                RGB(190, 215, 255),  // Pressed
                RGB(50, 50, 50)    // Text
            );

            SHSTOCKICONINFO sii = { sizeof(sii) };
            SHGetStockIconInfo(iconID, SHGSI_ICON | SHGSI_LARGEICON, &sii); // ← LARGEICON cho icon to hơn
            btn.SetIcon(sii.hIcon);
        };

    MakeBtn(m_btnAdd, ID_BTN_ADD, SIID_STACK, L"Add");
    MakeBtn(m_btnDelete, ID_BTN_DELETE, SIID_DELETE, L"Clear");

    MakeBtn(m_btnImport, ID_BTN_IMPORT, SIID_FOLDER, L"Import");
    MakeBtn(m_btnExport, ID_BTN_EXPORT, SIID_WORLD, L"Export");
}

void BookPageToolBar::RepositionControls()
{
    if (!m_searchCtrl.GetSafeHwnd()) return;

    CRect rc;
    GetClientRect(&rc);

    const int PAD = 8;
    const int BTN_W = 96;   // ← chữ nhật rộng
    const int BTN_H = 40;   // ← cao hơn để chứa icon + text
    const int GAP = 6;
    const int SEARCH_H = 36;
    const int SEARCH_MIN_W = 200;

    int y = (rc.Height() - BTN_H) / 2;
    int searchY = (rc.Height() - SEARCH_H) / 2;

    // LEFT: các button
    int xLeft = PAD;
    m_btnAdd.MoveWindow(xLeft, y, BTN_W, BTN_H);     xLeft += BTN_W + GAP;
    m_btnDelete.MoveWindow(xLeft, y, BTN_W, BTN_H);  xLeft += BTN_W + GAP;
    m_btnImport.MoveWindow(xLeft, y, BTN_W, BTN_H);  xLeft += BTN_W + GAP;
    m_btnExport.MoveWindow(xLeft, y, BTN_W, BTN_H);  xLeft += BTN_W + GAP;

    // CENTER/RIGHT: Search chiếm phần còn lại
    int searchW = rc.right - PAD - xLeft;
    if (searchW < SEARCH_MIN_W) searchW = SEARCH_MIN_W;

    m_searchCtrl.MoveWindow(xLeft, searchY, searchW, SEARCH_H);
}

void BookPageToolBar::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    if (m_searchCtrl.GetSafeHwnd())
        RepositionControls();
}

void BookPageToolBar::OnPaint()
{
    CPaintDC dc(this);
    CRect rc;
    GetClientRect(&rc);

    dc.FillSolidRect(rc, RGB(255, 255, 255));

    //CPen pen(PS_SOLID, 1, RGB(210, 210, 210));
    //CPen* pOld = dc.SelectObject(&pen);
    //dc.MoveTo(rc.left, rc.bottom - 1);
    //dc.LineTo(rc.right, rc.bottom - 1);
    //dc.SelectObject(pOld);
}

BOOL BookPageToolBar::OnEraseBkgnd(CDC* pDC)
{

    return TRUE;
}

LRESULT BookPageToolBar::OnSearch(WPARAM wParam, LPARAM lParam)
{
    if (GetParent())
        GetParent()->PostMessage(WM_SEARCH, wParam, lParam);
    return 0;
}

void BookPageToolBar::OnImportClicked()
{

    if (!m_importExportService)
    {
        AfxMessageBox(_T("ImportExportService is not Initialize."));
        return;
    }

    // Bước 1: mở hộp thoại chọn file CSV
    CFileDialog dlg(
        TRUE,                                     // TRUE = Open dialog
        _T("csv"),
        nullptr,
        OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
        _T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||"),
        this
    );

    if (dlg.DoModal() != IDOK)
        return;   // người dùng bấm Cancel -> không làm gì cả

    CString filePath = dlg.GetPathName();

    // Bước 2: người dùng đã bấm OK trên hộp thoại -> import ngay
    BeginWaitCursor();   // file lớn có thể mất vài giây, đổi con trỏ chờ
    //ImportResult result = m_importExportService->ImportFromCSV(filePath);
    ImportResult result = m_importExportService->ImportFromCSV(filePath);

    EndWaitCursor();

    if (!result.errorMsg.IsEmpty())
    {
        // Có lỗi nghiêm trọng -> ImportExportService đã Rollback toàn bộ
        AfxMessageBox(result.errorMsg);
        return;
    }

    CString msg;
    msg.Format(_T("Import completed successfully.\n\nTotal rows: %d\nSuccessful: %d\nFailed (skipped): %d"),
        result.totalRows, result.successRows, result.failedRows);
    AfxMessageBox(msg);


    if (GetParent())
        GetParent()->PostMessage(WM_IMPORT_BOOK, 0, 0);
}


void BookPageToolBar::OnExportClicked()
{

    if (!m_importExportService)
    {
        AfxMessageBox(_T("ImportExportService is not Initialize."));
        return;
    }

    CFileDialog dlg(
        FALSE,                                   // FALSE = Save dialog
        _T("csv"),
        _T("books.csv"),                          // tên file mặc định theo đúng đề bài
        OFN_OVERWRITEPROMPT,
        _T("CSV Files (*.csv)|*.csv||"),
        this
    );

    if (dlg.DoModal() != IDOK)
        return;

    CString errorMsg;
    if (m_importExportService->ExportToCSV(dlg.GetPathName(), errorMsg))
    {
        AfxMessageBox(_T("Export completed successfully."));
    }
    else
    {
        AfxMessageBox(errorMsg);
    }

    if (GetParent())
        GetParent()->PostMessage(WM_EXPORT_BOOK, 0, 0);
}