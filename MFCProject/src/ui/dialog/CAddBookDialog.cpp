// CAddBookDialog.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CAddBookDialog.h"
#include "Resource.h"
#include <string>

// CAddBookDialog dialog

IMPLEMENT_DYNAMIC(CAddBookDialog, CDialogEx)

CAddBookDialog::CAddBookDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BOOK_FORM, pParent)
{

}

CAddBookDialog::~CAddBookDialog()
{
}

Book CAddBookDialog::GetBook() const
{
	Book book;
	book.Name = cstr_Name;
	book.Price = cstr_Price;
	book.Qty = cstr_Quantity;
	return book;
}


void CAddBookDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BOOK_NAME, m_edit_bookname);
	DDX_Control(pDX, IDC_EDIT_BOOK_PRICE, m_edit_bookprice);
	DDX_Control(pDX, IDC_EDIT_BOOK_QUANTITY, m_edit_bookquatity);

	// Automatically map
	DDX_Text(pDX, IDC_EDIT_BOOK_NAME, cstr_Name);
	DDX_Text(pDX, IDC_EDIT_BOOK_PRICE, cstr_Price);
	DDX_Text(pDX, IDC_EDIT_BOOK_QUANTITY, cstr_Quantity);
}


void CAddBookDialog::OnOK()
{
    // UpdateData(TRUE) kích hoạt DDX_Text: đọc text từ control, parse sang
    // cstr_Price (double) / cstr_Quantity (int). Nếu người dùng gõ sai định dạng
    // số (vd chữ cái, hoặc dấu phẩy thay vì dấu chấm), MFC TỰ hiện MessageBox lỗi
    // và trả về FALSE tại đây - không cần tự validate kiểu số nữa.
    if (!UpdateData(TRUE))
        return;

    cstr_Name.Trim();
    if (cstr_Name.IsEmpty())
    {
        AfxMessageBox(_T("Book Name is empty"));
        m_edit_bookname.SetFocus();
        return;
    }

    if (cstr_Price <= 0)
    {
        AfxMessageBox(_T("Price must be > 0"));
        m_edit_bookprice.SetFocus();
        return;
    }

    if (cstr_Quantity < 0)
    {
        AfxMessageBox(_T("Quantity must be > 0"));
        m_edit_bookquatity.SetFocus();
        return;
    }

    CDialogEx::OnOK(); // hợp lệ -> đóng dialog, DoModal() trả về IDOK
}

BEGIN_MESSAGE_MAP(CAddBookDialog, CDialogEx)
END_MESSAGE_MAP()


// CAddBookDialog message handlers
