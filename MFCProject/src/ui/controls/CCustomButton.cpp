#include "pch.h"
#include "CCustomButton.h"

BEGIN_MESSAGE_MAP(CCustomButton, CButton)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


CCustomButton::CCustomButton() {}
CCustomButton::~CCustomButton() {}

void CCustomButton::SetIcon(HICON hIcon)
{
    m_hIcon = hIcon;

    // Tạo ImageList với mask để hỗ trợ transparent
    if (m_imgList.GetSafeHandle())
        m_imgList.DeleteImageList();

    m_imgList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 0);
    m_imgList.Add(hIcon);

    Invalidate();
}


void CCustomButton::SetColors(COLORREF clrNormal, COLORREF clrHover,
    COLORREF clrPressed, COLORREF clrText)
{
    m_clrNormal = clrNormal;
    m_clrHover = clrHover;
    m_clrPressed = clrPressed;
    m_clrText = clrText;
    Invalidate();
}
void CCustomButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
    CDC* pDC = CDC::FromHandle(lpDIS->hDC);
    CRect rc = lpDIS->rcItem;
    BOOL bEnabled = IsWindowEnabled();

    // Chọn màu theo trạng thái
    COLORREF clrBg;
    if (!bEnabled)       clrBg = RGB(180, 180, 180);
    else if (m_bPressed) clrBg = m_clrPressed;
    else if (m_bHover)   clrBg = m_clrHover;
    else                 clrBg = m_clrNormal;

    DrawBackground(pDC, rc, clrBg);
    DrawBorder(pDC, rc, clrBg);

    // ---- Tính tổng chiều rộng (icon + gap + text) để căn giữa ----
    int iconSize = 16;
    int gap = 6;   // khoảng cách giữa icon và text

    // Đo chiều rộng text
    CString strText;
    GetWindowText(strText);
    CFont* pOldFont = pDC->SelectObject(GetFont());
    CSize  sizeText = pDC->GetTextExtent(strText);
    pDC->SelectObject(pOldFont);

    int totalWidth = (m_hIcon ? iconSize + gap : 0) + sizeText.cx;

    // Điểm bắt đầu X để toàn bộ (icon + text) nằm giữa button
    int startX = rc.left + (rc.Width() - totalWidth) / 2;
    int centerY = rc.top + rc.Height() / 2;

    // Vẽ icon
    if (m_hIcon && m_imgList.GetSafeHandle())
    {
        int iconX = startX;
        int iconY = centerY - iconSize / 2;
        m_imgList.Draw(pDC, 0, CPoint(iconX, iconY), ILD_TRANSPARENT);
        startX += iconSize + gap;
    }

    // Vẽ text căn giữa theo Y
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(bEnabled ? m_clrText : RGB(220, 220, 220));
    pOldFont = pDC->SelectObject(GetFont());

    CRect rcText(startX, rc.top, startX + sizeText.cx, rc.bottom);
    pDC->DrawText(strText, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    pDC->SelectObject(pOldFont);
}

void CCustomButton::DrawBackground(CDC* pDC, CRect& rc, COLORREF clr)
{
    // Dùng GDI path để vẽ rounded rect
    CBrush brush(clr);
    CPen   pen(PS_SOLID, 1, clr);
    CBrush* pOld = pDC->SelectObject(&brush);
    CPen* pOldP = pDC->SelectObject(&pen);

    pDC->RoundRect(rc, CPoint(8, 8));  // bo góc 8px

    pDC->SelectObject(pOld);
    pDC->SelectObject(pOldP);
}

void CCustomButton::DrawBorder(CDC* pDC, CRect& rc, COLORREF clr)
{
    CPen pen(PS_SOLID, 1, clr);
    CPen* pOld = pDC->SelectObject(&pen);
    pDC->SelectStockObject(NULL_BRUSH);
    pDC->RoundRect(rc, CPoint(8, 8));
    pDC->SelectObject(pOld);
}

void CCustomButton::DrawText(CDC* pDC, CRect& rc, const CString& text, BOOL bEnabled)
{
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(bEnabled ? m_clrText : RGB(220, 220, 220));

    CFont* pOldFont = pDC->SelectObject(GetFont());
    pDC->DrawText(text, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    pDC->SelectObject(pOldFont);
}

// ---- Mouse tracking ----
void CCustomButton::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bHover)
    {
        m_bHover = TRUE;
        Invalidate();

        // Đăng ký nhận WM_MOUSELEAVE khi chuột rời khỏi
        TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, m_hWnd, 0 };
        TrackMouseEvent(&tme);
    }
    CButton::OnMouseMove(nFlags, point);
}

void CCustomButton::OnMouseLeave()
{
    m_bHover = FALSE;
    m_bPressed = FALSE;
    Invalidate();
}

void CCustomButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_bPressed = TRUE;
    Invalidate();
    CButton::OnLButtonDown(nFlags, point);
}

void CCustomButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bPressed = FALSE;
    Invalidate();
    CButton::OnLButtonUp(nFlags, point);
}