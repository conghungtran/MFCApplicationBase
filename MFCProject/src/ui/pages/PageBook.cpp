#include "pch.h"
#include "afxdialogex.h"
#include "PageBook.h"
#include "Resource.h"
#include <iostream>
#include <commoncontrols.h>
#include "CAddBookDialog.h"
#include <algorithm>
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
    // Hiện tại data đã load hết trong InitTable() nên chỉ update pagination UI
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

    ON_MESSAGE(WM_ADD_BOOK, &PageBook::OnAddBook)
    ON_MESSAGE(WM_DELETE_BOOK, &PageBook::OnBnClickedBtnClear)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_BOOK, &PageBook::OnLvnColumnClick)

    ON_WM_SIZE()
    ON_WM_SETCURSOR()
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void PageBook::OnLvnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    int nClickedCol = pNMLV->iSubItem;

    // BookService::GetSortedBooks() chỉ whitelist 2 cột NAME/PRICE (chống SQL injection
    // qua tên cột) - các cột khác (ID, Qty, Created Date, Action) không hỗ trợ sort qua click
    CString sortColumn;
    switch (nClickedCol)
    {
    case COL_NAME:  sortColumn = _T("NAME");  break;
    case COL_PRICE: sortColumn = _T("PRICE"); break;
    default:
        *pResult = 0;
        return;
    }

    // Click lại đúng cột đang sort -> đảo chiều ASC/DESC
    // Click sang cột khác -> reset về ASC
    if (m_nSortColumn == nClickedCol)
        m_bSortAscending = !m_bSortAscending;
    else
    {
        m_nSortColumn = nClickedCol;
        m_bSortAscending = true;
    }

    std::vector<Book> books = m_bookService->GetSortedBooks(sortColumn, m_bSortAscending);

    m_listCtrl.DeleteAllItems();
    for (const auto& book : books)
    {
        AddBookToListCtrl(book);   // hàm insert 1 row bạn đã có sẵn từ LoadData()
    }

    UpdateSortArrow(nClickedCol, m_bSortAscending);   // optional - xem bước 4

    *pResult = 0;
}

void PageBook::UpdateSortArrow(int nSortedCol, bool bAscending)
{
    CHeaderCtrl* pHeader = m_listCtrl.GetHeaderCtrl();
    if (!pHeader)
        return;

    int nCount = pHeader->GetItemCount();
    for (int i = 0; i < nCount; ++i)
    {
        HDITEM hdItem = {};
        hdItem.mask = HDI_FORMAT;
        pHeader->GetItem(i, &hdItem);

        // Xóa cờ mũi tên cũ (nếu có) ở tất cả cột
        hdItem.fmt &= ~(HDF_SORTUP | HDF_SORTDOWN);

        // Chỉ gắn mũi tên cho đúng cột đang được sort
        if (i == nSortedCol)
            hdItem.fmt |= bAscending ? HDF_SORTUP : HDF_SORTDOWN;

        pHeader->SetItem(i, &hdItem);
    }
}

LRESULT PageBook::OnBnClickedBtnClear(WPARAM, LPARAM)
{


    if (MessageBox(_T("Are you sure you want to delete ALL book data?\nThis action cannot be undone."),
        _T("Confirm deletion"), MB_YESNO | MB_ICONWARNING) != IDYES)
    {
        return 0;
    }

    CString errorMsg;
    if (m_bookService->DeleteAllBook(errorMsg))
    {
        // Chỉ xóa khỏi CListCtrl SAU KHI Database xóa thành công
        m_listCtrl.DeleteAllItems();
    }
    else
    {
        AfxMessageBox(errorMsg);
    }

    return 0;
}



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

LRESULT PageBook::OnAddBook(WPARAM, LPARAM)
{

    CAddBookDialog dlg(this);
    if (dlg.DoModal() == IDOK)
    {
        Book book = dlg.GetBook();
        CString errorMsg;
        // Gọi qua Service (không gọi thẳng Repository) - đúng kiến trúc UI -> Service -> Repository
        if (m_bookService->AddBook(book, errorMsg))
        {
            AddBookToListCtrl(book);   // book.ID và book.CreatedDate đã có giá trị thật từ DB
        }
        else
        {
            AfxMessageBox(errorMsg);
        }
    }
    return 0;
}

