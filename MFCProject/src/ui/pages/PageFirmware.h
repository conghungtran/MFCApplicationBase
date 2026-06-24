#pragma once
#include "afxdialogex.h"


// PageFirmware dialog

class PageFirmware : public CDialogEx
{
	DECLARE_DYNAMIC(PageFirmware)

public:
	PageFirmware(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PageFirmware();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAGE_FIRMWARE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
