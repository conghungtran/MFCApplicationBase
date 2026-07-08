
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"
#include "MFCProject.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <iostream>
#include "MFCProjectView.h"

// Đặt trước OnCreate — free function
static HICON GetShellIcon(SHSTOCKICONID id)
{
	SHSTOCKICONINFO sii = {};
	sii.cbSize = sizeof(sii);
	SHGetStockIconInfo(id, SHGSI_ICON | SHGSI_LARGEICON, &sii);
	return sii.hIcon;
}

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_COMMAND(ID_PAGE_BOOK, &CMainFrame::OnPagePrinter)
	//ON_COMMAND(ID_PAGE_TICKET, &CMainFrame::OnPageTicket)
	ON_COMMAND(IDD_PAGE_FUNCTION, &CMainFrame::OnPageFunction)


	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

void CMainFrame::OnFileOpen()
{
	std::cout << "Open files \n";
	CFileDialog dlg(
		TRUE,           // TRUE = Open dialog
		_T("txt"),      // Default extension
		nullptr,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||")
	);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		// Xử lý file path ở đây
		AfxGetApp()->OpenDocumentFile(strPath);
	}
}

void CMainFrame::OnPagePrinter()
{
	//TRACE0("OnPrinterAdd called!\n");  // xem trong Output window của VS
	//AfxMessageBox(_T("Add Printer!"));

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	MFCProjectView* pView = (MFCProjectView*)pFrame->GetActiveView();

	if (pView)
		pView->SwitchPage(&pView->m_pageBook);

}


CMainFrame::CMainFrame() noexcept
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

//void CMainFrame::OnPageTicket() { 
//	//AfxMessageBox(_T("Remove Printer")); 
//
//	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	MFCProjectView* pView = (MFCProjectView*)pFrame->GetActiveView();
//
//	if (pView)
//		pView->SwitchPage(&pView->m_pageTicket);
//}
void CMainFrame::OnPageFunction()
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	MFCProjectView* pView = (MFCProjectView*)pFrame->GetActiveView();

	if (pView)
		pView->SwitchPage(&pView->m_pageFunction);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	ReplaceToolbarWithShellIcons();

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(nullptr, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);

	if (!CreateTaskPane())
	{
		TRACE0("Failed to create tasks pane\n");
		return -1;
	}

	if (!CreateSidePane())
	{
		TRACE0("Failed to create tasks pane\n");
		return -1;
	}

	// -------------------------------------------------------------------------------- //
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == nullptr)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}
void CMainFrame::ReplaceToolbarWithShellIcons()
{
	const int SIZE = 24;

	SHSTOCKICONID shellIDs[] =
	{
		SIID_DOCNOASSOC,  // ID_FILE_NEW
		SIID_FOLDER,      // ID_FILE_OPEN
		SIID_STACK,       // ID_FILE_SAVE
		SIID_AUTOLIST,     // ID_FILE_PRINT
		SIID_INFO,        // ID_APP_ABOUT
		SIID_WORLD,
		SIID_DRIVECD,
		SIID_DRIVENETDISABLED,
		SIID_DRIVENET
	};

	CMFCToolBarImages* pImages = m_wndToolBar.GetImages();
	pImages->Clear();
	pImages->SetImageSize(CSize(SIZE, SIZE));

	for (int i = 0; i < _countof(shellIDs); i++)
	{
		HICON hIcon = GetShellIcon(shellIDs[i]);
		if (hIcon)
		{
			pImages->AddIcon(hIcon, TRUE);
			DestroyIcon(hIcon);
		}
	}

	m_wndToolBar.AdjustLayout();
	m_wndToolBar.RedrawWindow();
}


BOOL CMainFrame::CreateSidePane()
{
	if (!m_wndSidePane.Create(
		_T("Actions"),
		this,
		CRect(0, 0, 220, 400),
		TRUE,
		ID_VIEW_SIDE_PANE,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_GRIPPER | CBRS_FLOAT_MULTI))
	{
		return FALSE;
	}

	m_wndSidePane.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndSidePane, AFX_IDW_DOCKBAR_LEFT);


	return TRUE;
}

BOOL CMainFrame::CreateTaskPane()
{
	// Tạo CMFCTasksPane với ID và style
	if (!m_wndTasksPane.Create(
		_T("Tasks"),
		this,
		CRect(0, 0, 200, 0),
		TRUE,
		ID_VIEW_TASKS_PANE,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_GRIPPER | CBRS_FLOAT_MULTI
		| AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE))  // <-- thêm 2 flag này
	{
		return FALSE;
	}

	TRACE1("ID_BOOK_ADD = %d\n", ID_PAGE_BOOK);
	m_wndTasksPane.SetActiveInGroup(0);

	// Cho phép dock
	m_wndTasksPane.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndTasksPane, AFX_IDW_DOCKBAR_LEFT);
	// Thiết lập kích thước mặc định khi float
	m_wndTasksPane.SetWindowPos(nullptr, 0, 0, 220, 400,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	// ---- Thêm các Task Group ----
	int nGroup = m_wndTasksPane.AddGroup(_T("Printer Actions"), FALSE, FALSE);
	TRACE1("nGroup = %d\n", nGroup);  // phải là 0, 1, 2... không phải -1
	m_wndTasksPane.AddTask(nGroup, _T("MFC File New"), -1, ID_FILE_NEW);

	//CButton* pBtn = new CButton();
	//pBtn->Create(_T("Add Printer"),
	//	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	//	CRect(0, 0, 150, 30),
	//	&m_wndTasksPane,
	//	ID_BOOK_ADD);

	// Group Bot
	int nGroupBot = m_wndTasksPane.AddGroup(_T("Bot Actions"), 1, 0);
	m_wndTasksPane.AddTask(nGroupBot, _T("Logout"),
		0, 0);

	//int nTask = m_wndTasksPane.AddTask(nGroup, _T("Add Printer"), -1, ID_BOOK_ADD);
	//TRACE1("nTask = %d\n", nTask);    // phải >= 0


	//m_wndTasksPane.AddTask(nGroup, _T("Remove Printer"),
	//	0, 0, ID_BOOK_REMOVE);
	//m_wndTasksPane.AddTask(nGroup, _T("Print MFC Page"),
	//	0, 0, ID_BOOK_TEST);

	//int nGroup2 = m_wndTasksPane.AddGroup(_T("Maintenance"));
	//m_wndTasksPane.AddTask(nGroup2, _T("Create Ticket"),
	//	0, 0, ID_TICKET_CREATE);
	//m_wndTasksPane.AddTask(nGroup2, _T("View History"),
	//	0, 0, ID_TICKET_HISTORY);

	return TRUE;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != nullptr)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

