
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

// MFCProjectView

IMPLEMENT_DYNCREATE(MFCProjectView, CFormView)

BEGIN_MESSAGE_MAP(MFCProjectView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &MFCProjectView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON1, &MFCProjectView::OnBnClickedButton1)
END_MESSAGE_MAP()

// MFCProjectView construction/destruction

MFCProjectView::MFCProjectView() noexcept
	: CFormView(IDD_TESTPROJECT_FORM)
{
	// TODO: add construction code here

}

MFCProjectView::~MFCProjectView()
{
}

void MFCProjectView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnAddPrinter);
	DDX_Control(pDX, IDC_EDIT1, m_editModel);
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

	m_btnAddPrinter.SetColors(
		RGB(24, 95, 165),   // normal
		RGB(55, 138, 221),   // hover
		RGB(15, 70, 130),   // pressed
		RGB(255, 255, 255)   // text
	);

	m_editModel.SetPlaceholder(_T("Enter Model"));
	m_editModel.SetBorderColor(RGB(0, 120, 215));

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
	AfxMessageBox(_T("View Printer!"));
}
