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
    m_btnPagePrinter.Create(_T("Book"),
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        CRect(12, 50, 188, 82), this, ID_PAGE_BOOK);
    
    m_btnPagePrinter.SetFont(&m_font);
    SetButtonColors(m_btnPagePrinter);
    m_btnPagePrinter.SetColors(
        RGB(76, 153, 0),   // normal
        RGB(55, 138, 221),   // hover
        RGB(15, 70, 130),   // pressed
        RGB(255, 255, 255)   // text);
    );

    SHSTOCKICONINFO sii = { sizeof(sii) };
    SHGetStockIconInfo(SIID_FOLDER, SHGSI_ICON | SHGSI_SMALLICON, &sii);
    m_btnPagePrinter.SetIcon(sii.hIcon);

    m_btnPageTicket.Create(_T("Ticket"),
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        CRect(12, 94, 188, 126), this, ID_PAGE_TICKET);
    m_btnPageTicket.SetFont(&m_font);

    SHSTOCKICONINFO sii_ticket = { sizeof(sii_ticket) };
    SHGetStockIconInfo(SIID_USERS, SHGSI_ICON | SHGSI_SMALLICON, &sii_ticket);
    m_btnPageTicket.SetIcon(sii_ticket.hIcon);
    SetButtonColors(m_btnPageTicket);

    // --- Group: Maintenance ---
    m_btnPageFunction.Create(_T("Function"),
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        CRect(12, 180, 188, 212), this, IDD_PAGE_FUNCTION);
    m_btnPageFunction.SetFont(&m_font);
    SetButtonColors(m_btnPageFunction);

    SHSTOCKICONINFO sii_function = { sizeof(sii_function) };
    SHGetStockIconInfo(SIID_WORLD, SHGSI_ICON | SHGSI_SMALLICON, &sii_function);
    m_btnPageFunction.SetIcon(sii_function.hIcon);

    // --- Bottom button: Settings ---
    // Tạo ở vị trí tạm, OnSize sẽ đặt đúng vị trí
    m_btnSettings.Create(_T("SETTING"),
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        CRect(12, 0, 188, 32), this, ID_SETTINGS);
    m_btnSettings.SetFont(&m_font);
    SetButtonColors(m_btnSettings);

    SHSTOCKICONINFO sii_setting  = { sizeof(sii_setting) };
    SHGetStockIconInfo(SIID_DRIVEBD, SHGSI_ICON | SHGSI_SMALLICON, &sii_setting);
    m_btnSettings.SetIcon(sii_setting.hIcon);

    //--- Bottom button: Logout ---
    //Tạo ở vị trí tạm, OnSize sẽ đặt đúng vị trí
    m_btnLogout.Create(_T("LOGOUT"),
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        CRect(12, 0, 188, 32), this, ID_LOGOUT);
    m_btnLogout.SetFont(&m_font);
    SetButtonColors(m_btnLogout);

    SHSTOCKICONINFO sii_logout = { sizeof(sii_logout) };
    SHGetStockIconInfo(SIID_MEDIABDROM, SHGSI_ICON | SHGSI_SMALLICON, &sii_logout);
    m_btnLogout.SetIcon(sii_logout.hIcon);
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
    if (m_btnPagePrinter.GetSafeHwnd())
    {
        int w = cx - 24;  // margin 12px mỗi bên
        m_btnPagePrinter.SetWindowPos(nullptr, 12, 50, w, 32, SWP_NOZORDER);
        m_btnPageTicket.SetWindowPos(nullptr, 12, 94, w, 32, SWP_NOZORDER);
        m_btnPageFunction.SetWindowPos(nullptr, 12, 180, w, 32, SWP_NOZORDER);
    }

    const int btnHeight = 32;
    const int margin = 12;
    const int gap = 8; // khoảng cách giữa 2 button

    // Logout nằm sát đáy nhất
    if (m_btnLogout.GetSafeHwnd())
        m_btnLogout.SetWindowPos(nullptr,
            margin,
            cy - btnHeight - margin,
            cx - margin * 2,
            btnHeight,
            SWP_NOZORDER);

    // Settings nằm phía trên Logout
    if (m_btnSettings.GetSafeHwnd())
        m_btnSettings.SetWindowPos(nullptr,
            margin,
            cy - btnHeight * 2 - margin - gap,
            cx - margin * 2,
            btnHeight,
            SWP_NOZORDER);
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
    dc.DrawText(_T("BOOK ACTIONS"), rcLabel1, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // Vẽ label "Maintenance"
    CRect rcLabel2(12, 152, rcClient.right - 12, 174);
    dc.DrawText(_T("BOOK FUNCTION"), rcLabel2, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // Vẽ đường kẻ phân cách
    CPen pen(PS_SOLID, 1, RGB(220, 220, 220));
    CPen* pOldPen = dc.SelectObject(&pen);
    dc.MoveTo(12, 145);
    dc.LineTo(rcClient.right - 12, 145);
    dc.SelectObject(pOldPen);

    dc.SelectObject(pOld);
}