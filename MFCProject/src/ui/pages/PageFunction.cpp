// PageFunction.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "PageFunction.h"
#include "Resource.h"


// PageFunction dialog

IMPLEMENT_DYNAMIC(PageFunction, CDialogEx)

PageFunction::PageFunction(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_FUNCTION, pParent)
{

}

PageFunction::~PageFunction()
{
}

void PageFunction::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PageFunction, CDialogEx)
END_MESSAGE_MAP()


// PageFunction message handlers
