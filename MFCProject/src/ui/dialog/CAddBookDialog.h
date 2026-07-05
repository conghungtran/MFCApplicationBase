#pragma once
#include "afxdialogex.h"
#include "Book.h"


// CAddBookDialog dialog

class CAddBookDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAddBookDialog)

public:
	CAddBookDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAddBookDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BOOK_FORM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK() override;

	DECLARE_MESSAGE_MAP()

public:
	Book GetBook() const;

public:
	CEdit m_edit_bookname;
	CEdit m_edit_bookprice;
	CEdit m_edit_bookquatity;

public:
	CString cstr_Name;
	double cstr_Price;
	int cstr_Quantity;

};
