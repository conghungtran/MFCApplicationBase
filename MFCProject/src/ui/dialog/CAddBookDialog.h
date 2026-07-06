#pragma once
#include "afxdialogex.h"
#include "Book.h"

// (đúng theo Resource View của bạn, đổi tên ID nếu khác)
class CAddBookDialog : public CDialogEx
{
public:
    CAddBookDialog(CWnd* pParent = nullptr);

    // Gọi TRƯỚC DoModal() để pre-fill dữ liệu - dùng cho chức năng Edit.
    // Không gọi hàm này -> dialog ở mode Add bình thường (rỗng, ID = 0).
    void SetBook(const Book& book);

    // Gọi sau khi DoModal() trả về IDOK để lấy dữ liệu đã nhập.
    // book.ID giữ nguyên giá trị đã set qua SetBook() (0 nếu là Add).
    Book GetBook() const;

protected:
    virtual BOOL OnInitDialog() override;
    virtual void DoDataExchange(CDataExchange* pDX) override;
    virtual void OnOK() override;

    DECLARE_MESSAGE_MAP()

private:
    CEdit m_edit_bookname;
    CEdit m_edit_bookprice;
    CEdit m_edit_bookquatity;

    CString cstr_Name;
    double  cstr_Price;
    int     cstr_Quantity;

    long    m_bookId = 0;   // 0 = Add mode, >0 = Edit mode (giữ ID gốc)
};
