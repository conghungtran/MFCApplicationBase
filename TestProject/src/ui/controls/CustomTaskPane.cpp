#include "pch.h"
#include "CustomTaskPane.h"

#include "Resource.h"

IMPLEMENT_DYNAMIC(CSidePanelPane, CDockablePane)

BEGIN_MESSAGE_MAP(CSidePanelPane, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

CSidePanelPane::CSidePanelPane() {}
CSidePanelPane::~CSidePanelPane() {}

int CSidePanelPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    // Tạo font
    m_font.CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH, _T("Segoe UI"));

    CreateButtons();
    return 0;
}

void CSidePanelPane::CreateButtons()
{
    // --- Group: Printer Actions ---
    m_btnAddPrinter.Create(_T("Add Printer"),
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        CRect(12, 50, 188, 82), this, ID_PRINTER_ADD);
    
    m_btnAddPrinter.SetFont(&m_font);
    SetButtonColors(m_btnAddPrinter);
    m_btnAddPrinter.SetColors(
        RGB(76, 153, 0),   // normal
        RGB(55, 138, 221),   // hover
        RGB(15, 70, 130),   // pressed
        RGB(255, 255, 255)   // text);
    );

    SHSTOCKICONINFO sii = { sizeof(sii) };
    SHGetStockIconInfo(SIID_DOCASSOC, SHGSI_ICON | SHGSI_SMALLICON, &sii);
    m_btnAddPrinter.SetIcon(sii.hIcon);

    m_btnRemovePrinter.Create(_T("Remove Printer"),
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        CRect(12, 94, 188, 126), this, ID_PRINTER_REMOVE);
    m_btnRemovePrinter.SetFont(&m_font);

    SHSTOCKICONINFO sii2 = { sizeof(sii2) };
    SHGetStockIconInfo(SIID_USERS, SHGSI_ICON | SHGSI_SMALLICON, &sii2);
    m_btnRemovePrinter.SetIcon(sii2.hIcon);
    SetButtonColors(m_btnRemovePrinter);

    // --- Group: Maintenance ---
    m_btnCreateTicket.Create(_T("Create Ticket"),
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        CRect(12, 180, 188, 212), this, ID_TICKET_CREATE);
    m_btnCreateTicket.SetFont(&m_font);
    SetButtonColors(m_btnCreateTicket);
}

void CSidePanelPane::SetButtonColors(CCustomButton& btn)
{
    btn.SetColors(
        RGB(24, 95, 165),   // normal
        RGB(55, 138, 221),   // hover
        RGB(15, 70, 130),   // pressed
        RGB(255, 255, 255)   // text
    );
}

void CSidePanelPane::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);

    // Resize button theo chiều rộng pane
    if (m_btnAddPrinter.GetSafeHwnd())
    {
        int w = cx - 24;  // margin 12px mỗi bên
        m_btnAddPrinter.SetWindowPos(nullptr, 12, 50, w, 32, SWP_NOZORDER);
        m_btnRemovePrinter.SetWindowPos(nullptr, 12, 94, w, 32, SWP_NOZORDER);
        m_btnCreateTicket.SetWindowPos(nullptr, 12, 180, w, 32, SWP_NOZORDER);
    }
}

void CSidePanelPane::OnPaint()
{
    CPaintDC dc(this);
    CRect rcClient;
    GetClientRect(&rcClient);

    // Nền trắng
    dc.FillSolidRect(rcClient, ::GetSysColor(COLOR_WINDOW));

    // Vẽ label "Printer Actions"
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(RGB(100, 100, 100));
    CFont* pOld = dc.SelectObject(&m_font);

    CRect rcLabel1(12, 20, rcClient.right - 12, 44);
    dc.DrawText(_T("PRINTER ACTIONS"), rcLabel1, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // Vẽ label "Maintenance"
    CRect rcLabel2(12, 152, rcClient.right - 12, 174);
    dc.DrawText(_T("MAINTENANCE"), rcLabel2, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // Vẽ đường kẻ phân cách
    CPen pen(PS_SOLID, 1, RGB(220, 220, 220));
    CPen* pOldPen = dc.SelectObject(&pen);
    dc.MoveTo(12, 145);
    dc.LineTo(rcClient.right - 12, 145);
    dc.SelectObject(pOldPen);

    dc.SelectObject(pOld);
}