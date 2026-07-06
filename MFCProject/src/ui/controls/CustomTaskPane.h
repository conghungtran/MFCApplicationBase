#pragma once
#include "CCustomButton.h"

class CSidePanelPane : public CDockablePane
{
    DECLARE_DYNAMIC(CSidePanelPane)

public:
    CSidePanelPane();
    virtual ~CSidePanelPane();

protected:
    CCustomButton m_btnPagePrinter;
    //CCustomButton m_btnPageTicket;
    CCustomButton m_btnPageFunction;

    CCustomButton m_btnSettings;
    CCustomButton m_btnLogout;

    CFont         m_font;

    void CreateButtons();
    void SetButtonColors(CCustomButton& btn);

    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()
};