void PageBook::AddBookToListCtrl(const Book& book)
{
    CString idStr;
    idStr.Format(_T("%ld"), book.ID);

    int nIndex = m_listCtrl.InsertItem(m_listCtrl.GetItemCount(), idStr);
    m_listCtrl.SetItemText(nIndex, 1, book.Name);

    CString priceStr;
    priceStr.Format(_T("%.2f"), book.Price);
    m_listCtrl.SetItemText(nIndex, 2, priceStr);

    CString qtyStr;
    qtyStr.Format(_T("%d"), book.Qty);
    m_listCtrl.SetItemText(nIndex, 3, qtyStr);

    m_listCtrl.SetItemText(nIndex, 4, book.CreatedDate.Format(_T("%Y-%m-%d %H:%M:%S")));
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

    InitTable();
    LoadData();
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

void PageBook::InitTable()
{
    m_listCtrl.InsertColumn(COL_ID, _T("ID"), LVCFMT_CENTER, 100);
    m_listCtrl.InsertColumn(COL_NAME, _T("Name"), LVCFMT_LEFT, 150);
    m_listCtrl.InsertColumn(COL_PRICE, _T("Price"), LVCFMT_CENTER, 150);
    m_listCtrl.InsertColumn(COL_QUANTITY, _T("Quantity"), LVCFMT_CENTER, 150);
    m_listCtrl.InsertColumn(COL_CREATED_DATE, _T("Created Date"), LVCFMT_CENTER, 150);
    m_listCtrl.InsertColumn(COL_ACTION, _T("Action"), LVCFMT_LEFT, 200);

}

void PageBook::LoadData()
{
    m_listCtrl.DeleteAllItems();

    if (!m_bookService)
    {
        AfxMessageBox(_T("BookService is not initialize."));
        return;
    }

    std::vector<Book> books = m_bookService->GetAllBooks();

    for (size_t i = 0; i < books.size(); ++i)
    {
        const Book& book = books[i];

        CString idStr;
        idStr.Format(_T("%ld"), book.ID);
        int idx = m_listCtrl.InsertItem((int)i, idStr);   // cột ID (chỉ số 0) truyền lúc InsertItem

        m_listCtrl.SetItemText(idx, COL_NAME, book.Name);

        CString priceStr;
        priceStr.Format(_T("%.2f"), book.Price);
        m_listCtrl.SetItemText(idx, COL_PRICE, priceStr);

        CString qtyStr;
        qtyStr.Format(_T("%d"), book.Qty);
        m_listCtrl.SetItemText(idx, COL_QUANTITY, qtyStr);

        m_listCtrl.SetItemText(idx, COL_CREATED_DATE, book.CreatedDate.Format(_T("%Y-%m-%d %H:%M:%S")));

        // COL_ACTION: để trống ở đây - nếu bạn có nút Edit/Delete riêng trong từng dòng,
        // thường xử lý bằng owner-draw hoặc custom control chèn vào, không set text thường
    }

    // Cập nhật total sau khi load xong - dùng cho CPaginationBar bạn đã có
    m_nTotalRecords = m_listCtrl.GetItemCount();
}

LRESULT PageBook::OnEditItem(WPARAM wParam, LPARAM lParam)
{
    int nRow = (int)wParam;

    // Lấy dữ liệu hiện tại của dòng này để pre-fill lên dialog
    Book book;
    book.ID = _ttol(m_listCtrl.GetItemText(nRow, COL_ID));
    book.Name = m_listCtrl.GetItemText(nRow, COL_NAME);
    book.Price = _ttof(m_listCtrl.GetItemText(nRow, COL_PRICE));
    book.Qty = _ttoi(m_listCtrl.GetItemText(nRow, COL_QUANTITY));

    CAddBookDialog dlg(this);
    dlg.SetBook(book);   // mode Edit - dialog hiện sẵn dữ liệu, tiêu đề đổi thành "Edit Book"

    if (dlg.DoModal() != IDOK)
        return 0;

    Book updated = dlg.GetBook();   // updated.ID đã tự giữ đúng book.ID nhờ SetBook()

    CString errorMsg;
    if (m_bookService->UpdateBook(updated, errorMsg))
    {
        // Cập nhật lại đúng dòng trên CListCtrl, không cần LoadData() lại toàn bộ
        m_listCtrl.SetItemText(nRow, COL_NAME, updated.Name);

        CString priceStr;
        priceStr.Format(_T("%.2f"), updated.Price);
        m_listCtrl.SetItemText(nRow, COL_PRICE, priceStr);

        CString qtyStr;
        qtyStr.Format(_T("%d"), updated.Qty);
        m_listCtrl.SetItemText(nRow, COL_QUANTITY, qtyStr);
    }
    else
    {
        AfxMessageBox(errorMsg);
    }

    return 0;
}

LRESULT PageBook::OnDeleteItem(WPARAM wParam, LPARAM lParam)
{
    int nRow = (int)wParam;
    CString strName = m_listCtrl.GetItemText(nRow, COL_NAME);
    CString strId = m_listCtrl.GetItemText(nRow, COL_ID);
    long id = _ttol(strId);   // convert CString -> long

    if (AfxMessageBox(_T("Delete ") + strName + _T("?"),
        MB_YESNO | MB_ICONQUESTION) != IDYES)
    {
        return 0;
    }

    CString errorMsg;
    if (m_bookService->DeleteBook(id, errorMsg))
    {
        // Chỉ xóa khỏi CListCtrl SAU KHI Database xóa thành công
        m_listCtrl.DeleteItem(nRow);
    }
    else
    {
        AfxMessageBox(errorMsg);
    }

    return 0;
}


// PageBook message handlers
