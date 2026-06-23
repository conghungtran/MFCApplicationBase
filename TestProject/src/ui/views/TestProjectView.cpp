
// TestProjectView.cpp : implementation of the TestProjectView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TestProject.h"
#endif

#include "TestProjectDoc.h"
#include "TestProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// TestProjectView

IMPLEMENT_DYNCREATE(TestProjectView, CFormView)

BEGIN_MESSAGE_MAP(TestProjectView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &TestProjectView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON1, &TestProjectView::OnBnClickedButton1)
END_MESSAGE_MAP()

// TestProjectView construction/destruction

TestProjectView::TestProjectView() noexcept
	: CFormView(IDD_TESTPROJECT_FORM)
{
	// TODO: add construction code here

}

TestProjectView::~TestProjectView()
{
}

void TestProjectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnAddPrinter);
	DDX_Control(pDX, IDC_EDIT1, m_editModel);
}

BOOL TestProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void TestProjectView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_btnAddPrinter.SetColors(
		RGB(24, 95, 165),   // normal
		RGB(55, 138, 221),   // hover
		RGB(15, 70, 130),   // pressed
		RGB(255, 255, 255)   // text
	);

	m_editModel.SetPlaceholder(_T("Enter Model"));
	m_editModel.SetBorderColor(RGB(0, 120, 215));

}


// TestProjectView printing


void TestProjectView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL TestProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void TestProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void TestProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void TestProjectView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

void TestProjectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void TestProjectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// TestProjectView diagnostics

#ifdef _DEBUG
void TestProjectView::AssertValid() const
{
	CFormView::AssertValid();
}

void TestProjectView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTestProjectDoc* TestProjectView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestProjectDoc)));
	return (CTestProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// TestProjectView message handlers

void TestProjectView::OnBnClickedButton1()
{
	TRACE0("View called!\n");  // xem trong Output window của VS
	AfxMessageBox(_T("View Printer!"));
}
