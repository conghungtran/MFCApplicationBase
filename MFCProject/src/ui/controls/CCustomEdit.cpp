#include "pch.h"
#include "CCustomEdit.h"

IMPLEMENT_DYNAMIC(CCustomEdit, CEdit)

CCustomEdit::CCustomEdit()
    : m_clrText(RGB(0, 0, 0))
    , m_clrBackground(RGB(255, 255, 255))
    , m_clrBorder(RGB(200, 200, 200))
    , m_clrFocusBorder(RGB(0, 120, 215))
    , m_clrPlaceholder(RGB(128, 128, 128))
    , m_bShowPlaceholder(FALSE)
    , m_bHasFocus(FALSE)
{
}

CCustomEdit::~CCustomEdit()
{
}

BEGIN_MESSAGE_MAP(CCustomEdit, CEdit)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_SETFOCUS()      // ✅ Xử lý khi nhận focus
    ON_WM_KILLFOCUS()     // ✅ Xử lý khi mất focus
    //ON_EN_CHANGE()        // Xử lý khi nội dung thay đổi
END_MESSAGE_MAP()

void CCustomEdit::SetPlaceholder(const CString& strPlaceholder)
{
    m_strPlaceholder = strPlaceholder;
    UpdatePlaceholder();
}

void CCustomEdit::UpdatePlaceholder()
{
    CString strText;
    GetWindowText(strText);

    // Nếu đang focus, không hiển thị placeholder
    if (m_bHasFocus)
    {
        if (m_bShowPlaceholder)
        {
            m_bShowPlaceholder = FALSE;
            SetWindowText(_T(""));
        }
        return;
    }

    // Nếu không focus và text rỗng, hiển thị placeholder
    if (strText.IsEmpty() || (m_bShowPlaceholder && strText == m_strPlaceholder))
    {
        m_bShowPlaceholder = TRUE;
        SetWindowText(m_strPlaceholder);
    }
}

void CCustomEdit::OnSetFocus(CWnd* pOldWnd)
{
    m_bHasFocus = TRUE;

    // Xóa placeholder khi focus
    if (m_bShowPlaceholder)
    {
        m_bShowPlaceholder = FALSE;
        SetWindowText(_T(""));
    }

    // Vẽ lại viền
    Invalidate();

    CEdit::OnSetFocus(pOldWnd);
}

void CCustomEdit::OnKillFocus(CWnd* pNewWnd)
{
    m_bHasFocus = FALSE;

    CString strText;
    GetWindowText(strText);

    // Hiển thị placeholder nếu rỗng
    if (strText.IsEmpty())
    {
        m_bShowPlaceholder = TRUE;
        SetWindowText(m_strPlaceholder);
    }

    // Vẽ lại viền
    Invalidate();

    CEdit::OnKillFocus(pNewWnd);
}

void CCustomEdit::OnEnChange()
{
    // Khi user nhập text, nếu đang hiển thị placeholder thì xóa
    if (m_bShowPlaceholder)
    {
        m_bShowPlaceholder = FALSE;
        SetWindowText(_T(""));
    }
}

void CCustomEdit::SetEditFont(int nHeight, const CString& strFaceName)
{
    m_font.DeleteObject();
    m_font.CreateFont(nHeight, 0, 0, 0, FW_NORMAL, FALSE, FALSE,
        0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, strFaceName);
    SetFont(&m_font);
}

HBRUSH CCustomEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
    // Set màu chữ
    if (m_bShowPlaceholder)
        pDC->SetTextColor(m_clrPlaceholder);
    else
        pDC->SetTextColor(m_clrText);

    // Set màu nền
    pDC->SetBkColor(m_clrBackground);

    // Vẽ viền
    CPen pen(PS_SOLID, 1, m_bHasFocus ? m_clrFocusBorder : m_clrBorder);
    CPen* pOldPen = pDC->SelectObject(&pen);
    CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

    CRect rect;
    GetClientRect(rect);
    pDC->Rectangle(rect);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

    // Tạo brush cho nền
    static CBrush brush;
    brush.DeleteObject();
    brush.CreateSolidBrush(m_clrBackground);

    return (HBRUSH)brush;
}