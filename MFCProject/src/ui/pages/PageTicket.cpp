// PageTicket.cpp : implementation file

#include "pch.h"
#include "afxdialogex.h"
#include "PageTicket.h"
#include "Resource.h"

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
END_MESSAGE_MAP()


// PageTicket message handlers
