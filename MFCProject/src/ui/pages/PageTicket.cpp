// PageTicket.cpp : implementation file

#include "pch.h"
#include "afxdialogex.h"
#include "PageTicket.h"
#include "Resource.h"
#include <iostream>

// PageTicket dialog

IMPLEMENT_DYNAMIC(PageTicket, CDialogEx)

PageTicket::PageTicket(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_TICKET, pParent)
{

}

PageTicket::~PageTicket()
{
}

void PageTicket::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PageTicket, CDialogEx)
    ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_SEARCH, &PageTicket::OnSearch)
END_MESSAGE_MAP()


LRESULT PageTicket::OnSearch(WPARAM wParam, LPARAM lParam)
{
    std::cout << "search Ticket\n";
    CString keyword = (LPCTSTR)lParam;
    // Filter list theo keyword
    // ...
    return 0;
}

BOOL PageTicket::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    m_brBackground.CreateSolidBrush(RGB(255, 255, 255));
    m_search.Create(this, IDC_SEARCH,
        CRect(10, 5, 400,40));
    return TRUE;
}

HBRUSH PageTicket::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    // Kiểm tra nếu là background của dialog hoặc control
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        // Set màu nền trắng
        pDC->SetBkColor(RGB(255, 255, 255));
        return (HBRUSH)m_brBackground.GetSafeHandle();
    }

    return hbr;
}



// PageTicket message handlers
