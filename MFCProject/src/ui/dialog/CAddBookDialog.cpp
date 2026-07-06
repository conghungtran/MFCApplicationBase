// CAddBookDialog.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CAddBookDialog.h"
#include "Resource.h"
#include <string>

// CAddBookDialog dialog

CAddBookDialog::CAddBookDialog(CWnd* pParent)
    : CDialogEx(IDD_DIALOG_BOOK_FORM, pParent)
    , cstr_Price(0.0)
    , cstr_Quantity(0)
{
}

void CAddBookDialog::SetBook(const Book& book)
{
    // Gọi hàm này TRƯỚC DoModal(). Giá trị được gán vào biến DDX ở đây,
    // rồi CDialogEx::OnInitDialog() sẽ tự gọi UpdateData(FALSE) để đổ
    // các giá trị này lên control - dialog sẽ hiện sẵn dữ liệu (mode Edit).
    m_bookId = book.ID;
    cstr_Name = book.Name;
    cstr_Price = book.Price;
    cstr_Quantity = book.Qty;
}

BOOL CAddBookDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog(); // bên trong tự gọi UpdateData(FALSE) -> hiển thị dữ liệu đã SetBook()

    SetWindowText(m_bookId > 0 ? _T("Edit Book") : _T("Add Book"));

    return TRUE;
}

void CAddBookDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_BOOK_NAME, m_edit_bookname);
    DDX_Control(pDX, IDC_EDIT_BOOK_PRICE, m_edit_bookprice);
    DDX_Control(pDX, IDC_EDIT_BOOK_QUANTITY, m_edit_bookquatity);

    DDX_Text(pDX, IDC_EDIT_BOOK_NAME, cstr_Name);
    DDX_Text(pDX, IDC_EDIT_BOOK_PRICE, cstr_Price);
    DDX_Text(pDX, IDC_EDIT_BOOK_QUANTITY, cstr_Quantity);
}

BEGIN_MESSAGE_MAP(CAddBookDialog, CDialogEx)
END_MESSAGE_MAP()

void CAddBookDialog::OnOK()
{
    // UpdateData(TRUE) kích hoạt DDX_Text: đọc text từ control, parse sang
    // cstr_Price (double) / cstr_Quantity (int). Nếu người dùng gõ sai định dạng
    // số, MFC TỰ hiện MessageBox lỗi và trả về FALSE tại đây.
    if (!UpdateData(TRUE))
        return;

    cstr_Name.Trim();
    if (cstr_Name.IsEmpty())
    {
        AfxMessageBox(_T("Book Name không được để trống."));
        m_edit_bookname.SetFocus();
        return;
    }

    if (cstr_Price <= 0)
    {
        AfxMessageBox(_T("Price phải lớn hơn 0."));
        m_edit_bookprice.SetFocus();
        return;
    }

    if (cstr_Quantity < 0)
    {
        AfxMessageBox(_T("Quantity không được âm."));
        m_edit_bookquatity.SetFocus();
        return;
    }

    CDialogEx::OnOK(); // hợp lệ -> đóng dialog, DoModal() trả về IDOK
}

Book CAddBookDialog::GetBook() const
{
    Book book;
    book.ID = m_bookId;   // 0 nếu Add, giữ ID gốc nếu Edit (đã set qua SetBook())
    book.Name = cstr_Name;
    book.Price = cstr_Price;
    book.Qty = cstr_Quantity;
    return book;
}