// AppToolBar.h
#pragma once
#include "CSearchCtrl.h"
#include "CCustomButton.h"
#include "ImportExportService.h"
#include "AppMessages.h"

class BookPageToolBar : public CWnd
{
    DECLARE_DYNAMIC(BookPageToolBar)

public:
    BOOL Create(CWnd* pParent, UINT nID, const CRect& rect);
    

protected:
    CCustomButton  m_btnAdd;
    CCustomButton  m_btnDelete;

    CCustomButton  m_btnImport;
    CCustomButton  m_btnExport;

    CSearchCtrl    m_searchCtrl;

    afx_msg int  OnCreate(LPCREATESTRUCT lpcs);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg LRESULT OnSearch(WPARAM wParam, LPARAM lParam);
    afx_msg void OnAddClicked();
    afx_msg void OnDeleteClicked();

    afx_msg void OnImportClicked();
    afx_msg void OnExportClicked();

    afx_msg void OnBnClickedBtnImportSync();
    static void OnAsyncImportCallback(const ImportResult& result, void* context);
    afx_msg LRESULT OnImportAsyncDone(WPARAM wParam, LPARAM lParam);


    DECLARE_MESSAGE_MAP()


private:
    void RepositionControls();
    void CreateButtons();

public:
    void SetImportExportService(std::shared_ptr<CImportExportService> service) { m_importExportService = service; }

    std::shared_ptr<CImportExportService> m_importExportService;
};