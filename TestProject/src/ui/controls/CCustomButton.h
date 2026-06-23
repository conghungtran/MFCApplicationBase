#pragma once
#include <afxwin.h>



class CCustomButton : public CButton
{
public:
    CCustomButton();
    virtual ~CCustomButton();

    // Setter màu sắc
    void SetColors(COLORREF clrNormal, COLORREF clrHover,
        COLORREF clrPressed, COLORREF clrText);
    void SetIcon(HICON hIcon); // Set Icon
protected:
    // Owner-draw
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS) override;

    // Mouse tracking
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

    DECLARE_MESSAGE_MAP()

private:
    COLORREF m_clrNormal = RGB(24, 95, 165);   // blue
    COLORREF m_clrHover = RGB(55, 138, 221);   // blue nhạt hơn
    COLORREF m_clrPressed = RGB(15, 70, 130);   // blue đậm hơn
    COLORREF m_clrText = RGB(255, 255, 255);  // trắng

    BOOL m_bHover = FALSE;
    BOOL m_bPressed = FALSE;
    HICON m_hIcon = nullptr; // Icon
    CImageList  m_imgList;

    void DrawBackground(CDC* pDC, CRect& rc, COLORREF clr);
    void DrawText(CDC* pDC, CRect& rc, const CString& text, BOOL bEnabled);
    void DrawBorder(CDC* pDC, CRect& rc, COLORREF clr);
};