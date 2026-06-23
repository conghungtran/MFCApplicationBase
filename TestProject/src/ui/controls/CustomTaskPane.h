#pragma once
#include "CCustomButton.h"

class CSidePanelPane : public CDockablePane
{
    DECLARE_DYNAMIC(CSidePanelPane)

public:
    CSidePanelPane();
    virtual ~CSidePanelPane();

protected:
    CCustomButton m_btnAddPrinter;
    CCustomButton m_btnRemovePrinter;
    CCustomButton m_btnCreateTicket;
    CFont         m_font;

    void CreateButtons();
    void SetButtonColors(CCustomButton& btn);

    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();

    DECLARE_MESSAGE_MAP()
};