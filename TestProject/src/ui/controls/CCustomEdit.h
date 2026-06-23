#pragma once

class CCustomEdit : public CEdit
{
    DECLARE_DYNAMIC(CCustomEdit)

public:
    CCustomEdit();
    virtual ~CCustomEdit();

    // Màu sắc
    void SetTextColor(COLORREF color) { m_clrText = color; }
    void SetBackgroundColor(COLORREF color) { m_clrBackground = color; }
    void SetBorderColor(COLORREF color) { m_clrBorder = color; }
    void SetPlaceholderColor(COLORREF color) { m_clrPlaceholder = color; }
    void SetFocusBorderColor(COLORREF color) { m_clrFocusBorder = color; }


    // Placeholder text
    void SetPlaceholder(const CString& strPlaceholder);

    // Font
    void SetEditFont(int nHeight, const CString& strFaceName = _T("Segoe UI"));

protected:
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg void OnSetFocus(CWnd* pOldWnd);      // ✅ Dùng ON_WM_SETFOCUS
    afx_msg void OnKillFocus(CWnd* pNewWnd);     // ✅ Dùng ON_WM_KILLFOCUS
    afx_msg void OnEnChange();                    // Để theo dõi thay đổi nội dung

    DECLARE_MESSAGE_MAP()

private:
    void UpdatePlaceholder();

    COLORREF m_clrText;
    COLORREF m_clrBackground;
    COLORREF m_clrBorder;
    COLORREF m_clrFocusBorder;    // Màu viền khi focus
    COLORREF m_clrPlaceholder;
    CString m_strPlaceholder;
    BOOL m_bShowPlaceholder;
    CFont m_font;
    CPen m_penBorder;
    BOOL m_bHasFocus;              // Có đang focus không?
};