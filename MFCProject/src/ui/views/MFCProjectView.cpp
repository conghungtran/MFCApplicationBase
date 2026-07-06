
// MFCProjectView.cpp : implementation of the MFCProjectView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCProject.h"
#endif

#include "MFCProjectDoc.h"
#include "MFCProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "BookRepository.h"

// MFCProjectView

IMPLEMENT_DYNCREATE(MFCProjectView, CFormView)

BEGIN_MESSAGE_MAP(MFCProjectView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &MFCProjectView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// MFCProjectView construction/destruction

MFCProjectView::MFCProjectView() noexcept
	: CFormView(IDD_MFCPROJECT_FORM)
{
	// TODO: add construction code here

}

MFCProjectView::~MFCProjectView()
{
}

void MFCProjectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BUTTON1, m_btnAddBook);
	//DDX_Control(pDX, IDC_EDIT1, m_editModel);
}

BOOL MFCProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void MFCProjectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//m_btnAddBook.SetColors(
	//	RGB(24, 95, 165),   // normal
	//	RGB(55, 138, 221),   // hover
	//	RGB(15, 70, 130),   // pressed
	//	RGB(255, 255, 255)   // text
	//);

	//m_editModel.SetPlaceholder(_T("Enter Model"));
	//m_editModel.SetBorderColor(RGB(0, 120, 215));

	CRect rect;
	GetClientRect(&rect);
	auto repository = std::make_shared<CBookRepository>();
	auto bookService = std::make_shared<CBookService>(repository);
	m_pageBook.SetBookService(bookService);

	// Tạo các page — parent là View (this)
	m_pageBook.Create(IDD_PAGE_BOOK, this);
	m_pageBook.MoveWindow(rect);

	



	m_pageTicket.Create(IDD_PAGE_TICKET, this);
	m_pageTicket.MoveWindow(rect);

	m_pageFunction.Create(IDD_PAGE_FUNCTION, this);
	m_pageFunction.MoveWindow(rect);


	// Hiện trang mặc định
	m_pCurrentPage = nullptr;
	SwitchPage(&m_pageBook);

}

void MFCProjectView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rect(0, 0, cx, cy);

	if (m_pageBook.GetSafeHwnd())
		m_pageBook.MoveWindow(rect);

	if (m_pageTicket.GetSafeHwnd())
		m_pageTicket.MoveWindow(rect);

	if (m_pageFunction.GetSafeHwnd())
		m_pageFunction.MoveWindow(rect);
}


void MFCProjectView::OnDraw(CDC* pDC)
{
	// Để trống — các page dialog tự vẽ
}

// MFCProjectView printing


void MFCProjectView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL MFCProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}
void MFCProjectView::SwitchPage(CDialog* pNewPage)
{
	if (m_pCurrentPage)
		m_pCurrentPage->ShowWindow(SW_HIDE);

	m_pCurrentPage = pNewPage;
	m_pCurrentPage->ShowWindow(SW_SHOW);
}

void MFCProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void MFCProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void MFCProjectView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

void MFCProjectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void MFCProjectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// MFCProjectView diagnostics

#ifdef _DEBUG
void MFCProjectView::AssertValid() const
{
	CFormView::AssertValid();
}

void MFCProjectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMFCProjectDoc* MFCProjectView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCProjectDoc)));
	return (CMFCProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// MFCProjectView message handlers

void MFCProjectView::OnBnClickedButton1()
{
	TRACE0("View called!\n");  // xem trong Output window của VS
	AfxMessageBox(_T("View Book!"));
}
